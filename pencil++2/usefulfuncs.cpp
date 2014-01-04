/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "usefulfuncs.h"

double roundtodecimal(double number, int decimalpoint)
{
	number = number * pow((double)10, (double) decimalpoint);
	number = floor(number+0.5);
	number = number / pow((double)10, (double) decimalpoint);
	return number;
}

BOOL ResizeWindow(HWND hWnd, short width, short height)
{
	return SetWindowPos(hWnd,NULL,NULL,NULL,width,height,SWP_NOZORDER|SWP_NOMOVE);
}

BOOL ResizePosWindow(HWND hWnd, short x, short y, short width, short height)
{
	return SetWindowPos(hWnd,NULL,x,y,width,height,SWP_NOZORDER);
}

BOOL RePosWindow(HWND hWnd, short x, short y)
{
	return SetWindowPos(hWnd,NULL,x,y,NULL,NULL,SWP_NOZORDER|SWP_NOSIZE);
}

void ConvertFilter(wchar_t *filter)
{
	size_t strlen = wcslen(filter);
	for (unsigned int strpos = 0; strpos < strlen; strpos++)
	{
		if (filter[strpos] == L'|')
		{
			
			filter[strpos] = 0;
		}
	}
}

int ShowOpenDlg(HWND hWnd, std::wstring title, std::wstring filter , std::wstring *retfilepath, std::wstring *retfiletitle)
{

	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";
	wchar_t szFileTitle[MAX_PATH] = L"";
	size_t fillen = filter.size();
	wchar_t *szFilter = new wchar_t[fillen + 1];
	
	//memcpy(szFilter, filter.c_str(), fillen);
	for (unsigned int x = 0; x < fillen; x++)
	{
		szFilter[x] = (filter.c_str())[x];
	}
	szFilter[fillen] = 0;
	//MessageBox(NULL,szFilter,filter.c_str(),MB_OK);
	
	//memset(szFileName,0,maxsize);
	ConvertFilter(szFilter);
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	//ofn.lpstrInitialDir = szMSKIDSPath;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"";
	ofn.lpstrTitle = title.c_str();
	if(GetOpenFileName(&ofn))
	{
		//memcpy(output, szFileName, maxsize);
		if (retfilepath != NULL)
		{
			*retfilepath = szFileName;
		}
		if (retfiletitle != NULL)
		{
			*retfiletitle = szFileTitle;
		}
		return 1;
	}
	return 0;
}

int ShowSaveDlg(HWND hWnd, std::wstring title, std::wstring filter, std::wstring defext, std::wstring *retfilepath, std::wstring *retfiletitle)
{

	OPENFILENAME ofn;
	wchar_t szFileName[MAX_PATH] = L"";
	wchar_t szFileTitle[MAX_PATH] = L"";
	size_t fillen = filter.size();
	wchar_t *szFilter = new wchar_t[fillen + 1];
	
	//memcpy(szFilter, filter.c_str(), fillen);
	for (unsigned int x = 0; x < fillen; x++)
	{
		szFilter[x] = (filter.c_str())[x];
	}
	szFilter[fillen] = 0;
	//MessageBox(NULL,szFilter,filter.c_str(),MB_OK);
	wcscpy(szFileName, defext.c_str());
	//memset(szFileName,0,maxsize);
	ConvertFilter(szFilter);
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	//ofn.lpstrInitialDir = szMSKIDSPath;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = defext.c_str();
	ofn.lpstrTitle = title.c_str();
	if(GetSaveFileName(&ofn))
	{
		//memcpy(output, szFileName, maxsize);
		if (retfilepath != NULL)
		{
			*retfilepath = szFileName;
		}
		if (retfiletitle != NULL)
		{
			*retfiletitle = szFileTitle;
		}
		return 1;
	}
	return 0;
}

std::wstring ConvertNumberToString(short number)
{
	std::wstringstream ss;
	ss << number;
	return ss.str();
}

std::wstring ConvertNumberToString(long number)
{
	std::wstringstream ss;
	ss << number;
	return ss.str();
}

std::wstring ConvertNumberToString(unsigned short number)
{
	std::wstringstream ss;
	ss << number;
	return ss.str();
}

std::wstring ConvertNumberToString(unsigned long number)
{
	std::wstringstream ss;
	ss << number;
	return ss.str();
}

std::wstring GetTextFromHWND(HWND hWnd)
{
	unsigned short textlen = (unsigned short)SendMessage(hWnd, WM_GETTEXTLENGTH, 0,0);
	wchar_t *buffer = new wchar_t[textlen + 1];
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)textlen+1, (LPARAM)buffer);
	std::wstring retstring = buffer;
	delete buffer;
	return retstring;
}

unsigned long GetValueFromHWND(HWND hWnd)
{
	unsigned short textlen = (unsigned short)SendMessage(hWnd, WM_GETTEXTLENGTH, 0,0);
	wchar_t buffer[11];
	SendMessage(hWnd, WM_GETTEXT, (WPARAM)10+1, (LPARAM)buffer);
	return _wtol(buffer);
}