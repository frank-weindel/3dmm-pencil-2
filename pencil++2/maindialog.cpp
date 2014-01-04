/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

int currenttab;
bool tabrefloaded;
bool tabeditloaded;
bool tabhexloaded;

TVHITTESTINFO tvht;
TV_INSERTSTRUCT tvinsert;
std::wstringstream ss2;

unsigned long lastcursel;

bool treeviewloaded;

namespace maindlg
{
	HWND hWnd;
	HWND hWndQuadList;
	HWND hWndQuadTreeView;
	HWND hWndMoveUpBtn;
	HWND hWndMoveDownBtn;
	HWND hWndAddQuadBtn;
	HWND hWndRemoveQuadBtn;
	HWND hWndQuadLbl;
	HWND hWndIDLbl;
	HWND hWndStrLbl;
	HWND hWndQuadTxt;
	HWND hWndIDTxt;
	HWND hWndStrTxt;
	HWND hWndTab;
	HWND hWndSectDataBox;
	HWND hWndOffLenLbl;
	HWND hWndImportBtn;
	HWND hWndExportBtn;
	HWND hWndApplyBtn;
	HMENU hMenu;
};

void SetStructureView(bool treeView)
{
	if (treeView)
	{
		//TreeView
		//Hide List Box and Bottom Buttons
		ShowWindow(maindlg::hWndQuadList, SW_HIDE);
		ShowWindow(maindlg::hWndAddQuadBtn, SW_HIDE);
		ShowWindow(maindlg::hWndRemoveQuadBtn, SW_HIDE);
		ShowWindow(maindlg::hWndMoveDownBtn, SW_HIDE);
		ShowWindow(maindlg::hWndMoveUpBtn, SW_HIDE);

		ShowWindow(maindlg::hWndQuadTreeView, SW_SHOW); //Show Tree View
		treeviewloaded = true;
		//Set Tree Function
	}
	else
	{
		//Regular View
		ShowWindow(maindlg::hWndQuadTreeView, SW_HIDE); //Hide Tree View

		//Show List Box and Bottom Buttons
		ShowWindow(maindlg::hWndQuadList, SW_SHOW);
		ShowWindow(maindlg::hWndAddQuadBtn, SW_SHOW);
		ShowWindow(maindlg::hWndRemoveQuadBtn, SW_SHOW);
		ShowWindow(maindlg::hWndMoveDownBtn, SW_SHOW);
		ShowWindow(maindlg::hWndMoveUpBtn, SW_SHOW);
		treeviewloaded = false;
	}
}

void DoTreeView()
{
	int checkstate = CheckMenuItem(maindlg::hMenu, ID_EDIT_TREEVIEW, NULL);
	if (checkstate == MF_CHECKED)
	{
		CheckMenuItem(maindlg::hMenu, ID_EDIT_TREEVIEW, MF_UNCHECKED);
		SetStructureView(false);
	}
	else
	{
		CheckMenuItem(maindlg::hMenu, ID_EDIT_TREEVIEW, MF_CHECKED);
		SetStructureView(true);
		LoadTreeView();
	}
}
void LoadTreeView()
{
	SendMessage(maindlg::hWndQuadTreeView, TVM_SELECTITEM, TVGN_CARET, NULL);
	TreeView_DeleteAllItems(maindlg::hWndQuadTreeView);
	unsigned long numquads = dmmfile.getNumQuads();
	for (unsigned long quadnum = 0; quadnum < numquads; quadnum++)
	{
		SQuad quad = dmmfile.getQuad(quadnum, GETQUAD_QUADORDERSORT);
		if (quad.timesreferenced == 0)
		{
			ss2.str(L"");
			tvinsert.hParent=NULL;			// top most level no need handle
			tvinsert.hInsertAfter=TVI_ROOT; // work as root level
			tvinsert.item.mask=TVIF_TEXT | TVIF_CHILDREN | TVIF_PARAM;
			if (quad.string.getStringType() != STR_NONE)
			ss2 << ConvertQuadToString(quad.quadid.type) << L" - " << quad.quadid.id << L" \"" << quad.string.getString() << L"\"";
			else
			ss2 << ConvertQuadToString(quad.quadid.type) << L" - " << quad.quadid.id;
			wchar_t text[300];
			tvinsert.item.pszText=text;
			wcscpy(tvinsert.item.pszText,ss2.str().c_str());
			tvinsert.item.lParam = quad.uniqueid;
			if (quad.references.getNumReferences() > 0)
			{
				tvinsert.item.cChildren = 1;
			}
			else
			{
				tvinsert.item.cChildren = 0;
			}
			SendMessage(maindlg::hWndQuadTreeView,TVM_INSERTITEM,0,(LPARAM)&tvinsert);
				
			
		}
	}
	

}

