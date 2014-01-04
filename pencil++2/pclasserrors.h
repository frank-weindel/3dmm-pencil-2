/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

//Errors
#define PERROR_NONE					0
#define PERROR_FOPEN				1
#define PERROR_NOTVALID				2
#define PERROR_INVALIDQUADNUM		3
#define PERROR_FOPENSAVE			4
#define PERROR_ALREADYOPENED		5
#define PERROR_NOFPATHEXISTS		6

//Error text
#define PERRORTXT_NONE				L"No Error"
#define PERRORTXT_FOPEN				L"Error opening the specifed file"
#define PERRORTXT_NOTVALID			L"File not a valid 3DMM File"
#define PERRORTXT_INVALIDQUADNUM	L"Quadnum specified not valid"
#define PERRORTXT_FOPENSAVE			L"Error opening the specifed file for saving"
#define PERRORTXT_ALREADYOPENED		L"Pencil class is already initialized. Close it and try again."
#define PERRORTXT_NOFPATHEXISTS		L"No file path exists in pencil class. Use saveAs()."