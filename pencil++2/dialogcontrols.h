/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include <windows.h>

//////////////////////////////////
// Main Dialog (maindialog.cpp) //
//////////////////////////////////


namespace maindlg
{
	extern HWND hWnd;
	extern HWND hWndQuadList;
	extern HWND hWndQuadTreeView;
	extern HWND hWndMoveUpBtn;
	extern HWND hWndMoveDownBtn;
	extern HWND hWndAddQuadBtn;
	extern HWND hWndRemoveQuadBtn;
	extern HWND hWndQuadLbl;
	extern HWND hWndIDLbl;
	extern HWND hWndStrLbl;
	extern HWND hWndQuadTxt;
	extern HWND hWndIDTxt;
	extern HWND hWndStrTxt;
	extern HWND hWndTab;
	extern HWND hWndSectDataBox;
	extern HWND hWndOffLenLbl;
	extern HWND hWndImportBtn;
	extern HWND hWndExportBtn;
	extern HWND hWndApplyBtn;
	extern HMENU hMenu;
};
namespace refchild
{
	extern HWND hWnd;
	extern HWND hWndRefBox;
	extern HWND hWndRefList;
	extern HWND hWndQuadLbl;
	extern HWND hWndIDLbl;
	extern HWND hWndRefIDLbl;
	extern HWND hWndQuadTxt;
	extern HWND hWndIDTxt;
	extern HWND hWndRefIDTxt;
	extern HWND hWndAddBtn;
	extern HWND hWndRemoveBtn;
	extern HWND hWndChangeBtn;
	extern HWND hWndQSelOKBtn;
	extern HWND hWndRefByBox;
	extern HWND hWndRefByList;
};
namespace viewhexchild
{
	extern HWND hWnd;
	extern HWND hWndHexTxt;
};

namespace editheader
{
	extern HWND hWnd;
	extern HWND hWndSignature;
	extern HWND hWndUnk1;
	extern HWND hWndUnk2;
	extern HWND hWndStringType;
};

namespace finddlg
{
	extern HWND hWnd;
	extern HWND hWndQuadTxt;
	extern HWND hWndIDTxt;
	extern HWND hWndStrTxt;
	extern HWND hWndQuadIDOpt;
	extern HWND hWndStrOpt;
	extern HWND hWndUpOpt;
	extern HWND hWndDownOpt;
	extern HWND hWndFindBtn;
	extern HWND hWndCancelBtn;
};