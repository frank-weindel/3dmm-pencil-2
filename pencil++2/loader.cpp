/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

bool CPencil::open(std::wstring path)
{
	fpath = path;
	mainfp = _wfopen(fpath.c_str(), L"rb");
	if (!mainfp) //file error
	{
		raiseError(PERROR_FOPEN);
		return false;
	}
	if (fileIsOpen) { raiseError(PERROR_ALREADYOPENED); return false; }
	if (scan())
	{
		fileIsOpen = true;
		fclose(mainfp);
		return true;
	}
	else
	{
		fileIsOpen = false;
		fclose(mainfp);
		return false;
	}
}

bool CPencil::scan()
{
	if (!mainfp) { raiseError(PERROR_FOPEN); return false; }
	rewind(mainfp);
	FF_3dmmHeader head;
	fread(&head,sizeof(FF_3dmmHeader),1,mainfp); if (ferror(mainfp)) { raiseError(PERROR_NOTVALID); return false; }
	if (!CompareQuads(head.identifier, "2NHC") || head.filelen != head.filelen2 || FileSize(mainfp) != head.filelen || (head.magicnum != MN_1033 && head.magicnum != MN_1055))
	{
		raiseError(PERROR_NOTVALID);
		return false;
	}
	header.signature = head.signature;
	header.unk1 = head.unk1;
	header.unk2 = head.unk2;
	header.magicnum = head.magicnum;
	fgoto(mainfp,head.indexoffset); //Seek to index
	FF_3dmmIndexHeader indexhead; //3DMM Index header struct
	fread(&indexhead,sizeof(FF_3dmmIndexHeader),1,mainfp); //read index header
	//Check format conditions
	if (indexhead.FFFFFFFF != 0xFFFFFFFF || indexhead.unk20 != 20 || (indexhead.magicnum != MN_1033 && indexhead.magicnum != MN_1055))
	{
		raiseError(PERROR_NOTVALID);
		return false;
	}
	unsigned long quaddexoffset = ftell(mainfp); //Get offset to quaddex

	//Directory
	fseek(mainfp,indexhead.quadindexlen,SEEK_CUR); //Seek to directory
	FF_3dmmDirectoryEntry *directory = new FF_3dmmDirectoryEntry[indexhead.numsections];
	fread(directory,sizeof(FF_3dmmDirectoryEntry),indexhead.numsections,mainfp); //Read directory

	//Quads
	for (unsigned int sectnum = 0; sectnum < indexhead.numsections; sectnum++)
	{
		fgoto(mainfp, head.indexoffset + 20 + directory[sectnum].offset);
		unsigned int curpossect = 0;	
		FF_3dmmIndexQuad quaddata;
		fread(&quaddata,sizeof(FF_3dmmIndexQuad),1,mainfp);
		curpossect += sizeof(FF_3dmmIndexQuad);
		if (curpossect > directory[sectnum].length)
		{
			raiseError(PERROR_NOTVALID);
			return false;
		}
		SQuad *tempQuadSection = new SQuad;
		tempQuadSection->temp.quadoffset = directory[sectnum].offset;
		tempQuadSection->temp.quadsize = directory[sectnum].length;
		
		tempQuadSection->quadid.type = quaddata.type;
		tempQuadSection->quadid.id = quaddata.id;
		tempQuadSection->mode = quaddata.mode;
		tempQuadSection->timesreferenced = quaddata.timesreferenced;

		//refs
		FF_3dmmIndexReference *refs = new FF_3dmmIndexReference[quaddata.numreferences];
		fread(refs,sizeof(FF_3dmmIndexReference),quaddata.numreferences,mainfp);
		curpossect += (sizeof(FF_3dmmIndexReference) * quaddata.numreferences);
		if (curpossect > directory[sectnum].length)
		{
			raiseError(PERROR_NOTVALID);
			return false;
		}
		//REFERENCES
		tempQuadSection->references.quads = &quads;
		for (int refnum = 0; refnum < quaddata.numreferences; refnum++)
		{
			tempQuadSection->references.addReference(refs[refnum]);
		}
		delete[] refs;
		//tempQuadSection->quadoffset = directory[sectnum].offset;
		//STRINGZ
		if (curpossect + 3 <= directory[sectnum].length) //Possible String
		{
			unsigned short strtype;
			fread(&strtype,sizeof(short),1,mainfp);
			switch (strtype)
			{
			case 0x0303: //ASCII STRING
				unsigned char strlen;
				fread(&strlen,sizeof(char),1,mainfp);
				curpossect += 3;
				if (curpossect + strlen <= directory[sectnum].length)
				{
					char *str = new char[strlen + 1];
					fread(str,sizeof(char),strlen,mainfp);
					str[strlen] = 0;
					//fixstring(str);
					//wchar_t *wstr = new wchar_t[strlen + 1];
					//ConvertStrToWStr(str,wstr);
					tempQuadSection->string.setString(str);

					delete[] str;
					//delete[] wstr;
					//MessageBox(NULL,tempQuadSection->str.c_str(),L"DSFDS",MB_OK);
					break;
				}
				goto defaultarea;
				break;
			case 0x0505: //UNICODE STRING
				if (curpossect + 4 <= directory[sectnum].length)
				{
					unsigned short strlen;
					fread(&strlen,sizeof(short),1,mainfp);

					curpossect += 4;
					if (curpossect + (strlen * 2) <= directory[sectnum].length)
					{
						wchar_t *wstr = new wchar_t[strlen + 1];
						fread(wstr,sizeof(wchar_t),strlen,mainfp);
						wstr[strlen] = 0;
						tempQuadSection->string.setString(wstr);
						//MessageBox(NULL,tempQuadSection->GetString().c_str(),L"DSFDS",MB_OK);
						break;
					}
					
				}
			default:
defaultarea:
				break;
			}
		}
		//Get Section Data, and Import it
		size_t sectionlen = (quaddata.seclength[2]<<16)|(quaddata.seclength[1]<<8)|quaddata.seclength[0];
		
		fgoto(mainfp, quaddata.secoffset);
		tempQuadSection->temp.sectionoffset = quaddata.secoffset;
		tempQuadSection->temp.sectionsize = sectionlen;
		BYTE *sectiondata = new BYTE[sectionlen];
		fread(sectiondata,sizeof(BYTE),sectionlen,mainfp);
		tempQuadSection->section.setSectionData(sectiondata, (unsigned long)sectionlen);
		
		//delete[] sectiondata;
		tempQuadSection->uniqueid = lastuniqueid++;
		quads.push_back(tempQuadSection);
	}
	std::sort(quads.begin(), quads.end(), SectionOffsetSort());
	quadindexsort.setSortType(GETQUAD_QUADORDERSORT);
	quadindexsort.addPointers(&quads);
	directorysort.addPointers(&quads);
	raiseError(PERROR_NONE);
	return true;
}

bool CPencil::close()
{
	if (fileIsOpen)
	{
		for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
		{
			delete [] (*itr)->section.getSectionData();
			(*itr)->references.clearReferences();
			delete (*itr);
		}
		quads.clear();
		quadindexsort.clear();
		directorysort.clear();
		lastuniqueid = 0;
		fpath.clear();
		//MessageBox(NULL,L"DSFDS",L"SDFDS",MB_OK);
		fileIsOpen = false;
		return true;
	}
	return false;
}
bool CPencil::createNew()
{
	if (fileIsOpen) { raiseError(PERROR_ALREADYOPENED); return false; }
	header.magicnum = MN_1033;
	header.signature = ConvertStringToQuad("CHMP");
	header.unk1 = 0x0005;
	header.unk2 = 0x0004;
	fileIsOpen = true;
	return true;
}