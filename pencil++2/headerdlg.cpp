/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencil.h"

namespace editheader
{
	HWND hWnd;
	HWND hWndSignature;
	HWND hWndUnk1;
	HWND hWndUnk2;
	HWND hWndStringType;
};

LRESULT CALLBACK EditHeaderDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			editheader::hWnd = hWnd;
			editheader::hWndSignature = GetDlgItem(hWnd,IDC_SIGNATURE);
			editheader::hWndUnk1 = GetDlgItem(hWnd, IDC_UNK1);
			editheader::hWndUnk2 = GetDlgItem(hWnd, IDC_UNK2);
			editheader::hWndStringType = GetDlgItem(hWnd, IDC_STRTYPE);
			SendMessage(editheader::hWndStringType, CB_ADDSTRING, 0, (LPARAM)L"ASCII");
			SendMessage(editheader::hWndStringType, CB_ADDSTRING, 0, (LPARAM)L"Unicode");
			SendMessage(editheader::hWndSignature, EM_SETLIMITTEXT, (WPARAM)4, (LPARAM)0);

			//Load data
			SendMessage(editheader::hWndSignature, WM_SETTEXT, 0, (LPARAM)ConvertQuadToString(dmmfile.header.signature).c_str());
			std::wstringstream ss;
			ss << dmmfile.header.unk1;
			SendMessage(editheader::hWndUnk1, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());
			ss.str(L"");
			ss << dmmfile.header.unk2;
			SendMessage(editheader::hWndUnk2, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());
			switch(dmmfile.header.magicnum)
			{
			case 0x03030001:
					SendMessage(editheader::hWndStringType,CB_SETCURSEL,(WPARAM) 0 ,(LPARAM) 0 );
				break;
			case 0x05050001:
					SendMessage(editheader::hWndStringType,CB_SETCURSEL,(WPARAM) 1 ,(LPARAM) 0 );
				break;
			default:
				dmmfile.header.magicnum = 0x03030001;
				dmmfile.header.magicnum = 0x03030001;
				SendMessage(editheader::hWndStringType,CB_SETCURSEL,(WPARAM) 0 ,(LPARAM) 0 );
			}
			return 1;
		}
		break;
	case WM_QUIT:
	case WM_CLOSE:
		EndDialog(hWnd,0);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				wchar_t buffer[5];
				SendMessage(editheader::hWndSignature, WM_GETTEXT, (WPARAM)4+1, (LPARAM)buffer);
				//Check for bad QUAD
				if  (_GetStringType(buffer) == STR_UNICODE)
				{
					MessageBox(refchild::hWnd, SIGNATURE_UNICODE_USED, L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				if (wcslen(buffer) != 4)
				{
					MessageBox(refchild::hWnd,SIGNATURE_FOUR_CHARACTERS, L"Error", MB_OK | MB_ICONERROR);
					return 0;
				}
				dmmfile.header.signature = ConvertStringToQuad(buffer);
				dmmfile.header.unk1 = (short)GetValueFromHWND(editheader::hWndUnk1);
				dmmfile.header.unk2 = (short)GetValueFromHWND(editheader::hWndUnk2);
				switch (SendMessage(editheader::hWndStringType,CB_GETCURSEL,(WPARAM) 0 ,(LPARAM) 0 ))
				{
				case 0:
					dmmfile.header.magicnum = MN_1033;
					break;
				case 1:
					dmmfile.header.magicnum = MN_1055;
					break;
				}
				EndDialog(hWnd,0);
			}
			break;
		case IDCANCEL:
			EndDialog(hWnd,0);
			break;
		}
	default:;

	}	
	return 0;
}