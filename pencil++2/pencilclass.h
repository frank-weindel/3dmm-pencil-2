/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#ifndef _PENCILCLASS_H
#define _PENCILCLASS_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include "pclasserrors.h"
#include "fileformatstructs.h"
#include <windows.h>

#define PENCIL_GOOD 0
#define PENCIL_ERROR 1

//MagicNumbers
#define MN_1033 0x03030001
#define MN_1055 0x05050001

#define ERROR_LNG 0xFFFFFFFF
#define ERROR_SHRT 0xFFFF
/////////////////////////////////////////////
//////Misc functions/////////////////////////
/////////////////////////////////////////////

int CompareQuads(const char *sQuad1, const char *sQuad2);
int CompareQuads(quad qQuad1, const char *sQuad2);
int ConvertQuadToString(quad qQuad, char sQuad[5]);
int ConvertQuadToString(quad qQuad, wchar_t sQuad[5]);
std::wstring ConvertQuadToString(quad qQuad);
quad ConvertStringToQuad(const wchar_t sQuad[5]);
quad ConvertStringToQuad(const char sQuad[5]);
int ConvertWStrToStr(const wchar_t *wstr, char *str);
int ConvertStrToWStr(const char *str, wchar_t *wstr);
int _GetStringType(const wchar_t *wstr);
size_t FileSize(FILE *fp);
int fgoto(FILE *fp, long offset);
unsigned long fsize(FILE *fp);
quad GetQuadFromHWND(HWND hWnd);
int fixstring(char *string);

struct SQuadID
{
	quad type;
	unsigned long id;
};

struct SQuad;


class CQuadReferences
{
public:
	unsigned short addReference(FF_3dmmIndexReference reference);
	unsigned short addReference(quad type, unsigned long id, unsigned long refid); //returns reference number, returns -1 on error (already the same reference)
	std::vector <FF_3dmmIndexReference> getReferences() { return references; }; 
	bool getReference(unsigned short refnum, FF_3dmmIndexReference *retreference);
	unsigned short getNumReferences() { return (unsigned short)references.size(); };
	bool removeReference(quad type, unsigned long refid);
	bool removeReference(unsigned short refnum);
	void clearReferences() { references.clear(); };
	bool checkReference(FF_3dmmIndexReference reference, FF_3dmmIndexReference ignore);

	unsigned short getNumOccurances(SQuadID quadid);
	unsigned short getNumOccurances(quad type, unsigned long id);
	std::vector<SQuad*> *quads;

	void addCheck(FF_3dmmIndexReference reference);
	std::vector <SQuadID> checkRefs;
private:
	std::vector <FF_3dmmIndexReference> references;
	bool modifyTimesReferenced(quad type, unsigned long id, int value);
};

class CQuadSection
{
public:
	CQuadSection() { sectiondata = NULL; size = 0;};
	void setSectionData(BYTE *_sectiondata, size_t _size);
	BYTE* getSectionData() {return sectiondata;};
	size_t getSectionDataSize() { return size; };
	size_t getUncompressedSectionDataSize();
	bool isSectionCompressed();
private:
	BYTE *sectiondata;
	size_t size;

};

#define STR_NONE 0
#define STR_ASCII 1
#define STR_UNICODE 2

#define STR_FORCE_AUTO		0
#define STR_FORCE_ASCII		1
#define STR_FORCE_UNICODE	2

class CQuadString
{
public:
	CQuadString();
	int setString(std::wstring _str);
	int setString(std::string _str);
	int getStringType() { return strtype; }
	std::wstring getString();

private:
	std::wstring str;
	int strtype;
	int force;
};

struct SRefBy
{
	SQuadID quadid;
	long refid;
	CQuadString string;
	unsigned long uniqueid;
};

struct dmmEditHeader
{
	quad signature;
	short unk1;
	short unk2;
	unsigned long magicnum;
};

struct SProcessData
{
	unsigned long quadoffset;
	size_t quadsize;
	unsigned long sectionoffset;
	size_t sectionsize;
};

struct SQuad
{
	SQuadID quadid;
	unsigned char mode;

	CQuadReferences references;
	CQuadString string;
	CQuadSection section;
	
	SProcessData temp;

	unsigned long uniqueid;

	unsigned short timesreferenced;
};

unsigned char GetQuadMode(SQuad* quad);

#define GETQUAD_SECTIONORDERSORT 0
#define GETQUAD_QUADORDERSORT 1
#define GETQUAD_DIRSORT 2

class CQuadPointerSort
{
public:
	CQuadPointerSort();
	void addPointers(std::vector<SQuad*> *quads);
	void addPointer(SQuad* quad);
	void removePointer(SQuad* quad);
	void insertPointer(SQuad *quad, unsigned long where);
	void setSortType(int _sorttype) { sorttype = _sorttype; };
	void clear() { quadpointers.clear(); };
	SQuad* getQuadPT(unsigned int quadnum);
	unsigned long getItemNumber(SQuad* quad);
	std::vector<SQuad*> quadpointers;
private:
	int sorttype;
};

