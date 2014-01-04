/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

unsigned char GetQuadMode(SQuad* quad)
{
	if (CompareQuads(quad->quadid.type, "HTOP")) return 18;
	if (quad->timesreferenced == 0)
	{
		if (quad->section.isSectionCompressed()) return 6;
		else return 2;
	}
	else
	{
		if (quad->section.isSectionCompressed()) return 4;
		else return 0;
	}
	return 0;
}

bool CPencil::moveQuadUp(unsigned long uniqueid)
{

	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
	{
		if ((*itr)->uniqueid == uniqueid)
		{
			unsigned long itemnum = quadindexsort.getItemNumber((*itr));
			if (itemnum == 0)
			{
				return false;
			}
			//std::wstringstream ss;
			//ss << itemnum;
			//MessageBox(NULL, ss.str().c_str(), L"EWFREW", MB_OK);
			quadindexsort.removePointer((*itr));
			quadindexsort.insertPointer((*itr), itemnum - 1);
			return true;
		}
	}
	return false;
}

bool CPencil::moveQuadDown(unsigned long uniqueid)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
	{
		if ((*itr)->uniqueid == uniqueid)
		{
			unsigned long itemnum = quadindexsort.getItemNumber((*itr));
			//std::wstringstream ss;
			//	ss << L"ITEMNUM: " << (unsigned long)itemnum;
			//MessageBox (NULL, ss.str().c_str(), L"EWFREW", MB_OK);
			if (itemnum >= quads.size() - 1)
			{
				
				return false;
			}
			quadindexsort.removePointer((*itr));
			quadindexsort.insertPointer((*itr), itemnum + 1);
			return true;
		}
	}
	return false;
}

unsigned short CPencil::getTimesReferenced(SQuadID quadid)
{
	unsigned short timesreferenced = 0;
	for (std::vector<SQuad*>::iterator iter = quads.begin(); iter != quads.end(); iter++)
	{
		timesreferenced += (*iter)->references.getNumOccurances(quadid);
	}
	return timesreferenced;
}

SQuad* CPencil::getQuadPT(SQuadID quadid)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++) //Find Quad
	{
		if ((*itr)->quadid.type == quadid.type && (*itr)->quadid.id == quadid.id)
		{
			return (*itr);
		}
	}
}

void CPencil::doRefCheck(SQuad *quad, int value)
{
	if (value == 0)
	{
		for (std::vector<SQuadID>::iterator iter = quad->references.checkRefs.begin(); iter != quad->references.checkRefs.end(); iter++)
		{
			unsigned short timesreffed = getTimesReferenced((*iter));
			SQuad* thisquad = getQuadPT((*iter));
			if (thisquad)
			{
				thisquad->timesreferenced = timesreffed;
			}
		}
		quad->references.checkRefs.clear();
	}
	else if (value == 1)
	{
		unsigned short numrefs = quad->references.getNumReferences();
		for (unsigned short refnum = 0; refnum < numrefs; refnum++)
		{
			FF_3dmmIndexReference ref;
			quad->references.getReference(refnum, &ref);
			SQuadID quadid;
			quadid.type = ref.type;
			quadid.id = ref.id;
			unsigned short timesreffed = getTimesReferenced(quadid);
			SQuad* thisquad = getQuadPT(quadid);
			if (thisquad)
			{
				thisquad->timesreferenced = timesreffed;
			}
		}
	}
	
}

unsigned long CPencil::addQuad(SQuad *quad, unsigned long where)
{
	unsigned long newquadnum = (unsigned long)quads.size();
	if (where > newquadnum) return ERROR_LNG; //invalid where
	if (getQuad(quad->quadid,NULL)) return ERROR_LNG; //already exists
	quad->uniqueid = lastuniqueid++;
	quad->references.quads = &quads; //pointer to quads struct
	quad->timesreferenced = getTimesReferenced(quad->quadid);
	doRefCheck(quad,1);
	quads.push_back(quad);
	quadindexsort.insertPointer(quads[newquadnum], where);
	directorysort.addPointer(quads[newquadnum]);
	
	return quad->uniqueid;
}

bool CPencil::removeQuad(unsigned long uniqueid)
{
	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
	{
		if ((*itr)->uniqueid == uniqueid)
		{
			
			quadindexsort.removePointer((*itr));
			directorysort.removePointer((*itr));
			if ((*itr)->section.getSectionDataSize() > 0)
			{
				delete [] (*itr)->section.getSectionData();
			}
			(*itr)->references.clearReferences();
			SQuad *quadptr = (*itr);
			quads.erase (itr);
			doRefCheck(quadptr,1);
			delete quadptr;
			return true;
		}
	}
	return false;
}

bool CPencil::changeQuad(SQuad *quad)
{
	SQuad foundquad;
	if (getQuad(quad->quadid, &foundquad))
	{
		if (foundquad.uniqueid != quad->uniqueid) return false; //if unique ids aren't the same then it's an already used quadid
	}

	for (std::vector<SQuad*>::iterator itr = quads.begin(); itr != quads.end(); itr++)
	{
		if ((*itr)->uniqueid == quad->uniqueid)
		{
			(*itr)->quadid = quad->quadid;
			(*itr)->string = quad->string;
			if ((*itr)->section.getSectionData() != (*quad).section.getSectionData()) delete [] (*itr)->section.getSectionData();
			(*itr)->section = quad->section;
			(*itr)->references = quad->references;
			doRefCheck((*itr), 0);
			return true;
		}
	}
	return false; //unique id not found
}