/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

SQuadID addquad;

bool curSecDataIsNew = false;

LRESULT CALLBACK AddQuadDlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hWnd, IDC_QUADTYPE, EM_SETLIMITTEXT, (WPARAM)4, (LPARAM)0);
		break;
	case WM_CLOSE:
	case WM_QUIT:
		EndDialog(hWnd,0);
	break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				wchar_t buffer[5];
				SendMessage(GetDlgItem(hWnd, IDC_QUADTYPE), WM_GETTEXT, (WPARAM)4+1, (LPARAM)buffer);
				//Check for bad QUAD
				if  (_GetStringType(buffer) == STR_UNICODE)
				{
					MessageBox(refchild::hWnd, QUAD_UNICODE_USED, L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				if (wcslen(buffer) != 4)
				{
					MessageBox(refchild::hWnd,QUAD_FOUR_CHARACTERS, L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				SQuadID newquad;
				newquad.type = ConvertStringToQuad(buffer);
				newquad.id = GetValueFromHWND(GetDlgItem(hWnd, IDC_QUADID));
				if (dmmfile.getQuad(newquad, NULL))
				{
					MessageBox(refchild::hWnd,QUAD_QUADID_EXISTS, L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				addquad = newquad;
				EndDialog(hWnd,1);
			}
			break;
		case IDCANCEL:
			EndDialog(hWnd,0);
			break;
		}
		break;
	}
	return 0;
}

void MoveQuadUP()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCURSEL, 0, 0);
	if (cursel == 0) return;
	if (cursel == LB_ERR) return;
	unsigned long uniqueid = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETITEMDATA, cursel, 0);
	if (!dmmfile.moveQuadUp(uniqueid)) return;
	
	unsigned short textlen = (unsigned short)SendMessage(maindlg::hWndQuadList,LB_GETTEXTLEN, (WPARAM)cursel, 0);
	wchar_t *strhold = new wchar_t[textlen + 1];
	SendMessage(maindlg::hWndQuadList,LB_GETTEXT, (WPARAM)cursel, (LPARAM)strhold);
	strhold[textlen] = 0;
	SendMessage(maindlg::hWndQuadList,LB_DELETESTRING,(WPARAM)cursel, 0);
	unsigned long listnum = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_INSERTSTRING, cursel - 1,(LPARAM) strhold);
	SendMessage(maindlg::hWndQuadList, LB_SETITEMDATA, listnum,(LPARAM)uniqueid);
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)listnum, 0);
	delete [] strhold;
}

void MoveQuadDOWN()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCURSEL, 0, 0);
	unsigned long numitems = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCOUNT, 0, 0);
	if (cursel >= numitems - 1) return;
	if (cursel == LB_ERR) return;
	unsigned long uniqueid = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETITEMDATA, cursel, 0);
	if (!dmmfile.moveQuadDown(uniqueid)) return;
	
	unsigned short textlen = (unsigned short)SendMessage(maindlg::hWndQuadList,LB_GETTEXTLEN, (WPARAM)cursel, 0);
	wchar_t *strhold = new wchar_t[textlen + 1];
	SendMessage(maindlg::hWndQuadList,LB_GETTEXT, (WPARAM)cursel, (LPARAM)strhold);
	strhold[textlen] = 0;
	SendMessage(maindlg::hWndQuadList,LB_DELETESTRING,(WPARAM)cursel, 0);
	unsigned long listnum = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_INSERTSTRING, cursel + 1,(LPARAM) strhold);
	SendMessage(maindlg::hWndQuadList, LB_SETITEMDATA, listnum,(LPARAM)uniqueid);
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)listnum, 0);
	delete [] strhold;
}

