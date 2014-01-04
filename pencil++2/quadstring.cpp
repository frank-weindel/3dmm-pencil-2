/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"


CQuadString::CQuadString()
{
	strtype = STR_NONE;
}

int  CQuadString::setString(std::wstring _str)
{
	strtype = _GetStringType(_str.c_str());
	str = _str;
	return strtype;
}

int  CQuadString::setString(std::string _str)
{
	
	int requiredchars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _str.c_str(), -1, NULL, 0);
	wchar_t *wstr = new wchar_t[requiredchars];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, _str.c_str(), -1, wstr, requiredchars);
	str = wstr;
	strtype = _GetStringType(str.c_str());
	delete [] wstr;
	return strtype;
}

std::wstring CQuadString::getString()
{
	return str;
}