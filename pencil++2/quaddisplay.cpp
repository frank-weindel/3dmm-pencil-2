/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

int ListQuads()
{
	SendMessage(maindlg::hWndQuadList,WM_SETREDRAW,(WPARAM)FALSE,0);
	SendMessage(maindlg::hWndQuadList, LB_RESETCONTENT, 0, 0);
	unsigned long numquads = dmmfile.getNumQuads();
	for (unsigned long quadnum = 0; quadnum < numquads; quadnum++)
	{
		std::wstringstream ss;
		SQuad curquad = dmmfile.getQuad(quadnum, GETQUAD_QUADORDERSORT);
		if (curquad.string.getStringType() > STR_NONE)
		{
			ss << ConvertQuadToString(curquad.quadid.type) << L" - " << curquad.quadid.id << L" \"" << curquad.string.getString() << L"\"";
		}
		else
		{
			ss << ConvertQuadToString(curquad.quadid.type) << L" - " << curquad.quadid.id;
		}
		unsigned long listnum = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_ADDSTRING, 0,(LPARAM) ss.str().c_str());
		SendMessage(maindlg::hWndQuadList, LB_SETITEMDATA, listnum,(LPARAM)curquad.uniqueid);
		
	}
	SendMessage(maindlg::hWndQuadList,WM_SETREDRAW,(WPARAM)TRUE,0);
	return 0;
}


void DisplayReferences(SQuadID quadid, CQuadReferences *references)
{
	SendMessage(refchild::hWndRefList,WM_SETREDRAW,(WPARAM)FALSE,0);
	SendMessage(refchild::hWndRefList, LB_RESETCONTENT, 0, 0);
	unsigned short numrefs = references->getNumReferences();
	for (int refnum = 0; refnum < numrefs; refnum++)
	{
		FF_3dmmIndexReference ref;
		if (!references->getReference(refnum,&ref)) break;
		SQuad quad;
		unsigned long listnum = (unsigned long)SendMessage(refchild::hWndRefList, LB_ADDSTRING, 0, (LPARAM)GenQuadReferenceItemString(ref, &quad).c_str());
		SendMessage(refchild::hWndRefList, LB_SETITEMDATA, listnum,(LPARAM)quad.uniqueid);
	}
	SendMessage(refchild::hWndRefList,WM_SETREDRAW,(WPARAM)TRUE,0);
	std::vector<SRefBy> refby;
	SendMessage(refchild::hWndRefByList, LB_RESETCONTENT, 0, 0);
	if (dmmfile.getQuadRefBy(quadid, &refby))
	{
		SendMessage(refchild::hWndRefByList,WM_SETREDRAW,(WPARAM)FALSE,0);
		for (std::vector<SRefBy>::iterator itr = refby.begin(); itr != refby.end(); itr++)
		{
			std::wstringstream ss;
			if (itr->string.getStringType() > STR_NONE)
			{
				ss << ConvertQuadToString(itr->quadid.type) << L" - " << itr->quadid.id << L" : " << itr->refid << L" \"" << itr->string.getString() << L"\"";
			}
			else
			{
				ss << ConvertQuadToString(itr->quadid.type) << L" - " << itr->quadid.id << L" : " << itr->refid;
			}
			unsigned long listnum = (unsigned long)SendMessage(refchild::hWndRefByList, LB_ADDSTRING, 0, (LPARAM)ss.str().c_str());
			SendMessage(refchild::hWndRefByList, LB_SETITEMDATA, listnum,(LPARAM)itr->uniqueid);
		}
		SendMessage(refchild::hWndRefByList,WM_SETREDRAW,(WPARAM)TRUE,0);
	}
	tabrefloaded = true;
}

void ChangeQuad()
{
	if (!quadselectmode)
	{
		SetCurQuad();
		DisplayQuad();
	}
	else
	{
		SelQuadQSM();
	}
}

void SetCurQuad()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList,	LB_GETCURSEL, 0, 0);

	if (cursel == LB_ERR) return;
	curquad = dmmfile.getQuad(cursel, GETQUAD_QUADORDERSORT);
	if (curSecDataIsNew)
	{
		delete [] curquad.section.getSectionData();
	}
	curSecDataIsNew = false;
	lastcursel = cursel;
}

int DisplayQuad()
{
	tabrefloaded = false;
	tabeditloaded = false;
	tabhexloaded = false;
	
	
	SendMessage(maindlg::hWndQuadTxt, WM_SETTEXT, 0, (LPARAM)ConvertQuadToString(curquad.quadid.type).c_str());
	SendMessage(maindlg::hWndIDTxt, WM_SETTEXT, 0, (LPARAM)ConvertNumberToString(curquad.quadid.id).c_str());
	SendMessage(maindlg::hWndStrTxt, WM_SETTEXT, 0, (LPARAM)curquad.string.getString().c_str());
	DisplaySectionInfo();
	switch (currenttab)
	{
	case TAB_REFERENCES:
		DisplayReferences(curquad.quadid,&curquad.references);
		break;
	case TAB_VIEWEDIT:
		DisplayEditor();
		break;
	case TAB_VIEWHEX:
		SetViewHex();
		break;
	}
	ResetChangeMade();
	return 0;
}

void DisplaySectionInfo()
{
	std::wstringstream lengthstring;
	unsigned long sectiondatasize = curquad.section.getSectionDataSize();
	double megabytes = roundtodecimal(double(double(sectiondatasize) / 1048576), 2);
	double kilobytes = roundtodecimal(double(double(sectiondatasize) / 1024), 2);
	lengthstring << L"Length: ";
	if (kilobytes >= 800.0)
	{
		lengthstring << megabytes << L" mb (" << sectiondatasize << L" bytes)";
	}
	else
	{
		lengthstring << kilobytes << L" kb (" << sectiondatasize << L" bytes)";
	}
	if (curquad.section.isSectionCompressed())
	{
		size_t decomplen = curquad.section.getUncompressedSectionDataSize();
		lengthstring << std::endl << L"Compressed" << std::endl << L"Decomp Len: ";
		megabytes = roundtodecimal(double(double(decomplen) / 1048576), 2);
		kilobytes = roundtodecimal(double(double(decomplen) / 1024), 2);
		if (kilobytes >= 800.0)
		{
			lengthstring << megabytes << L" mb (" << decomplen << L" bytes)";
		}
		else
		{
			lengthstring << kilobytes << L" kb (" << decomplen << L" bytes)";
		}
	}
	else lengthstring << std::endl << L"Not Compressed";
	
	SendMessage(maindlg::hWndOffLenLbl, WM_SETTEXT, 0,(LPARAM)lengthstring.str().c_str());
}

void DisplayReference()
{
	if (!quadselectmode)
	{
		unsigned long cursel = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETCURSEL, 0,0);
		if (cursel == LB_ERR) return;
		FF_3dmmIndexReference reference;
		curquad.references.getReference((unsigned short)cursel, &reference);
		SendMessage(refchild::hWndQuadTxt, WM_SETTEXT, 0, (LPARAM)ConvertQuadToString(reference.type).c_str());
		SendMessage(refchild::hWndIDTxt, WM_SETTEXT, 0, (LPARAM)ConvertNumberToString(reference.id).c_str());
		SendMessage(refchild::hWndRefIDTxt, WM_SETTEXT, 0, (LPARAM)ConvertNumberToString(reference.refid).c_str());
	}
}