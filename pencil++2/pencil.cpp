/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

HINSTANCE hInstance;

CPencil dmmfile;
SQuad curquad;
std::wstring appPath;

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Init Decomp DLL
	char * c  = new char[MAX_PATH];
    

    GetModuleFileNameA(NULL,c,(DWORD)MAX_PATH);
	wchar_t *wc2 = new wchar_t[MAX_PATH];
	GetModuleFileName(NULL,wc2,(DWORD)MAX_PATH);
	//MessageBox(NULL,c,c,MB_OK);
	std::string appPath2;
	appPath2 = c;
	appPath = wc2;
    //cout << appPath.substr(0,s.find_last_of("\\")+1) << endl;
	appPath2 = appPath2.substr(0,appPath2.find_last_of("\\")+1);
	appPath = appPath.substr(0,appPath.find_last_of(L"\\")+1);
	delete[] wc2;
	delete[] c;
	if (!Init(((std::string)(appPath2 + "\\3DMOVIE.EXE")).c_str()))
	{
		MessageBox(NULL,L"3DMOVIE.EXE could not be found in your 3DMM Pencil++ 2 directory. Decompression functions will not work properly.",L"Error",MB_OK | MB_ICONEXCLAMATION);
	}
	///////////////////////////////////////////////////////////////////////////

	LoadPlugins(); //load plugins (DUH!)

	INITCOMMONCONTROLSEX InitCtrlEx;
	InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitCtrlEx.dwICC = ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&InitCtrlEx);

	hInstance = hInst;
	WNDCLASS wc;
	if (!GetClassInfo(hInst,L"#32770",&wc))
	{
		return 0;
	}

	//wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	//wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc Handles Messages
	//wc.cbClsExtra		= 0;						// No Extra Window Data
	wc.cbWndExtra		= DLGWINDOWEXTRA;						// No Extra Window Data

	//wc.hInstance		= hInst;					// Set The Instance
	wc.hIcon		= LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2));			// Load The Default Icon
	//wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	//wc.hbrBackground	= NULL;						// No Background Required For GL
	//wc.lpszMenuName	= NULL;						// We Don't Want A Menu
	wc.lpszClassName	= L"3DMMPencil";					// Set The Class Name
	
	
	if (!RegisterClass(&wc))						// Attempt To Register The Window Class
	{
		MessageBox(NULL,L"Failed To Register The Window Class.",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 1;							// Exit And Return FALSE
	}
	dmmfile.createNew();
	int ret;
	ret = (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL	,(DLGPROC) MainDlgProc);
	//ShowWindow(hwndGoto, nCmdShow);
	//	MSG msg;


	//BOOL bRet;

	//while ( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0 ) 
	//{ 
	//	if (bRet == -1 )
	//	{
	//		// handle the error and possibly exit
	//	}
	//	else if (!IsWindow(hwndGoto) || !IsDialogMessage(hwndGoto, &msg)) 
	//	{ 
	//		TranslateMessage(&msg); 
	//		DispatchMessage(&msg); 
	//	} 
	//} 

   UnregisterClass(L"3DMMPencil",hInst);
   Shutdown();
   FreePlugins();
	return ret;

}