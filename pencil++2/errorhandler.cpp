/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

void CPencil::raiseError(int errnum)
{
	lasterror = errnum;
}

std::wstring CPencil::getLastErrorText()
{
	return getErrorText(lasterror);
}
std::wstring CPencil::getErrorText(int errnum)
{
	std::wstring errstr;
	switch (errnum)
	{
	case PERROR_NONE:
		errstr = PERRORTXT_NONE;
		break;
	case PERROR_FOPEN:
		errstr = PERRORTXT_FOPEN;
		break;
	case PERROR_NOTVALID:
		errstr = PERRORTXT_NOTVALID;
		break;
	case PERROR_INVALIDQUADNUM:
		errstr = PERROR_INVALIDQUADNUM;
		break;
	case PERROR_FOPENSAVE:
		errstr = PERRORTXT_FOPENSAVE;
		break;
	case PERROR_ALREADYOPENED:
		errstr = PERRORTXT_ALREADYOPENED;
		break;
	case PERROR_NOFPATHEXISTS:
		errstr = PERRORTXT_NOFPATHEXISTS;
		break;
	}
	return errstr;
}