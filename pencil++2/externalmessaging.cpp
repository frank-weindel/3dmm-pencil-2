#include "externalmessaging.h"

LRESULT SendPencilMessage(UINT message, LPARAM lParam, WPARAM wParam)
{
	switch (message)
	{
	case MSG_GETCURQUADID:
		{
			SExtQuadID *quadid = (SExtQuadID*)lParam;
			quadid->type = curquad.quadid.type;
			quadid->id   = curquad.quadid.id;
			return 0;
		}
	case MSG_GETCURQUADSTRING:
		{
			unsigned int sizeofbuffer = (unsigned int)wParam;
			wchar_t *buffer = (wchar_t*)lParam;
			unsigned short strsize = curquad.string.getString().size();
			if (sizeofbuffer - 1 < strsize)
			{
				wmemcpy(buffer, curquad.string.getString().c_str(), sizeofbuffer - 1);
				buffer[sizeofbuffer] = 0;
				return 1;
			}
			wmemcpy(buffer, curquad.string.getString().c_str(), strsize + 1);
			return 0;
		}
	case MSG_GETCURQUADSTRINGLEN:
		{
			return curquad.string.getString().size();
		}
	case MSG_ALLOCATEBYTEARRAY:
		{
			//BYTE *data = (BYTE*)GlobalAlloc(GMEM_FIXED,(unsigned int) lParam);

			BYTE *data = new BYTE[(unsigned int) lParam];
			return (LRESULT)data;
		}
	case MSG_SAVECURQUADSECTION:
		{
			BYTE* data = (BYTE*)wParam;
			SetCurSection(data, (unsigned int) lParam);
			DisplaySectionInfo();
			ChangeMade();
			return 0;
		}
	}
	return 0;
}