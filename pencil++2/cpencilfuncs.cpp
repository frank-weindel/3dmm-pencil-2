/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

CPencil::CPencil()
{
	fileIsOpen = false;
	lastuniqueid = 0;
	directorysort.setSortType(GETQUAD_DIRSORT);
}
CPencil::~CPencil()
{
	close();
}

unsigned long CPencil::getQuadNum(unsigned long uniqueid, int quadsort)
{
	for (unsigned long quadnum = 0; quadnum < quads.size(); quadnum++)
	{
		switch (quadsort)
		{
		case GETQUAD_DIRSORT:
			if (quads[quadnum]->uniqueid == uniqueid) return quadnum;
			break;
		case GETQUAD_QUADORDERSORT:
			if (quadindexsort.getQuadPT(quadnum)->uniqueid == uniqueid) 
			{
				return quadnum;
			}
			break;
		}
	}
	return 0xFFFFFFFF;
}

bool CPencil::getQuad(unsigned long uniqueid, SQuad *retQuad)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++) //Find Quad
	{
		if ((*itr)->uniqueid == uniqueid)
		{
			retQuad->mode = (*itr)->mode;
			retQuad->quadid = (*itr)->quadid;
			retQuad->references = (*itr)->references;
			retQuad->section = (*itr)->section;
			retQuad->string = (*itr)->string;
			retQuad->temp = (*itr)->temp;
			retQuad->uniqueid = (*itr)->uniqueid;
			return true;
		}
	}
	return false;
}

SQuad CPencil::getQuad(unsigned long quadnum, int quadsort) 
{
	unsigned long numquads = (unsigned long)quads.size();
	if (quadnum < quads.size())
	{
		switch (quadsort)
		{
		case GETQUAD_DIRSORT:
			return (*(quads[quadnum]));
			break;
		case GETQUAD_QUADORDERSORT:
			return (*(quadindexsort.getQuadPT(quadnum)));
			break;
		default:
			return (*(quads[quadnum]));
			break;
		}
	}
	else
	{
		SQuad error;
		error.quadid.id = -1;
		error.mode = -1;
		error.quadid.type = 0xFFFFFFFF;
		raiseError(PERROR_INVALIDQUADNUM);
		return error;
	}
	
};

bool CPencil::getQuad(SQuadID quadid, SQuad *retQuad)
{
	return getQuad(quadid.type, quadid.id, retQuad);
}

bool CPencil::getQuad(quad type, unsigned long id, SQuad *retQuad)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++) //Find Quad
	{
		if ((*itr)->quadid.type == type && (*itr)->quadid.id == id)
		{
			if (retQuad != NULL)
			{
				retQuad->mode = (*itr)->mode;
				retQuad->quadid = (*itr)->quadid;
				retQuad->references = (*itr)->references;
				retQuad->section = (*itr)->section;
				retQuad->string = (*itr)->string;
				retQuad->temp = (*itr)->temp;
				retQuad->uniqueid = (*itr)->uniqueid;
			}
			return true;
		}
	}
return false;
}

bool CPencil::getQuadRefBy(SQuadID quadid, std::vector<SRefBy> *retRefBy)
{
	return getQuadRefBy(quadid.type, quadid.id, retRefBy);
}

bool CPencil::getQuadRefBy(quad type, unsigned long id, std::vector<SRefBy> *retRefBy)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++) //Find Quad
	{
		unsigned short numrefs = (*itr)->references.getNumReferences();
		for (unsigned short refnum = 0; refnum < numrefs; refnum++)
		{
			FF_3dmmIndexReference reference;
			if ((*itr)->references.getReference(refnum,&reference))
			{
				if (reference.type == type && reference.id == id)
				{
					SRefBy refby;
					refby.quadid = (*itr)->quadid;
					refby.refid = reference.refid;
					refby.string = (*itr)->string;
					refby.uniqueid = (*itr)->uniqueid;
					retRefBy->push_back(refby);
				}
			}
		}
	}
	return true;
}

int _GetStringType(const wchar_t *wstr)
{
	size_t strlen = wcslen(wstr);
	if (strlen == 0)
	{
		return STR_NONE;
	}
	for (unsigned int curchar = 0; curchar < strlen; curchar++)
	{
		if (HIBYTE(wstr[curchar]) != 0)
		{
			return STR_UNICODE;
		}
	}
	return STR_ASCII;
	
}

void ReverseQuadStr(char sQuad[5])
{
	char sQuad2[4];
	sQuad2[0] = sQuad[3];
	sQuad2[1] = sQuad[2];
	sQuad2[2] = sQuad[1];
	sQuad2[3] = sQuad[0];

	sQuad[0] = sQuad2[0];
	sQuad[1] = sQuad2[1];
	sQuad[2] = sQuad2[2];
	sQuad[3] = sQuad2[3];
}