void SetTitle(std::wstring str)
{
	SendMessage(maindlg::hWnd, WM_SETTEXT, NULL, (LPARAM)str.c_str());
}


bool ProcessMainResize(short width, short height)
{
	RECT clientrect;
	GetClientRect(maindlg::hWnd, &clientrect);
	short clientwidth = (short)clientrect.right;
	short clientheight = (short)clientrect.bottom;
	//42% Box
	short listwidth = (short)(clientwidth * 0.42);
		if (listwidth > 425) { listwidth = 425; }
	ResizePosWindow(maindlg::hWndQuadList,  10, 10, listwidth, clientheight - 67); //QuadList Box
	ResizePosWindow(maindlg::hWndQuadTreeView, 10, 10, listwidth, clientheight - 20); //Tab Window
	//Button Width = 79x37
	//Buttom Gap Width = 11
	short buttonwidth = (listwidth - (3*11)) / 4;
	ResizePosWindow(maindlg::hWndMoveUpBtn, 8, clientheight - 48, buttonwidth ,37);
	ResizePosWindow(maindlg::hWndAddQuadBtn, 20 + buttonwidth, clientheight - 48, buttonwidth ,37);
	ResizePosWindow(maindlg::hWndRemoveQuadBtn, 31 + (buttonwidth * 2), clientheight - 48, buttonwidth ,37);
	ResizePosWindow(maindlg::hWndMoveDownBtn, 43 + (buttonwidth * 3), clientheight - 48, buttonwidth ,37);

	short afterlistx = listwidth + 19;
	RePosWindow(maindlg::hWndQuadLbl, afterlistx, 14);
	RePosWindow(maindlg::hWndQuadTxt, afterlistx, 29);
	RePosWindow(maindlg::hWndIDLbl, afterlistx, 52);
	RePosWindow(maindlg::hWndIDTxt, afterlistx, 68);
	RePosWindow(maindlg::hWndStrLbl, afterlistx, 91);
	RePosWindow(maindlg::hWndStrTxt, afterlistx, 107);


	ResizePosWindow(maindlg::hWndTab, afterlistx, 137, clientwidth - afterlistx - 10, clientheight - 137 - 87); //Tab Window
	ResizeWindow(refchild::hWnd, clientwidth - afterlistx - 20, clientheight - 137 - 117);
	ResizeWindow(viewhexchild::hWnd, clientwidth - afterlistx - 20, clientheight - 137 - 117);
	if (hWndEditor != NULL && tabeditloaded == true)
	{
		ResizeWindow(hWndEditor, clientwidth - afterlistx - 20, clientheight - 137 - 117);
	}

	ResizePosWindow(maindlg::hWndSectDataBox, afterlistx,clientheight - 77, clientwidth - afterlistx - 82, 67);
	RECT rectsectbox;
	rectsectbox.top = clientheight - 77; rectsectbox.bottom = (rectsectbox.top + 67); rectsectbox.left = afterlistx; rectsectbox.right = rectsectbox.left + (clientwidth - afterlistx - 82);
	ResizePosWindow(maindlg::hWndOffLenLbl, (short)rectsectbox.left + 10,(short)rectsectbox.top + 16, rectsectbox.right - rectsectbox.left - 190,50);
	RePosWindow(maindlg::hWndImportBtn, (short)rectsectbox.right - 176,(short)rectsectbox.top + 13);
	RePosWindow(maindlg::hWndExportBtn, (short)rectsectbox.right - 91,(short)rectsectbox.top + 13);
	RePosWindow(maindlg::hWndApplyBtn, (short)clientwidth - 72,(short)rectsectbox.top);
	InvalidateRect(maindlg::hWndOffLenLbl, NULL, true);
	InvalidateRect(maindlg::hWndImportBtn, NULL, true);
	InvalidateRect(maindlg::hWndExportBtn, NULL, true);
	InvalidateRect(maindlg::hWndApplyBtn, NULL, true);

	return true;
}