#define FIND_ERR 0xFFFFFFFF
#define FIND_DOWN	0
#define FIND_UP		1
class CPencil
{
public:
	CPencil();
	~CPencil();
	bool createNew();
	bool open(std::wstring path);
	bool close();
	bool save();
	bool saveAs(std::wstring path, bool copy);

	int getLastError() { return lasterror; };
	std::wstring getLastErrorText();
	std::wstring getErrorText(int errnum);
	dmmEditHeader header;
	unsigned long getNumQuads() { return (unsigned long)quads.size(); };
	unsigned long getQuadNum(unsigned long uniqueid, int quadsort);
	SQuad getQuad(unsigned long quadnum, int quadsort);
	bool getQuad(unsigned long uniqueid, SQuad *retQuad);
	bool getQuad(quad type, unsigned long id, SQuad *retQuad);
	bool getQuad(SQuadID quadid, SQuad *retQuad);
	bool getQuadRefBy(quad type, unsigned long id, std::vector<SRefBy> *retRefBy);
	bool getQuadRefBy(SQuadID quadid, std::vector<SRefBy> *retRefBy);

	void doRefCheck(SQuad *quad, int value);
	unsigned short getTimesReferenced(SQuadID quadid);
	std::wstring getFPath() { return fpath; }

	unsigned long addQuad(SQuad *quad, unsigned long where);
	bool removeQuad(unsigned long uniqueid);
	bool changeQuad(SQuad *quad);
	bool moveQuadUp(unsigned long uniqueid);
	bool moveQuadDown(unsigned long uniqueid);
	
	bool checkQuadNumValidity(unsigned long quadnum);

	unsigned long findFromQuad_Type(quad type, unsigned long quadnumstart, int direction, int quadsort);
	unsigned long findFromQuad_ID(unsigned long id, unsigned long quadnumstart, int direction, int quadsort);
	unsigned long findFromQuad_QuadID(SQuadID quadid, unsigned long quadnumstart, int direction, int quadsort);
	unsigned long findFromQuad_String(std::wstring str, unsigned long quadnumstart, int direction, int quadsort);
private:
	bool fileIsOpen;
	bool scan();
	void raiseError(int errnum);

	FILE *mainfp; //file pointer

	std::wstring fpath; //file path
	
	int lasterror;

	std::vector<SQuad*>* getQuadPointerVector(int quadsort);

	std::vector<SQuad*> quads;
	CQuadPointerSort quadindexsort;
	CQuadPointerSort directorysort;

	unsigned long lastuniqueid;

	SQuad* getQuadPT(SQuadID quadid);
};

//Sorts
class IndexAscendingQuadOffsetSort : public std::binary_function<const SQuad*,const SQuad*,bool> 
{ 
public:
	bool operator()(const SQuad* rpStart, const SQuad* rpEnd) 
	 {
		 return rpStart->temp.quadoffset < rpEnd->temp.quadoffset; 
	 } 
};
class SectionOffsetSort : public std::binary_function<const SQuad*,const SQuad*,bool> 
{ 
public:
     bool operator()(const SQuad* rpStart, const SQuad* rpEnd) 
	 {
		 return rpStart->temp.sectionoffset < rpEnd->temp.sectionoffset; 
	 } 
};
class SectionOffsetSortPT : public std::binary_function<const SQuad*,const SQuad*,bool> 
{ 
public:
     bool operator()(const SQuad* rpStart, const SQuad* rpEnd) 
	 {
		 return rpStart->temp.sectionoffset < rpEnd->temp.sectionoffset; 
	 } 
};
class DirectorySort : public std::binary_function<const SQuad*,const SQuad*,bool> 
{ 
public:
     bool operator()(const SQuad* rpStart, const SQuad* rpEnd) 
	 {
		 if (rpStart->quadid.type == rpEnd->quadid.type)
		 {
			 return rpStart->quadid.id < rpEnd->quadid.id;
		 }
		 else
		 {
			return rpStart->quadid.type < rpEnd->quadid.type;
		 }
	 } 
}; 

//Finds

struct FindQuadID: public std::unary_function<const SQuad*, bool>  
{	
	SQuadID quadid;
	bool operator()(const SQuad* obj) 
	{ 
		return obj->quadid.id == quadid.id && obj->quadid.type == quadid.type; 
	}
};
struct FindID: public std::unary_function<const SQuad*, bool>  
{	
	unsigned long id;
	bool operator()(const SQuad* obj) 
	{ 
		return obj->quadid.id == id; 
	}
};
struct FindQuad: public std::unary_function<const SQuad*, bool>  
{	
	quad type;
	bool operator()(const SQuad* obj) 
	{
		return obj->quadid.type == type; 
	}
};

int wildcmp(wchar_t *wild,wchar_t *string); //Wildcard Comparer

struct FindString: public std::unary_function<SQuad*, bool>  
{	
	std::wstring str;
	bool operator()(SQuad* obj) 
	{
		if (wildcmp((wchar_t*)str.c_str(),(wchar_t*)obj->string.getString().c_str()))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
};


#endif