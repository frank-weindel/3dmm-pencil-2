/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#ifndef _FILEFORMATSTRUCTS_H
#define _FILEFORMATSTRUCTS_H

typedef unsigned long quad;
typedef unsigned char BYTE;

struct FF_3dmmHeader
{
	quad identifier;
	quad signature;
	short unk1;
	short unk2;
	unsigned long magicnum;
	unsigned long filelen;
	unsigned long indexoffset;
	unsigned long indexlen;
	unsigned long filelen2;
	BYTE zeros[96];
};



struct FF_3dmmIndexHeader
{
	unsigned long magicnum;
	unsigned long numsections;
	unsigned long quadindexlen;
	unsigned long FFFFFFFF;
	unsigned long unk20; //Always 20
};

struct FF_3dmmIndexQuad
{
	quad type;
	unsigned long id;
	unsigned long secoffset;
	BYTE mode;
	BYTE seclength[3];
	unsigned short numreferences;
	unsigned short timesreferenced;
};
struct FF_3dmmIndexReference
{
	quad type;
	unsigned long id;
	long refid;
};
struct FF_3dmmDirectoryEntry
{
	long offset;
	size_t length;
};

#endif