void RemoveQuad()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR) return;
	unsigned long uniqueid = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETITEMDATA, cursel, 0);
	if (!dmmfile.removeQuad(uniqueid)) return;
	SendMessage(maindlg::hWndQuadList,LB_DELETESTRING,(WPARAM)cursel, 0);
	unsigned long numitems = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCOUNT, (WPARAM)cursel,0);
	if (cursel >= numitems) cursel = numitems - 1;
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)cursel,0);
	ChangeQuad();
}

void AddQuad()
{
	if (!DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_ADDQUAD) ,maindlg::hWnd, (DLGPROC)AddQuadDlgProc)) return;
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCURSEL, 0, 0) + 1;
	if (cursel == LB_ERR) cursel = 0;
	SQuad *newquad = new SQuad;
	newquad->quadid = addquad;
	newquad->mode = 0;
	unsigned long uniqueid;
	if ((uniqueid = dmmfile.addQuad(newquad,cursel)) == ERROR_LNG) return;
	std::wstringstream ss;
	ss << ConvertQuadToString(newquad->quadid.type) << L" - " << newquad->quadid.id;
	unsigned long listnum = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_INSERTSTRING, cursel,(LPARAM) ss.str().c_str());
	SendMessage(maindlg::hWndQuadList, LB_SETITEMDATA, listnum,(LPARAM)uniqueid);
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)cursel, 0);
	ChangeQuad();
}

void RemoveReference()
{
	unsigned long cursel = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR) return;
	SendMessage(refchild::hWndRefList, LB_DELETESTRING, (WPARAM)cursel,0);
	curquad.references.removeReference((unsigned short)cursel);
	unsigned long numitems = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETCOUNT, 0,0);
	if (cursel >= numitems) cursel = numitems - 1;
	SendMessage(refchild::hWndRefList, LB_SETCURSEL, (WPARAM)cursel,0);
	ChangeMade();
}

