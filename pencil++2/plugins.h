#ifndef _PLUGINS_H
#define _PLUGINS_H

#include "pencil.h"
#include "externalmessaging.h"

///////////////////////////
//PLUGINS//////////////////
///////////////////////////


typedef void (__cdecl*closefunc)();
typedef HWND (__cdecl*cfunc)(HWND hWnd,BYTE *sectiondata, size_t sectionlen);
typedef int (__cdecl*aboutfunc)(HWND hWnd);
typedef int (__cdecl*cfuncEx)(HWND hWnd,BYTE *sectiondata, char quadtype[4], unsigned long id, const char *str);


struct SPlugin
{
	HINSTANCE hLib;
	cfunc DisplayEditor;
	cfuncEx DisplayEditorEx;
	aboutfunc DisplayAbout;
	closefunc CloseEditor;
	std::vector<quad> QuadType;
	std::wstring button;
	std::wstring filetitle;
};

extern std::vector<SPlugin> plugins;
extern HWND hWndEditor;

void LoadPlugins();
void FreePlugins();
void DisplayEditor();
void CloseEditor();

#endif