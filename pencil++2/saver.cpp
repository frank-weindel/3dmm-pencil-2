/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

bool CPencil::save()
{
	if (fpath == L"")
	{
		raiseError(PERROR_NOFPATHEXISTS);
		return false;
	}
	else
	{
		saveAs(fpath, false);
	}
}

bool CPencil::saveAs(std::wstring path, bool copy)
{
	FILE *mainfp = _wfopen(path.c_str(), L"wb");
	if (!mainfp)
	{
		raiseError(PERROR_FOPENSAVE);
		return false;
	}
	FF_3dmmHeader head;
	head.identifier = ConvertStringToQuad("2NHC"); //our main man!
	head.magicnum = header.magicnum;
	head.signature = header.signature;
	head.unk1 = header.unk1;
	head.unk2 = header.unk2;
	memset(head.zeros,0,96); //set zeros to 0
	//allocate room for header by skipping it
	fseek(mainfp,sizeof(FF_3dmmHeader), SEEK_CUR);
	//Write each section and store its location
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
	{
		SQuad *quadptr = (*itr);
		BYTE* data = quadptr->section.getSectionData();
		size_t datalen = quadptr->section.getSectionDataSize();
		quadptr->temp.sectionoffset = ftell(mainfp);
		quadptr->temp.sectionsize = datalen;
		fwrite(data, sizeof(BYTE), datalen, mainfp);
	}
	//Prepare Index Header Data, but do not write until we finish writing the index
	FF_3dmmIndexHeader indexhead;
	indexhead.magicnum = head.magicnum;
	indexhead.numsections = quads.size();
	indexhead.unk20 = 20;
	indexhead.FFFFFFFF = 0xFFFFFFFF;
	//we must set indexhead.quadindexlen after we know how long it is then we write this struct to the file
	head.indexoffset = ftell(mainfp); //set header index offset;

	head.indexlen = sizeof(FF_3dmmIndexHeader); //start lencount at the size of the header
	//Skip over and allocate room for the index header
	fseek(mainfp, sizeof(FF_3dmmIndexHeader), SEEK_CUR);

	//WRITE INDEX!!!!
	indexhead.quadindexlen = 0;
	for (std::vector<SQuad*>::iterator itr = quadindexsort.quadpointers.begin(); itr != quadindexsort.quadpointers.end(); itr++)
	{
		SQuad* quadptr = (*itr);
		quadptr->temp.quadoffset = ftell(mainfp) - head.indexoffset - sizeof(FF_3dmmIndexHeader);
		quadptr->temp.quadsize = 0;
		FF_3dmmIndexQuad indexquad;
		indexquad.type = quadptr->quadid.type;
		indexquad.id = quadptr->quadid.id;
		indexquad.secoffset = quadptr->temp.sectionoffset;
		indexquad.mode = GetQuadMode(quadptr);
		memcpy(&indexquad.seclength,&quadptr->temp.sectionsize,3);
		indexquad.numreferences = quadptr->references.getNumReferences();
		indexquad.timesreferenced = quadptr->timesreferenced;
		fwrite(&indexquad,sizeof(FF_3dmmIndexQuad), 1, mainfp);
		quadptr->temp.quadsize += sizeof(FF_3dmmIndexQuad);
		//References
		for (unsigned short refnum = 0; refnum < indexquad.numreferences; refnum++)
		{
			FF_3dmmIndexReference reference;
			if (!quadptr->references.getReference(refnum, &reference)) continue;
			fwrite(&reference,sizeof(FF_3dmmIndexReference), 1, mainfp);
			quadptr->temp.quadsize += sizeof(FF_3dmmIndexReference);
		}
		//String
		int stringtype = quadptr->string.getStringType();
		int extraBytes = 0;
		long zero = 0;
		unsigned short stringid;
		if (quadptr->string.getStringType())
		{
			switch (head.magicnum)
			{
			case MN_1033:
				{
					stringid = 0x0303;
					fwrite(&stringid, sizeof(unsigned short),1, mainfp);
					std::wstring string = quadptr->string.getString();
					int requiredsize = WideCharToMultiByte(CP_ACP,0,string.c_str(), -1, NULL, 0, NULL, NULL);
					char *cvString = new char[requiredsize];
					requiredsize = WideCharToMultiByte(CP_ACP,0,string.c_str(), -1, cvString, requiredsize, NULL, NULL);
					unsigned char strlen = requiredsize - 1;
					fwrite(&strlen, sizeof(unsigned char), 1, mainfp);
					fwrite(cvString, sizeof(char),requiredsize,mainfp);
					delete [] cvString;
					if ((strlen) % 4 == 0)
					{
						extraBytes = 0;
					}
					else
					{
						extraBytes = 4 - ((strlen) % 4);
					}
					fseek(mainfp,extraBytes,SEEK_CUR);
					quadptr->temp.quadsize += 2 + 1 + requiredsize;
					break;
				}
			case MN_1055:
				{
					stringid = 0x0505;
					fwrite(&stringid, sizeof(unsigned short),1, mainfp);
					std::wstring string = quadptr->string.getString();
					unsigned short strlen = (unsigned short)string.size();
					fwrite(&strlen, sizeof(unsigned short),1,mainfp);
					fwrite(string.c_str(),sizeof(wchar_t),strlen + 1,mainfp);
					if ((strlen) % 2 == 0)
					{
						extraBytes = 2;
					}
					else
					{
						extraBytes = 0;
					}
					fseek(mainfp,extraBytes,SEEK_CUR);
					quadptr->temp.quadsize += 2 + 2 + (strlen * 2) + 2;
					break;
				}
			}
		}
		indexhead.quadindexlen += quadptr->temp.quadsize + extraBytes; //add on quadsize to total quadindex length
		
	}
	head.indexlen += indexhead.quadindexlen; //add on total quadindex length onto total index length
	//DIRECTORY writer
	for (std::vector<SQuad*>::iterator itr = directorysort.quadpointers.begin(); itr != directorysort.quadpointers.end(); itr++)
	{
		SQuad* quadptr = (*itr);
		FF_3dmmDirectoryEntry direntry;
		direntry.offset = quadptr->temp.quadoffset;
		direntry.length = quadptr->temp.quadsize;
		fwrite(&direntry, sizeof(FF_3dmmDirectoryEntry), 1, mainfp);
		head.indexlen += sizeof(FF_3dmmDirectoryEntry);
	}
	head.filelen = fsize(mainfp);
	head.filelen2 = head.filelen;
	fseek(mainfp,0,SEEK_SET);
	fwrite(&head,sizeof(FF_3dmmHeader),1,mainfp);
	fgoto(mainfp, head.indexoffset);
	fwrite(&indexhead, sizeof(FF_3dmmIndexHeader),1, mainfp);

	fclose(mainfp);

	if (!copy) fpath = path;
	return true;
}