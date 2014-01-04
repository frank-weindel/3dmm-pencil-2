/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */
#include "pencil.h"

bool changesMade = false;




int PromptChangesMade()
{
	if (changesMade)
	{
		int ret = MessageBox(maindlg::hWnd,L"Changes have been made to this quad. Save Changes?", L"Changes Made", MB_YESNOCANCEL | MB_ICONQUESTION);
		switch (ret)
		{
		case IDYES:
			return CHANGE_YES;
			break;
		case IDNO:
			return CHANGE_NO;
			break;
		case IDCANCEL:
			return CHANGE_CANCEL;
			break;
		}
	}
	else return CHANGE_YES;
}

void ChangeMade()
{
	EnableWindow(maindlg::hWndApplyBtn, true);
	changesMade = true;
}

void ResetChangeMade()
{
	EnableWindow(maindlg::hWndApplyBtn, false);
	changesMade = false;
}

std::wstring GenQuadReferenceItemString(FF_3dmmIndexReference ref, SQuad *quad)
{
	std::wstringstream ss;
	if (!dmmfile.getQuad(ref.type, ref.id, quad))
	{
		ss << ConvertQuadToString(ref.type) << L" - " << ref.id << L" : " << ref.refid << L" [QUAD NOT FOUND]";
	}
	else
	{
		if (quad->string.getStringType() > STR_NONE)
		{
			ss << ConvertQuadToString(ref.type) << L" - " << ref.id << L" : " << ref.refid << L" \"" << quad->string.getString() << L"\"";
		}
		else
		{
			ss << ConvertQuadToString(ref.type) << L" - " << ref.id << L" : " << ref.refid;
		}
	}
	return ss.str();
}

void ChangeReference()
{
	unsigned long cursel = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETCURSEL, 0,0);
	if (cursel == LB_ERR) return;
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
		MessageBox(refchild::hWnd,QUAD_FOUR_CHARACTERS, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	///////////////////////////
	FF_3dmmIndexReference newref;
	newref.type = ConvertStringToQuad(buffer);
	newref.id = GetValueFromHWND(refchild::hWndIDTxt);
	newref.refid = GetValueFromHWND(refchild::hWndRefIDTxt);

	if (curquad.quadid.type == newref.type && curquad.quadid.id == newref.id)
	{
		MessageBox(refchild::hWnd,QUAD_REFERENCED_ITSELF, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	FF_3dmmIndexReference oldref;
	curquad.references.getReference((unsigned short)cursel, &oldref);
	if (!curquad.references.checkReference(newref, oldref))
	{
		MessageBox(refchild::hWnd, QUAD_REFID_EXISTS, L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	SendMessage(refchild::hWndRefList, LB_DELETESTRING, (WPARAM)cursel, 0);
	curquad.references.removeReference((unsigned short)cursel);
	unsigned short insertnum;
	insertnum = curquad.references.addReference(newref);
	SQuad quad;
	unsigned long listnum = (unsigned long)SendMessage(refchild::hWndRefList, LB_INSERTSTRING, (WPARAM)insertnum, (LPARAM)GenQuadReferenceItemString(newref,&quad).c_str());
	SendMessage(refchild::hWndRefList, LB_SETITEMDATA, (WPARAM)listnum,(LPARAM)quad.uniqueid);
	SendMessage(refchild::hWndRefList, LB_SETCURSEL, (WPARAM)listnum, 0);
	
}

void ApplyChanges()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR) return;
	wchar_t buffer[5];
	SendMessage(maindlg::hWndQuadTxt, WM_GETTEXT, (WPARAM)4+1, (LPARAM)buffer);
	//Check for bad QUAD
	if  (_GetStringType(buffer) == STR_UNICODE)
	{
		MessageBox(maindlg::hWnd, QUAD_UNICODE_USED, L"Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	if (wcslen(buffer) != 4)
	{
		MessageBox(maindlg::hWnd,QUAD_FOUR_CHARACTERS, L"Error", MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	///////////////////////////
	curquad.quadid.type = ConvertStringToQuad(buffer);
	curquad.quadid.id = GetValueFromHWND(maindlg::hWndIDTxt);
	unsigned short strlen = (unsigned short)SendMessage(maindlg::hWndStrTxt, WM_GETTEXTLENGTH, 0, 0);
	wchar_t *stringbuffer = new wchar_t[strlen + 1];
	SendMessage(maindlg::hWndStrTxt, WM_GETTEXT, (WPARAM)strlen + 1, (LPARAM)stringbuffer);
	stringbuffer[strlen] = 0;
	curquad.string.setString(stringbuffer);
	delete [] stringbuffer;
	if (!dmmfile.changeQuad(&curquad))
	{
		MessageBox(maindlg::hWnd, QUAD_QUADID_EXISTS, L"Error", MB_OK | MB_ICONEXCLAMATION );
		return;
	}
	curSecDataIsNew = false;
	std::wstringstream ss;
	if (curquad.string.getStringType() > STR_NONE) ss << ConvertQuadToString(curquad.quadid.type) << L" - " << curquad.quadid.id << L" \"" << curquad.string.getString() << L"\"";
	else ss << ConvertQuadToString(curquad.quadid.type) << L" - " << curquad.quadid.id;
	SendMessage(maindlg::hWndQuadList, LB_DELETESTRING, (WPARAM)cursel,0);
	unsigned long listnum = (unsigned long)SendMessage(maindlg::hWndQuadList, LB_INSERTSTRING, (WPARAM)cursel,(LPARAM) ss.str().c_str());
	SendMessage(maindlg::hWndQuadList, LB_SETITEMDATA, (WPARAM)listnum,(LPARAM)curquad.uniqueid);
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)cursel,0);
	ResetChangeMade();

}