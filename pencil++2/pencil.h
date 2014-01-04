/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#ifndef _PENCIL_H
#define _PENCIL_H


#include <windows.h>
#include "resource.h"
#include "pencilclass.h"
#include <string>
#include <sstream>
#include "commctrl.h"
#include "dialogcontrols.h"
#include "globalconstants.h"
#include "usefulfuncs.h"
#include "decompproxy.h"
#include "plugins.h"
#include "editstructs.h"
extern HINSTANCE hInstance;

extern CPencil dmmfile;
extern SQuad curquad;
extern std::wstring appPath;

extern int currenttab;
extern bool tabrefloaded;
extern bool tabeditloaded;
extern bool tabhexloaded;

extern bool treeviewloaded;

extern bool quadselectmode;
extern bool addingref;
extern unsigned long lastcursel;

extern SQuadID addquad;
extern bool curSecDataIsNew;

//applychanges.cpp
extern bool changesMade;



//maindialog.cpp
LRESULT CALLBACK MainDlgProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool ProcessMainResize(short width, short height);
void SetStructureView(bool treeView);
void DoTreeView();
void LoadTreeView();
void SetTitle(std::wstring);

//refdialog.cpp
LRESULT CALLBACK RefDlgProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void SetQuadSelectMode(bool mode);
void SelQuadQSM();

//viewhexdlg.cpp
LRESULT CALLBACK ViewHexDlgProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void SetViewHex();

//headerdlg.cpp
LRESULT CALLBACK EditHeaderDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//quaddisplay.cpp
int ListQuads(); //Lists quads to the Quad List Box
int DisplayQuad(); //Gets current listbox selection and displays quad information in textboxes/etc.
void DisplayReferences(SQuadID quadid, CQuadReferences *references); //Handles reference tab data
void SetCurQuad(); //Sets curquad to the currently select quad
void DisplayReference(); //Displays currently selected reference data
void DisplaySectionInfo(); //Displays Section Length Info
void ChangeQuad(); //Automatically selects either quadselect mode select or setcurquad

//applychanges.cpp
std::wstring GenQuadReferenceItemString(FF_3dmmIndexReference ref, SQuad *quad); //generates a string used in a reference list item
void ChangeReference(); //changes the currently selected reference with the data in the ref box
void ApplyChanges(); //applys quad changes
void ChangeMade(); //Called when a change is made to a quad.. enables Apply Changes button.
void ResetChangeMade(); //disables apply button, sets changes varible to false.
int PromptChangesMade(); //prompts changes made message box if changes are made. returns result.

//modifyfilestructure.cpp
void AddReference();
void RemoveReference();
void AddQuad();
void RemoveQuad();
void MoveQuadUP();
void MoveQuadDOWN();
void ExportSection();
void ImportSection();
void SetCurSection(BYTE* data, size_t size);

//finddlg.cpp
LRESULT CALLBACK FindDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void FindNext();



#endif