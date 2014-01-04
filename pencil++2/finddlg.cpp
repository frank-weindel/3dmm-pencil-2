/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

namespace finddlg
{
	HWND hWnd;
	HWND hWndQuadTxt;
	HWND hWndIDTxt;
	HWND hWndStrTxt;
	HWND hWndQuadIDOpt;
	HWND hWndStrOpt;
	HWND hWndUpOpt;
	HWND hWndDownOpt;
	HWND hWndFindBtn;
	HWND hWndCancelBtn;
};

LRESULT CALLBACK FindDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			finddlg::hWnd = hWnd;
			finddlg::hWndQuadTxt = GetDlgItem(hWnd,IDC_FIND_TYPE);
			finddlg::hWndIDTxt = GetDlgItem(hWnd,IDC_FIND_ID);
			finddlg::hWndStrTxt = GetDlgItem(hWnd,IDC_FIND_STR);
			finddlg::hWndQuadIDOpt = GetDlgItem(hWnd,IDC_SEARCHBY_QUADID);
			finddlg::hWndStrOpt = GetDlgItem(hWnd,IDC_SEARCHBY_STRING);
			finddlg::hWndUpOpt = GetDlgItem(hWnd,IDC_DIRECT_UP);
			finddlg::hWndDownOpt = GetDlgItem(hWnd,IDC_DIRECT_DOWN);
			finddlg::hWndFindBtn = GetDlgItem(hWnd,IDC_FINDNEXT);
			finddlg::hWndCancelBtn = GetDlgItem(hWnd,IDCANCEL);
			
			SendMessage(finddlg::hWndQuadTxt, EM_SETLIMITTEXT, (WPARAM)4, (LPARAM)0);
			SetWindowLongPtr(hWnd, GWLP_HWNDPARENT,(LONG)(LONG_PTR)maindlg::hWnd);

			SendMessage(finddlg::hWndQuadIDOpt,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			SendMessage(finddlg::hWndDownOpt,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
			return 1;
		}
		break;
	case WM_QUIT:
	case WM_CLOSE:
		finddlg::hWnd = NULL;
		finddlg::hWndQuadTxt = NULL;
		finddlg::hWndIDTxt = NULL;
		finddlg::hWndStrTxt = NULL;
		finddlg::hWndQuadIDOpt = NULL;
		finddlg::hWndStrOpt = NULL;
		finddlg::hWndUpOpt = NULL;
		finddlg::hWndDownOpt = NULL;
		finddlg::hWndFindBtn = NULL;
		finddlg::hWndCancelBtn = NULL;
		EndDialog(hWnd,0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDCANCEL:
			SendMessage(hWnd,WM_CLOSE,0,0);
			break;
		case IDC_FIND_TYPE:
			switch(HIWORD(wParam))
			{
			case EN_SETFOCUS:
				SendMessage(finddlg::hWndQuadIDOpt,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(finddlg::hWndStrOpt,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
				//MessageBox(hWnd,L"SDFDS",L"SDFDS",MB_OK);
				break;
			}
			break;
		case IDC_FIND_ID:
			switch(HIWORD(wParam))
			{
			case EN_SETFOCUS:
				SendMessage(finddlg::hWndQuadIDOpt,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(finddlg::hWndStrOpt,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
				//MessageBox(hWnd,"SDFDS","SDFDS",MB_OK);
				break;
			}
			break;
		case IDC_FIND_STR:
			switch(HIWORD(wParam))
			{
			case EN_SETFOCUS:
				SendMessage(finddlg::hWndStrOpt,BM_SETCHECK,(WPARAM)BST_CHECKED,0);
				SendMessage(finddlg::hWndQuadIDOpt,BM_SETCHECK,(WPARAM)BST_UNCHECKED,0);
				//MessageBox(hWnd,"SDFDS","SDFDS",MB_OK);
				break;
			}
			break;
		case IDC_FINDNEXT:
			{
				FindNext();
			}
		}
		break;
		
	default:;

	}
	return 0;
}

void FindNext()
{
	unsigned long curSel;
	if ((curSel = (unsigned long)SendMessage(maindlg::hWndQuadList,	LB_GETCURSEL, 0, 0)) == LB_ERR) return;
	int direction;
	if (SendMessage(finddlg::hWndDownOpt,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		direction = FIND_DOWN;
	}
	else if (SendMessage(finddlg::hWndUpOpt,BM_GETCHECK,0,0) == BST_CHECKED)
	{
		direction = FIND_UP;
	}
	unsigned long retVal = FIND_ERR;
	if (SendMessage(finddlg::hWndQuadIDOpt,BM_GETCHECK,0,0) == BST_CHECKED) //Quad and ID Search
	{
		std::wstring idtxt = GetTextFromHWND(finddlg::hWndIDTxt);
		unsigned long idval = GetValueFromHWND(finddlg::hWndIDTxt);
		char quadstr[5];
		unsigned char len = (unsigned char)SendMessageA(finddlg::hWndQuadTxt, WM_GETTEXT, (WPARAM)5,(LPARAM)quadstr);
		quad quadtype;
		if (len == 4) quadtype = ConvertStringToQuad(quadstr);
		if (len == 4 && idtxt == L"") //Quad Only
		{
			retVal = dmmfile.findFromQuad_Type(quadtype, curSel, direction, GETQUAD_QUADORDERSORT);
		}
		else if (idtxt != L"" && len == 0) //ID Only
		{
			retVal = dmmfile.findFromQuad_ID(idval, curSel, direction, GETQUAD_QUADORDERSORT);
		}
		else if (len == 4 && idtxt != L"") //Both
		{
			SQuadID quadid;
			quadid.type = quadtype;
			quadid.id = idval;
			retVal = dmmfile.findFromQuad_QuadID(quadid, curSel, direction, GETQUAD_QUADORDERSORT);
		}
	}
	else if (SendMessage(finddlg::hWndStrOpt,BM_GETCHECK,0,0) == BST_CHECKED) // String Search
	{
		std::wstring str = GetTextFromHWND(finddlg::hWndStrTxt);
		retVal = dmmfile.findFromQuad_String(str, curSel, direction, GETQUAD_QUADORDERSORT);
	}
	/*std::wstringstream ss;
	ss << retVal;
	MessageBox(finddlg::hWnd, ss.str().c_str(), L"", MB_OK);*/
	if (retVal == FIND_ERR) return;
	SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, retVal, 0);
	ChangeQuad();
	return;
}