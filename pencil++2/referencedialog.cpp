/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

bool quadselectmode = false;
bool addingref = false;

namespace refchild
{
	HWND hWnd;
	HWND hWndRefBox;
	HWND hWndRefList;
	HWND hWndQuadLbl;
	HWND hWndIDLbl;
	HWND hWndRefIDLbl;
	HWND hWndQuadTxt;
	HWND hWndIDTxt;
	HWND hWndRefIDTxt;
	HWND hWndAddBtn;
	HWND hWndRemoveBtn;
	HWND hWndQSelOKBtn;
	HWND hWndChangeBtn;
	HWND hWndRefByBox;
	HWND hWndRefByList;
};

bool ProcessRefResize(short width, short height)
{
	RECT clientrect;
	GetClientRect(refchild::hWnd, &clientrect);
	short clientwidth = (short)clientrect.right;
	short clientheight = (short)clientrect.bottom;
	short boxheight = short(clientheight * 0.65);
	
		//if (listwidth > 425) { listwidth = 425; }
	ResizeWindow(refchild::hWndRefBox, clientwidth - 5, boxheight); //Ref Box
	short reflistheight = boxheight - 60;
	ResizeWindow(refchild::hWndRefList, clientwidth - 50, reflistheight); //Ref List
	short addrembuttonheight = reflistheight / 2;
	ResizePosWindow(refchild::hWndAddBtn,clientwidth - 37, 14,28 , addrembuttonheight);
	ResizePosWindow(refchild::hWndRemoveBtn,clientwidth - 37, 14 + addrembuttonheight + 2, 28 , addrembuttonheight);
	short bottomreflisty = reflistheight + 18;
	//Labels and text boxes
	RePosWindow(refchild::hWndQuadLbl,10,bottomreflisty);
	RePosWindow(refchild::hWndIDLbl,73,bottomreflisty);
	RePosWindow(refchild::hWndRefIDLbl,169,bottomreflisty);

	RePosWindow(refchild::hWndQuadTxt,10,bottomreflisty + 15);
	RePosWindow(refchild::hWndIDTxt,73,bottomreflisty + 15);
	RePosWindow(refchild::hWndRefIDTxt,169,bottomreflisty + 15);

	RePosWindow(refchild::hWndChangeBtn,clientwidth - 72,bottomreflisty + 5);
	RePosWindow(refchild::hWndQSelOKBtn,clientwidth - 140,bottomreflisty + 5);

	ResizePosWindow(refchild::hWndRefByBox, 2, boxheight, clientwidth - 5, clientheight - boxheight - 2);
	ResizePosWindow(refchild::hWndRefByList, 10, boxheight + 15, clientwidth - 20, clientheight - boxheight - 23);
	return true;
}

LRESULT CALLBACK RefDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		refchild::hWnd = hWnd;
		refchild::hWndRefBox = GetDlgItem(hWnd,IDC_REF_BOX);
		refchild::hWndRefList = GetDlgItem(hWnd,IDC_LIST_REF);
		refchild::hWndQuadLbl = GetDlgItem(hWnd,IDC_QUAD_LBL);
		refchild::hWndIDLbl = GetDlgItem(hWnd,IDC_ID_LBL);
		refchild::hWndRefIDLbl = GetDlgItem(hWnd,IDC_REFID_LBL);
		refchild::hWndQuadTxt = GetDlgItem(hWnd,IDC_REFEDIT_QUADTYPE);
		refchild::hWndIDTxt = GetDlgItem(hWnd,IDC_REFEDIT_ID);
		refchild::hWndRefIDTxt = GetDlgItem(hWnd,IDC_REFEDIT_REFID);
		refchild::hWndAddBtn = GetDlgItem(hWnd,IDC_REFEDIT_ADD);
		refchild::hWndRemoveBtn = GetDlgItem(hWnd,IDC_REFEDIT_DELETE);
		refchild::hWndChangeBtn = GetDlgItem(hWnd,IDC_REFEDIT_CHANGE);
		refchild::hWndRefByBox = GetDlgItem(hWnd,IDC_REFBY_BOX);
		refchild::hWndRefByList = GetDlgItem(hWnd,IDC_LIST_REFFEDBY);
		refchild::hWndQSelOKBtn = GetDlgItem(hWnd, IDC_REFEDIT_OK);
		return 1;
		break;
	case WM_QUIT:
	case WM_CLOSE:
	case WM_SIZE:
		SetWindowLong(hWnd,DWL_MSGRESULT,ProcessRefResize(LOWORD(lParam), HIWORD(lParam)));
		//MessageBox(hWnd, L"LOL", L"EFEW",MB_OK);
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_CTLCOLORSTATIC:
		SetBkMode((HDC)wParam, TRANSPARENT);
		return (LRESULT)(HBRUSH)GetStockObject(NULL_BRUSH);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LIST_REF:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				DisplayReference();
				break;
			case LBN_DBLCLK:
				if (!quadselectmode)
				{
					unsigned long cursel = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETCURSEL, 0,0);
					if (cursel != LB_ERR)
					{
						unsigned long uniqueid = (unsigned long)SendMessage(refchild::hWndRefList, LB_GETITEMDATA, cursel, 0);
						unsigned long quadnum = dmmfile.getQuadNum(uniqueid, GETQUAD_QUADORDERSORT);
						SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)quadnum, 0);
						SetCurQuad();
						DisplayQuad();
					}
				}
				break;
			}
			break;
		case IDC_LIST_REFFEDBY:
			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK:
					unsigned long cursel = (unsigned long)SendMessage(refchild::hWndRefByList, LB_GETCURSEL, 0,0);
					if (cursel != LB_ERR)
					{
						unsigned long uniqueid = (unsigned long)SendMessage(refchild::hWndRefByList, LB_GETITEMDATA, cursel, 0);
						unsigned long quadnum = dmmfile.getQuadNum(uniqueid, GETQUAD_QUADORDERSORT);
						SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, (WPARAM)quadnum, 0);
						SetCurQuad();
						DisplayQuad();
					}
				break;
			}
			break;
		case IDC_REFEDIT_CHANGE:
			if (quadselectmode)
			{
				SetQuadSelectMode(false);
				addingref = false;
				DisplayReference();
				EnableWindow(refchild::hWndRefList, true);
			}
			else
			{
				ChangeReference();
			}
			break;
		case IDC_REFEDIT_ADD:
			EnableWindow(refchild::hWndRefList, false);
			SetQuadSelectMode(true);
			addingref = true;
			break;
		case IDC_REFEDIT_DELETE:
			RemoveReference();
			break;
		case IDC_REFEDIT_OK:
			if (quadselectmode)
			{
				if (addingref)
				{
					AddReference();
				}
				else
				{
					SetQuadSelectMode(false);
				}
			}
			else SetQuadSelectMode(true);
			break;
		}
	default:;
		//MessageBox(hWnd, L"EWFGEW", L"EWFEW", MB_OK);
	}	
	return 0;
}