int ConvertStrToWStr(const char *str, wchar_t *wstr)
{
	int stringlen = (int)strlen(str);
	for (int strpos = 0; strpos < stringlen;strpos++)
	{
		wstr[strpos] = str[strpos];
	}
	wstr[stringlen] = 0;
	return 1;
}

int ConvertWStrToStr(const wchar_t *wstr, char *str)
{
	int stringlen = (int)wcslen(wstr);
	for (int strpos = 0; strpos < stringlen;strpos++)
	{
		memcpy(&str[strpos],&wstr[strpos],1);
	}
	str[stringlen] = 0;
	return 1;
}

int ConvertQuadToString(quad qQuad, char sQuad[5])
{
	memcpy(sQuad,&qQuad,4);
	ReverseQuadStr(sQuad);
	sQuad[4] = 0;
	
	return 1;
}

int ConvertQuadToString(quad qQuad, wchar_t sQuad[5])
{
	char sQuad2[5];
	memcpy(sQuad2,&qQuad,4);
	ReverseQuadStr(sQuad2);
	sQuad2[4] = 0;
	
	ConvertStrToWStr(sQuad2,sQuad);
	sQuad[4] = 0;
	return 1;
}

std::wstring ConvertQuadToString(quad qQuad)
{
	char sQuad2[5];
	wchar_t sQuad[5];
	memcpy(sQuad2,&qQuad,4);
	ReverseQuadStr(sQuad2);
	sQuad2[4] = 0;
	
	ConvertStrToWStr(sQuad2,sQuad);
	sQuad[4] = 0;
	return (std::wstring(sQuad));
}

quad ConvertStringToQuad(const wchar_t sQuad[5])
{
	quad ret;
	BYTE sQuad2[4];
	sQuad2[3] = (char)sQuad[0];
	sQuad2[2] = (char)sQuad[1];
	sQuad2[1] = (char)sQuad[2];
	sQuad2[0] = (char)sQuad[3];
	memcpy(&ret,sQuad2,4);
	return ret;
}

quad ConvertStringToQuad(const char sQuad[5])
{
	quad ret;
	char sQuad2[4];
	sQuad2[3] = sQuad[0];
	sQuad2[2] = sQuad[1];
	sQuad2[1] = sQuad[2];
	sQuad2[0] = sQuad[3];
	memcpy(&ret,sQuad2,4);
	//wchar_t buff[5];
	//ConvertQuadToString(ret,buff);
	//MessageBox(NULL,buff,buff,MB_OK);
	return ret;
}

int CompareQuads(const char *sQuad1, const char *sQuad2)
{
	if (strlen(sQuad1) != 4 || strlen(sQuad2) != 4)
	{
		return -1;
	}
	if (sQuad1[0] == sQuad2[0] && sQuad1[1] == sQuad2[1] && sQuad1[2] == sQuad2[2] && sQuad1[3] == sQuad2[3])
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CompareQuads(quad qQuad1, const char *sQuad2)
{
	
	if (strlen(sQuad2) != 4 )
	{
		return -1;
	}
	
	char rQuad2[4];
	rQuad2[0] = sQuad2[3];
	rQuad2[1] = sQuad2[2];
	rQuad2[2] = sQuad2[1];
	rQuad2[3] = sQuad2[0];

	quad qQuad2;
	CopyMemory(&qQuad2,rQuad2,4);
	return qQuad1 == qQuad2;
}

size_t FileSize(FILE *fp)
{
	unsigned long curspot = ftell(fp);
	fseek(fp,0,SEEK_END);
	size_t filesize = ftell(fp);
	fseek(fp,curspot,SEEK_SET);
	return filesize;
}

int fgoto(FILE *fp, long offset)
{
	return fseek(fp,-(ftell(fp) - offset),SEEK_CUR);
}

unsigned long fsize(FILE *fp)
{
	unsigned long curoffset = ftell(fp);
	fseek(fp,0,SEEK_END);
	unsigned long filesize = ftell(fp);
	fseek(fp,curoffset, SEEK_SET);
	return filesize;
}

quad GetQuadFromHWND(HWND hWnd)
{
	unsigned short textlen = (unsigned short)SendMessage(hWnd, WM_GETTEXTLENGTH, 0,0);
	wchar_t buffer[5];
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)4+1, (LPARAM)buffer);
	return ConvertStringToQuad(buffer);
}

int fixstring(char *string)
{
	int size = strlen(string) + 1;
	for (int num = 0; num < size; num++)
	{
		if (string[num] == -110)
		{
			string[num] = 0x27;
		}

	}
	return 1;
}