#include "plugins.h"

std::vector<SPlugin> plugins;
HWND hWndEditor;
SPlugin *curplugin;

BYTE *uncompressedsectdata;
size_t uncompressedsectlen;
bool datadecomped = false;
void LoadPlugins()
{
	std::vector<std::wstring> pluginfiles;
	 WIN32_FIND_DATA FindFileData;
	 HANDLE hFind;
	 //MessageBox(NULL,((string)appPath + "plugins\\*.dll").c_str(),"AFS",MB_OK);
	 hFind = FindFirstFile(((std::wstring)appPath + L"plugins\\*.dll").c_str(),&FindFileData);
	 if (hFind == INVALID_HANDLE_VALUE) 
	 {
		 MessageBox(NULL,L"Plugin directory does not exist!",L"Error",MB_OK | MB_ICONERROR);
		return;
	 }
	 else 
	 {
		 pluginfiles.push_back(FindFileData.cFileName);
		 while (FindNextFile(hFind,&FindFileData))
		 {
			pluginfiles.push_back(FindFileData.cFileName);
		 }
		//MessageBox(NULL,FindFileData.cFileName,"Error",MB_OK | MB_ICONERROR);
		 FindClose(hFind);
	 }
	 for (std::vector<std::wstring>::iterator Iter = pluginfiles.begin(); Iter != pluginfiles.end(); Iter++)
	 {
		SPlugin temp;
		temp.hLib=LoadLibrary(((std::wstring)appPath + L"plugins\\" + (*Iter)).c_str());

		if(temp.hLib==NULL)
		{
			MessageBox(NULL,((std::wstring)L"Unable to load plugin: " +  (*Iter)).c_str(),L"Error",MB_OK | MB_ICONERROR);
			continue;
		}
		typedef int (__cdecl*getfunc)(quad* quad);
		typedef int (__cdecl*getbutton)(wchar_t button[20]);
		typedef void (__cdecl*initplugin)(LRESULT (*sendpencilmessage) (UINT, LPARAM, WPARAM));
		//typedef int (__cdecl*setdatap)(char *p);
		getbutton GetButtonName;
		getfunc GetQuadTypes;
		initplugin InitPlugin;
		//setdatap SetDataPointer;
		GetButtonName=(getbutton)GetProcAddress((HMODULE)temp.hLib,"GetButtonName");
		//SetDataPointer=(setdatap)GetProcAddress((HMODULE)temp.hLib, "SetDataPointer");
		GetQuadTypes=(getfunc)GetProcAddress((HMODULE)temp.hLib, "GetQuadTypes");
		InitPlugin=(initplugin)GetProcAddress((HMODULE)temp.hLib, "InitPlugin");
		temp.DisplayEditor=(cfunc)GetProcAddress((HMODULE)temp.hLib, "DisplayEditor");
		temp.DisplayAbout=(aboutfunc)GetProcAddress((HMODULE)temp.hLib, "DisplayAbout");
		temp.DisplayEditorEx=(cfuncEx)GetProcAddress((HMODULE)temp.hLib, "DisplayEditorEx");
		temp.CloseEditor=(closefunc)GetProcAddress((HMODULE)temp.hLib, "CloseEditor");
		temp.filetitle = (*Iter);
		if((temp.DisplayEditor == NULL && temp.DisplayEditorEx == NULL) || GetQuadTypes == NULL || GetButtonName == NULL || InitPlugin == NULL || temp.DisplayAbout == NULL || temp.CloseEditor == NULL) 
		{

				MessageBox(NULL,((std::wstring)L"Unable to load fuction(s) in " + (*Iter)).c_str(),L"Error",MB_OK | MB_ICONERROR);
				FreeLibrary((HMODULE)temp.hLib);
				continue;
				
		}

		//char quad[255];
		
			quad quadtype;
			wchar_t button[20]={0};
			//SetDataPointer(quad);
			//SetDataPointer(button);
			InitPlugin(&SendPencilMessage);
			while(GetQuadTypes(&quadtype))
			{
				temp.QuadType.push_back(quadtype);
			}
			GetButtonName(button);
			temp.button = button;
			plugins.push_back(temp);
	   
	 }
	   return;

}

void FreePlugins()
{
	for (std::vector<SPlugin>::iterator Iter = plugins.begin(); Iter != plugins.end(); Iter++)
	{
		FreeLibrary((HMODULE)Iter->hLib);
	}
}

void DisplayEditor()
{
	CloseEditor();
	SQuad quadd;
	if (!dmmfile.getQuad(curquad.uniqueid, &quadd)) return;
	for (std::vector<SPlugin>::iterator itr = plugins.begin(); itr != plugins.end(); itr++)
	{
		for (std::vector<quad>::iterator qitr = itr->QuadType.begin(); qitr != itr->QuadType.end(); qitr++)
		{
			if ((*qitr) == quadd.quadid.type)
			{
				if (quadd.section.isSectionCompressed())
				{
					datadecomped = true;
					uncompressedsectlen = GetSize(quadd.section.getSectionData(), quadd.section.getSectionDataSize());
					uncompressedsectdata = new BYTE[uncompressedsectlen];
					DecompressSmart(quadd.section.getSectionData(), quadd.section.getSectionDataSize(), uncompressedsectdata);
				} 
				else
				{
					datadecomped = false;
					uncompressedsectdata = quadd.section.getSectionData();
					uncompressedsectlen = quadd.section.getSectionDataSize();
				}
				curplugin = &(*itr);
				hWndEditor = itr->DisplayEditor(maindlg::hWndTab, uncompressedsectdata, uncompressedsectlen);
				RePosWindow(hWndEditor,5,23);
				ShowWindow(hWndEditor,SW_SHOW);
			}
		}
	}
	tabeditloaded = true;
	ProcessMainResize(0, 0);

}

void CloseEditor()
{
	if (curplugin == NULL) return;
	curplugin->CloseEditor();
	curplugin = NULL;
	hWndEditor = NULL;
	if (datadecomped)
	{
		delete [] uncompressedsectdata;
		datadecomped = false;
	}
	tabeditloaded = false;
}