LRESULT CALLBACK WndProc(	HWND	hWnd,					// Handle For This Window
				UINT	uMsg,					// Message For This Window
				WPARAM	wParam,					// Additional Message Information
				LPARAM	lParam)					// Additional Message Information
{
	
	//MessageBox(hWnd,"SDFDS","SDFDS",MB_OK);
	switch (uMsg)								// Check For Windows Messages
	{
	default:
		
		return DefDlgProc(hWnd,uMsg,wParam,lParam);
	}
	
	
	return 0;
}

LRESULT CALLBACK MainDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{	
			//Get all Dialog Item Handles
			maindlg::hWnd = hWnd;
			maindlg::hWndQuadList = GetDlgItem(hWnd, IDC_QUAD_LIST);
			maindlg::hWndQuadTreeView = GetDlgItem(hWnd, IDC_TREEVIEW);
			maindlg::hWndMoveUpBtn = GetDlgItem(hWnd, IDC_SECTION_MVUP);
			maindlg::hWndMoveDownBtn = GetDlgItem(hWnd, IDC_SECTION_MVDN);
			maindlg::hWndAddQuadBtn = GetDlgItem(hWnd, IDC_SECTION_ADD);
			maindlg::hWndRemoveQuadBtn = GetDlgItem(hWnd, IDC_SECTION_DELETE);
			maindlg::hWndQuadLbl = GetDlgItem(hWnd, IDC_QUAD_LBL);
			maindlg::hWndIDLbl = GetDlgItem(hWnd, IDC_ID_LBL);
			maindlg::hWndStrLbl = GetDlgItem(hWnd, IDC_STRING_LBL);
			maindlg::hWndQuadTxt = GetDlgItem(hWnd, IDC_SECTIONEDIT_QUADTYPE);
			maindlg::hWndIDTxt = GetDlgItem(hWnd, IDC_SECTIONEDIT_ID);
			maindlg::hWndStrTxt = GetDlgItem(hWnd, IDC_SECTIONEDIT_STRING);
			maindlg::hWndTab = GetDlgItem(hWnd, IDC_TAB1);
			maindlg::hWndSectDataBox = GetDlgItem(hWnd, IDC_SECTBOX);
			maindlg::hWndOffLenLbl = GetDlgItem(hWnd, IDC_INF_SECT_DATA);
			maindlg::hWndImportBtn = GetDlgItem(hWnd, IDC_SECT_IMPORT);
			maindlg::hWndExportBtn = GetDlgItem(hWnd, IDC_SECT_EXPORT);
			maindlg::hWndApplyBtn = GetDlgItem(hWnd, IDC_APPLY_CHANGE);
			maindlg::hMenu = GetMenu(hWnd);

			//Create Tab Items
			HWND hWndTabControl = GetDlgItem(hWnd , IDC_TAB1);
			TCITEM tci1, tci2, tci3;
			tci1.mask = TCIF_TEXT;
			tci1.pszText = L"View Hex";
			tci1.cchTextMax = (int)wcslen(L"View Hex") + 1;
			tci1.iImage = -1;
			tci1.lParam = 0;
			tci2.mask = TCIF_TEXT;
			tci2.pszText = L"View/Edit";
			tci2.cchTextMax = (int)wcslen(L"View/Edit") + 1;
			tci2.iImage = -1;
			tci2.lParam = 0;
			tci3.mask = TCIF_TEXT;
			tci3.pszText = L"References";
			tci3.cchTextMax = (int)wcslen(L"References") + 1;
			tci3.iImage = -1;
			tci3.lParam = 0;
			TabCtrl_InsertItem(hWndTabControl, 0, &  tci1); //Insert Tab 1
			TabCtrl_InsertItem(hWndTabControl, 0, &  tci2); //Insert Tab 2
			TabCtrl_InsertItem(hWndTabControl, 0, &  tci3); //Insert Tab 2
			TabCtrl_SetCurSel(hWndTabControl,0);
			currenttab = TAB_REFERENCES;
			refchild::hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_REFERENCES), maindlg::hWndTab, (DLGPROC)RefDlgProc);
			RePosWindow(refchild::hWnd, 5,23);
			viewhexchild::hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_VIEW_HEX), maindlg::hWndTab, (DLGPROC)ViewHexDlgProc);
			RePosWindow(viewhexchild::hWnd, 5,23);
			ShowWindow(refchild::hWnd, SW_SHOW);
			SetStructureView(false);
			CheckMenuItem(maindlg::hMenu, ID_EDIT_TREEVIEW, MF_UNCHECKED);
			SetTitle(MAINDLG_TITLE_ + std::wstring(L"Untitled"));
			return 1;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_EDIT_FIND1:
			if (finddlg::hWnd == NULL)
			{
				DialogBox(NULL,MAKEINTRESOURCE(IDD_FIND),NULL,(DLGPROC)FindDlgProc);
			}
			else
			{
				BringWindowToTop(finddlg::hWnd);
			}
		break;
		case ID_EDIT_TREEVIEW:
			DoTreeView();
			break;
		case ID_FILE_NEW1:
			{
				HANDLE hThread = GetCurrentThread();
				SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
				dmmfile.close();
				dmmfile.createNew();
				ListQuads();
				SetTitle(MAINDLG_TITLE_ + std::wstring(L"Untitled"));
				SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
				break;
			}
		case ID_FILE_OPEN1:
			{
			HANDLE hThread = GetCurrentThread();
			SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);
			std::wstring fpath;
			std::wstring ftitle;
			if (!ShowOpenDlg(hWnd,L"Open 3DMM File", L"3DMM Files (*.3cn;*.3th;*.chk;*.3mm)|*.3cn;*.3th;*.chk;*.3mm|N3DMM Files (*.nth;*.nmm)|*.nth;*.nmm|",&fpath,&ftitle)) break;  
			dmmfile.close();
			if (dmmfile.open(fpath))
			{
				ListQuads();
				if (treeviewloaded)
				{
					LoadTreeView();
				}
			}
			else { MessageBox(hWnd,  dmmfile.getLastErrorText().c_str(), L"Error", MB_OK | MB_ICONEXCLAMATION); }
			SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			SetTitle(MAINDLG_TITLE_ + ftitle);
			}
			break;
		case ID_FILE_SAVE1:
			{
				if (!dmmfile.save())
				{
					if (dmmfile.getLastError() != PERROR_NOFPATHEXISTS) break;
				} else break;
			}
		case ID_FILE_SAVEAS1:
			{
				std::wstring fpath;
				std::wstring ftitle;
				ShowSaveDlg(hWnd, L"Save 3DMM File", L"3DMM 3CN Data Files (*.3cn)|*.3cn|3DMM 3TH Data Files (*.3th)|*.3th|3DMM CHK Data Files (*.chk)|*.chk|3DMM Movie Files (*.3mm)|*.3mm|N3DMM NTH Data Files (*.nth)|*.nth|N3DMM Movie Files (*.nmm)|*.nmm|",
					L"",&fpath, &ftitle);
				dmmfile.saveAs(fpath, false);
				SetTitle(MAINDLG_TITLE_ + ftitle);
				break;
			}
		case ID_FILE_EXIT:
			{
				dmmfile.close();
				SendMessage(hWnd,WM_CLOSE,0,0);
				break;
			}
		case ID_HEADERINFO:
			DialogBox(hInstance,MAKEINTRESOURCE(IDD_HEADER),hWnd,(DLGPROC)EditHeaderDlgProc);
			break;
		case ID_HELP_ABOUT:
			ShellAbout(maindlg::hWnd,L"3DMM Pencil++ 2 Beta 2", L"Yet another beta and still no real about box...", LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2)));
			break;
		case IDC_QUAD_LIST:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				ChangeQuad();
				break;
			}
			break;
		case IDC_SECTION_ADD:
			AddQuad();
			break;
		case IDC_SECTION_DELETE:
			RemoveQuad();
			break;
		case IDC_SECTION_MVDN:
			MoveQuadDOWN();
			break;
		case IDC_SECTION_MVUP:
			MoveQuadUP();
			break;
		case IDC_APPLY_CHANGE:
			ApplyChanges();
			break;
		case IDC_SECT_EXPORT:
			ExportSection();
			break;
		case IDC_SECT_IMPORT:
			ImportSection();
			break;
		//CHANGES MADE
		case IDC_SECTIONEDIT_STRING: //STRING EDIT BOX
			switch(HIWORD(wParam))
			{
			case EN_CHANGE: //Change
				ChangeMade();
			break;
			}
			break;
		case IDC_SECTIONEDIT_QUADTYPE: //QUAD EDIT BOX
			switch(HIWORD(wParam))
			{
			case EN_CHANGE: //Change
				ChangeMade();
			break;
			}
			break;
		case IDC_SECTIONEDIT_ID: //ID EDIT BOX
			switch(HIWORD(wParam))
			{
			case EN_CHANGE: //Change
				ChangeMade();
			break;
			}
			break;
		}
		break;
	case WM_QUIT:
	case WM_CLOSE:
		//dmmfile.CloseFile();
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		{
			short newWidth = LOWORD(lParam);
			short newHeight = HIWORD(lParam);
			SetWindowLong(hWnd,DWL_MSGRESULT,ProcessMainResize(newWidth, newHeight));
			return true;
		}
	case WM_SIZING:
		{
			RECT *sizerect = (RECT*)lParam;
			short width = short(sizerect->right - sizerect->left);
			short height = short(sizerect->bottom - sizerect->top);
			
			RECT dlgrect;
			GetWindowRect(hWnd, &dlgrect);
			
			if (width < MAINDLG_MIN_WIDTH)
			{
				sizerect->left = dlgrect.left;	
				sizerect->right = sizerect->left + MAINDLG_MIN_WIDTH;
			}
			if	(height < MAINDLG_MIN_HEIGHT)
			{
				sizerect->top = dlgrect.top;
				sizerect->bottom = sizerect->top + MAINDLG_MIN_HEIGHT;
			}
			SetWindowLong(hWnd,DWL_MSGRESULT,true);
			return true;
		}
	case WM_NOTIFY:
		switch(LOWORD(wParam))
		{
		case IDC_TREEVIEW:
			if (((LPNMTREEVIEW)lParam)->hdr.code == TVN_SELCHANGED)
			{
				TVITEMEX item;
				memset(&item,0,sizeof(item));
				item.hItem = ((LPNMTREEVIEW)lParam)->itemNew.hItem;
				item.mask = TVIF_PARAM;
				if (TreeView_GetItem(maindlg::hWndQuadTreeView,&item))
				{
					unsigned long quadnum = dmmfile.getQuadNum(item.lParam, GETQUAD_QUADORDERSORT);
					SendMessage(maindlg::hWndQuadList, LB_SETCURSEL,(WPARAM)quadnum,0);
					ChangeQuad();
				}
			}
			else if(((LPNMTREEVIEW)lParam)->hdr.code == TVN_ITEMEXPANDED && ((LPNMTREEVIEW)lParam)->action == TVE_COLLAPSE) // if code == NM_CLICK - Single click on an item
				{
					HTREEITEM ecchiITEM = ((LPNMTREEVIEW)lParam)->itemNew.hItem;
					HTREEITEM child = TreeView_GetChild(maindlg::hWndQuadTreeView, ecchiITEM);
					while (child != NULL)
					{
						TreeView_DeleteItem(maindlg::hWndQuadTreeView, child);
						child = TreeView_GetChild(maindlg::hWndQuadTreeView, ecchiITEM);
					}

				}
			else if(((LPNMTREEVIEW)lParam)->hdr.code == TVN_ITEMEXPANDING && ((LPNMTREEVIEW)lParam)->action == TVE_EXPAND) // if code == NM_CLICK - Single click on an item
			{
				TVITEMEX item;
				memset(&item,0,sizeof(item));
				item.hItem = ((LPNMTREEVIEW)lParam)->itemNew.hItem;
				item.mask = TVIF_PARAM;

				
				if (TreeView_GetItem(maindlg::hWndQuadTreeView,&item))
				{
					unsigned long uniqueid;
					uniqueid = (unsigned long)item.lParam;
					//char test[255];
					//wsprintf(test,"%i",ff3dmm.dmmIndex[quadNum].refsDisplayed);
					//MessageBox(hWndDlg,test,"tojtss",MB_OK);
					//CHECK IF REFS HAVE BEEN ALREADY DISPLAYED

					//FIND THE QUAD NUM OF ALL REFERENCES AND DISPLAY THEM IN THE LIST
					SQuad quadparent;
					dmmfile.getQuad(uniqueid, &quadparent);
					std::vector<FF_3dmmIndexReference> references = quadparent.references.getReferences();
					for(std::vector <FF_3dmmIndexReference>::iterator refItr = references.begin();refItr != references.end();refItr++)
					{
						SQuad quad;
						if (dmmfile.getQuad(refItr->type, refItr->id, &quad))
						{
							tvinsert.hParent=item.hItem;			// top most level no need handle
							tvinsert.hInsertAfter=TVI_LAST;
							tvinsert.item.mask=TVIF_TEXT | TVIF_CHILDREN | TVIF_PARAM;
							tvinsert.item.lParam = quad.uniqueid;
							if (quad.references.getNumReferences() > 0)
							{
								tvinsert.item.cChildren = 1;
							}
							else
							{
								tvinsert.item.cChildren = 0;
							}
							wchar_t text[300];
							ss2.str(L"");
							if (quad.string.getStringType())
							{
								ss2 << ConvertQuadToString(quad.quadid.type) << L" " << quad.quadid.id << L":" << refItr->refid << L" \"" << quad.string.getString() <<  L"\"";
							}
							else
							{
								ss2 << ConvertQuadToString(quad.quadid.type) << L" " << quad.quadid.id << L":" << refItr->refid;
							}
							

							//MessageBox(hWndDlg,test,"test",MB_OK);
							tvinsert.item.pszText=text;
							wcscpy(tvinsert.item.pszText,ss2.str().c_str());
							SendMessage(maindlg::hWndQuadTreeView,TVM_INSERTITEM,0,(LPARAM)&tvinsert);	
							//goto outFor;
						}

					}
				}
			}
			break;
		case IDC_TAB1:
			HWND hWndTabControl = GetDlgItem(hWnd,IDC_TAB1);
			int cursel = TabCtrl_GetCurSel(hWndTabControl);
			if (((LPNMHDR)lParam)->code == TCN_SELCHANGING)
			{
				if (quadselectmode)
				{
					SetWindowLong(hWnd,DWL_MSGRESULT,true);
					return true;
				}
				switch (cursel)
				{
				case 0:
					ShowWindow(refchild::hWnd,SW_HIDE);
					break;
				case 1:
					ShowWindow(hWndEditor,SW_HIDE);
					break;
				case 2:
					ShowWindow(viewhexchild::hWnd,SW_HIDE);
					break;
				}
				currenttab = TAB_NONE;
				SetWindowLong(hWnd,DWL_MSGRESULT,false);
				return true;
			}
			else if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
			{
				switch (cursel)
				{
				case 0:
					if (!tabrefloaded)
					{
						DisplayReferences(curquad.quadid, &curquad.references);
					}
					ShowWindow(refchild::hWnd,SW_SHOW);
					currenttab = TAB_REFERENCES;
					break;
				case 1:
					if (!tabeditloaded)
					{
						DisplayEditor();
					}
					ShowWindow(hWndEditor,SW_SHOW);
					currenttab = TAB_VIEWEDIT;
					break;
				case 2:
					if (!tabhexloaded)
					{
						SetViewHex();
					}
					ShowWindow(viewhexchild::hWnd,SW_SHOW);
					currenttab = TAB_VIEWHEX;
					break;
				}
			}
		}
	default:;
		//MessageBox(hWnd, L"EWFGEW", L"EWFEW", MB_OK);
	}	
	return 0;
}