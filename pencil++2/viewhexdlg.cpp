#include "pencil.h"
#include "hex.h"

hexentry hexdata[256] = HEXCODES;
HFONT hfont;
namespace viewhexchild
{
	HWND hWnd;
	HWND hWndHexTxt;
};

void GetHex(BYTE *bytearray, unsigned long size, std::wstringstream *ss)
{
	
	for (unsigned long pos = 0; pos < size; pos++)
	{
		(*ss) << hexdata[bytearray[pos]] << L" ";
	}
}

void GetString(BYTE *bytearray, unsigned long size, std::wstringstream *ss)
{
	
	for (unsigned long pos = 0; pos < size; pos++)
	{
		if ((bytearray[pos] >= 0 && bytearray[pos] < 32) || bytearray[pos] == 0xA0)
		{
			(*ss) << L".";
		}
		else
		{
			(*ss) << (wchar_t)bytearray[pos];
		}
		
	}
}

void GetFilePos(unsigned long pos, std::wstringstream *ss)
{
	unsigned char byte1 = unsigned char(pos & 0xffffff); //GOOD
	unsigned char byte2 = unsigned char(pos >> 8);
	unsigned char byte3 = unsigned char(pos >> 16);
	unsigned char byte4 = unsigned char(pos >> 24);
	(*ss) << L"0x" << hexdata[byte4] << hexdata[byte3] << hexdata[byte2] << hexdata[byte1] << L": ";
}

void SetViewHex()
{
	bool compressed = curquad.section.isSectionCompressed();
	BYTE* datapointer;
	unsigned long sectsize;
	if (compressed) //data is compressed
	{
		sectsize = (unsigned long)GetSize(curquad.section.getSectionData(), (int)curquad.section.getSectionDataSize());
		datapointer = new BYTE[sectsize]; //create new buffer for decompressed data (delete it later)
		DecompressSmart(curquad.section.getSectionData(), (int)curquad.section.getSectionDataSize(), datapointer); //decompress data
	}
	else //data is not compressed
	{
		datapointer = curquad.section.getSectionData();
		sectsize = (unsigned long)curquad.section.getSectionDataSize();
	}
	const unsigned long bytesPerLine = 16;
	std::wstringstream ss;
	for (unsigned long bytePos = 0; bytePos < sectsize; bytePos += bytesPerLine)
	{
		unsigned long bytesThisLine = sectsize - bytePos;
		unsigned short gapfill;
		if (sectsize - bytePos >= bytesPerLine) { bytesThisLine = bytesPerLine; gapfill = 0; }
		else { bytesThisLine = sectsize - bytePos; gapfill = unsigned short(bytesPerLine - bytesThisLine); }
		GetFilePos(bytePos,&ss);
		GetHex(datapointer + bytePos, bytesThisLine, &ss);
		//Fill Gap
		for (unsigned short gapnum = 0; gapnum < gapfill; gapnum++)
		{
			ss << L"   ";
		}
		GetString(datapointer + bytePos, bytesThisLine, &ss);
		ss << L"\r\n";
	}
	SendMessage(viewhexchild::hWndHexTxt, WM_SETTEXT, 0, (LPARAM)ss.str().c_str());
	tabhexloaded = true;
	if (compressed) delete [] datapointer; //delete datapointer if data was compressed
	
}

LRESULT CALLBACK ViewHexDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			viewhexchild::hWnd = hWnd;
			viewhexchild::hWndHexTxt = GetDlgItem(hWnd,IDC_EDIT1);
			
			LOGFONT lFont;
			
			lFont.lfHeight = 10;
			lFont.lfWidth = 10;
			
			lFont.lfEscapement = 0;
			lFont.lfOrientation = 0;
			lFont.lfWeight = FW_NORMAL;
			lFont.lfItalic = FALSE;
			lFont.lfUnderline = FALSE;
			lFont.lfStrikeOut = FALSE;
			lFont.lfCharSet = DEFAULT_CHARSET;
			lFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			lFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			lFont.lfQuality = DEFAULT_QUALITY;
			lFont.lfPitchAndFamily = DEFAULT_PITCH;
			wcscpy(lFont.lfFaceName, L"FixedSys");//Microsoft Sans Serif
			
			hfont = CreateFontIndirect(&lFont);
			SendMessage(viewhexchild::hWndHexTxt,WM_SETFONT,(WPARAM)hfont,FALSE);
			return 1;
		}
		break;
	case WM_QUIT:
	case WM_CLOSE:
		DeleteObject(hfont);
		break;
	case WM_SIZE:
		{
			RECT clientrect;
			GetClientRect(hWnd, &clientrect);
			short clientwidth = (short)clientrect.right;
			short clientheight = (short)clientrect.bottom;
			ResizePosWindow(viewhexchild::hWndHexTxt, 4, 5, clientwidth - 10, clientheight - 10);
		}
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	default:;

	}	
	return 0;
}

