/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */
#ifndef _USEFULFUNCS_H
#define _USEFULFUNCS_H

#include <windows.h>
#include <string>
#include <sstream>
#include <math.h>

//Functions
double roundtodecimal(double number, int decimalpoint);

BOOL ResizeWindow(HWND hWnd, short width, short height);
BOOL RePosWindow(HWND hWnd, short x, short y);
BOOL ResizePosWindow(HWND hWnd, short x, short y, short width, short height);
int ShowOpenDlg(HWND hWnd, std::wstring title, std::wstring filter , std::wstring *retfilepath, std::wstring *retfiletitle);
int ShowSaveDlg(HWND hWnd, std::wstring title, std::wstring filter, std::wstring defext, std::wstring *retfilepath, std::wstring *retfiletitle);

std::wstring ConvertNumberToString(short);
std::wstring ConvertNumberToString(long);
std::wstring ConvertNumberToString(unsigned short);
std::wstring ConvertNumberToString(unsigned long);

std::wstring GetTextFromHWND(HWND hWnd);
unsigned long GetValueFromHWND(HWND hWnd);
#endif