void AddReference()
{
	wchar_t buffer[5];
	SendMessage(refchild::hWndQuadTxt, WM_GETTEXT, (WPARAM)4+1, (LPARAM)buffer);
	//Check for bad QUAD
	if  (_GetStringType(buffer) == STR_UNICODE)
	{
		MessageBox(refchild::hWnd, QUAD_UNICODE_USED, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	if (wcslen(buffer) != 4)
	{
		MessageBox(refchild::hWnd, QUAD_FOUR_CHARACTERS, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	///////////////////////////
	FF_3dmmIndexReference newref;
	newref.type = ConvertStringToQuad(buffer);
	newref.id = GetValueFromHWND(refchild::hWndIDTxt);
	newref.refid = GetValueFromHWND(refchild::hWndRefIDTxt);
	if (curquad.quadid.type == newref.type && curquad.quadid.id == newref.id)
	{
		MessageBox(refchild::hWnd, QUAD_REFERENCED_ITSELF, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	unsigned long insertnum;
	if ((insertnum = curquad.references.addReference(newref)) == ERROR_SHRT)
	{
		MessageBox(refchild::hWnd, QUAD_REFID_EXISTS, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	SQuad quad;
	unsigned long listnum = (unsigned long)SendMessage(refchild::hWndRefList, LB_INSERTSTRING,(WPARAM)insertnum, (LPARAM)GenQuadReferenceItemString(newref,&quad).c_str());
	SendMessage(refchild::hWndRefList, LB_SETITEMDATA, (WPARAM)listnum,(LPARAM)quad.uniqueid);
	SendMessage(refchild::hWndRefList, LB_SETCURSEL, (WPARAM)listnum, 0);
	SetQuadSelectMode(false);
	addingref = false;
	EnableWindow(refchild::hWndRefList, true);
	ChangeMade();
}

void ExportSection()
{
	std::wstring filename;
	std::wstring filetitle;
	SQuad tmpquad;
	if (!dmmfile.getQuad(curquad.uniqueid, &tmpquad)) return;
	std::wstringstream extensions;
	std::wstring type = ConvertQuadToString(tmpquad.quadid.type);
	extensions << type << L" Files (*." << type << L")|*." << type << L"|All Files (*.*)|*.*|";
	std::wstringstream newfiletitle;
	std::wstring strplace;
	if (tmpquad.string.getStringType() > STR_NONE) strplace = L" - " + tmpquad.string.getString();
	else strplace = L"";
	newfiletitle << tmpquad.quadid.id << strplace << L"." << type;
	if (!ShowSaveDlg(maindlg::hWnd, L"Export Section",extensions.str(),newfiletitle.str(),&filename, &filetitle)) return;
	size_t lendata = curquad.section.getSectionDataSize();
	//////////////////////////////////
	////// Prompt Decompression //////
	//////////////////////////////////

	BYTE *sectiondatapointer = curquad.section.getSectionData();

	bool decompress = false;
	
	if (curquad.section.isSectionCompressed())
	{
		int retbox = MessageBox(maindlg::hWnd,L"This section is compressed, do you wish to export the decompressed version?", L"Section is Compressed", MB_YESNOCANCEL | MB_ICONQUESTION);
		switch (retbox)
		{
		case IDCANCEL:
			return;
			break;
		case IDNO:
			decompress = false;
			break;
		case IDYES:
			decompress = true;
			break;
		}
	}
	//////////////////////////////////
	//////////////////////////////////

	if (decompress)
	{
		int uncompressedlength = GetSize(sectiondatapointer, lendata);
		BYTE *uncompressed = new BYTE[uncompressedlength];
		DecompressSmart(sectiondatapointer, lendata, uncompressed);
		sectiondatapointer = uncompressed;
		lendata = uncompressedlength;
	}
	
	FILE *fp = _wfopen(filename.c_str(), L"wb");
	if (fp == NULL)
	{
		MessageBox(maindlg::hWnd, L"Failed to open file for writing.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (lendata == 0)
	{
		fclose(fp);
		return;
	}
	if (fwrite(sectiondatapointer,sizeof(BYTE), lendata, fp) != lendata)
	{
		MessageBox(maindlg::hWnd, L"Error writing to file, written file may be corrupted.", L"Error", MB_OK | MB_ICONEXCLAMATION);
	}
	if (decompress)
	{
		delete [] sectiondatapointer;
	}
	fclose(fp);
	return;
}

void ImportSection()
{
	std::wstring filename;
	std::wstring filetitle;
	SQuad tmpquad;
	if (!dmmfile.getQuad(curquad.uniqueid, &tmpquad)) return;
	std::wstringstream extensions;
	std::wstring type = ConvertQuadToString(tmpquad.quadid.type);
	extensions << type << L" Files (*." << type << L")|*." << type << L"|All Files (*.*)|*.*|";
	if (!ShowOpenDlg(maindlg::hWnd, L"Import Section",extensions.str(),&filename, &filetitle)) return;
	FILE *fp = _wfopen(filename.c_str(), L"rb");
	if (fp == NULL)
	{
		MessageBox(maindlg::hWnd, L"Failed to open file for reading.", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	unsigned long filesize = fsize(fp);
	fseek(fp,SEEK_SET,0);
	if (filesize == 0)
	{
		if (curSecDataIsNew && curquad.section.getSectionData() != NULL) delete [] curquad.section.getSectionData();
		curquad.section.setSectionData(NULL, 0);
		curSecDataIsNew = false;
		fclose(fp);
		return;
	}
	
	BYTE *newsection = new BYTE[filesize];
	fread(newsection, sizeof(BYTE), filesize, fp);
	curquad.section.setSectionData(newsection,filesize);
	fclose(fp);
	curSecDataIsNew = true;
	DisplaySectionInfo();
	ChangeMade();
}

void SetCurSection(BYTE* data, size_t size)
{
	if (curSecDataIsNew)
	{
		delete [] curquad.section.getSectionData();
	}
	curquad.section.setSectionData(data, size);
	curSecDataIsNew = true;
}