void SetQuadSelectMode(bool mode)
{
	switch (mode)
	{
	case true:
		EnableWindow(maindlg::hWndAddQuadBtn, false);
		EnableWindow(maindlg::hWndRemoveQuadBtn, false);
		EnableWindow(maindlg::hWndMoveDownBtn, false);
		EnableWindow(maindlg::hWndMoveUpBtn, false);
		EnableWindow(maindlg::hWndQuadTxt, false);
		EnableWindow(maindlg::hWndIDTxt, false);
		EnableWindow(maindlg::hWndStrTxt, false);

		EnableWindow(maindlg::hWndQuadLbl, false);
		EnableWindow(maindlg::hWndIDLbl, false);
		EnableWindow(maindlg::hWndStrLbl, false);
		EnableWindow(maindlg::hWndOffLenLbl, false);

		EnableWindow(maindlg::hWndExportBtn, false);
		EnableWindow(maindlg::hWndImportBtn, false);
		EnableWindow(maindlg::hWndApplyBtn, false);
		EnableWindow(refchild::hWndAddBtn, false);
		EnableWindow(refchild::hWndRemoveBtn, false);
		EnableWindow(refchild::hWndRefByList, false);
		
		SendMessage(refchild::hWndQSelOKBtn, WM_SETTEXT, 0, (LPARAM)L"OK");
		SendMessage(refchild::hWndChangeBtn, WM_SETTEXT, 0, (LPARAM)L"Cancel");
		break;
	case false:
		EnableWindow(maindlg::hWndAddQuadBtn, true);
		EnableWindow(maindlg::hWndRemoveQuadBtn, true);
		EnableWindow(maindlg::hWndMoveDownBtn, true);
		EnableWindow(maindlg::hWndMoveUpBtn, true);
		EnableWindow(maindlg::hWndQuadTxt, true);
		EnableWindow(maindlg::hWndIDTxt, true);
		EnableWindow(maindlg::hWndStrTxt, true);

		EnableWindow(maindlg::hWndQuadLbl, true);
		EnableWindow(maindlg::hWndIDLbl, true);
		EnableWindow(maindlg::hWndStrLbl, true);
		EnableWindow(maindlg::hWndOffLenLbl, true);

		EnableWindow(maindlg::hWndExportBtn, true);
		EnableWindow(maindlg::hWndImportBtn, true);
		EnableWindow(maindlg::hWndApplyBtn, true);
		EnableWindow(refchild::hWndAddBtn, true);
		EnableWindow(refchild::hWndRemoveBtn, true);
		EnableWindow(refchild::hWndRefByList, true);

		SendMessage(refchild::hWndQSelOKBtn, WM_SETTEXT, 0, (LPARAM)L"Quad Sel");
		SendMessage(refchild::hWndChangeBtn, WM_SETTEXT, 0, (LPARAM)L"Change");
		SendMessage(maindlg::hWndQuadList, LB_SETCURSEL, lastcursel, 0);
		break;
	}
	quadselectmode = mode;	
}
void SelQuadQSM()
{
	unsigned long cursel = (unsigned long)SendMessage(maindlg::hWndQuadList,	LB_GETCURSEL, 0, 0);
	if (cursel == LB_ERR) return;
	SQuad quad = dmmfile.getQuad(cursel,GETQUAD_QUADORDERSORT);
	SendMessage(refchild::hWndQuadTxt, WM_SETTEXT, 0, (LPARAM)ConvertQuadToString(quad.quadid.type).c_str());
	SendMessage(refchild::hWndIDTxt, WM_SETTEXT, 0, (LPARAM)ConvertNumberToString(quad.quadid.id).c_str());
}