/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */

#include "pencilclass.h"

bool CQuadReferences::checkReference(FF_3dmmIndexReference reference, FF_3dmmIndexReference ignore)
{
	for (std::vector<FF_3dmmIndexReference>::iterator itr = references.begin(); itr != references.end(); itr++)
	{
		if (reference.type == itr->type && reference.refid == itr->refid)
		{
			if (reference.type == ignore.type && reference.refid == ignore.refid) continue;
			else return false;
		}
	}
	return true;
}
void CQuadReferences::addCheck(FF_3dmmIndexReference reference)
{
	SQuadID entry;
	entry.type = reference.type;
	entry.id = reference.id;
	for (std::vector<SQuadID>::iterator iter = checkRefs.begin(); iter != checkRefs.end(); iter++)
	{
		if ((*iter).type == entry.type && (*iter).id == entry.id) return;
	}
	checkRefs.push_back(entry);
}
bool CQuadReferences::modifyTimesReferenced(quad type, unsigned long id, int value)
{
	for (std::vector<SQuad*>::iterator iter = quads->begin(); iter != quads->end(); iter++)
	{
		if ((*iter)->quadid.type == type && (*iter)->quadid.id == id)
		{
			(*(*iter)).timesreferenced += value;
			return true;
		}
	}
	return false;
}

unsigned short CQuadReferences::addReference(FF_3dmmIndexReference reference)
{
	unsigned short retVal;
	if (references.size() == 0) //If there are no other references in the std::vector then just add it
	{
		references.push_back(reference);
		addCheck(reference);
		return (int)references.size() - 1;
	}
	else
	{
		for (std::vector<FF_3dmmIndexReference>::iterator itr = references.begin(); itr != references.end(); itr++)
		{

			if (reference.refid < itr->refid)
			{
				if ((unsigned short)(itr - references.begin()) == 0)
				{
					retVal = (unsigned short)(itr - references.begin());
					references.insert(itr,reference);
					addCheck(reference);
					return retVal;
				}
				std::vector<FF_3dmmIndexReference>::iterator prevItr = itr - 1;
				if (reference.refid > prevItr->refid)
				{
					retVal = (unsigned short)(itr - references.begin());
					references.insert(itr,reference);
					addCheck(reference);
					return retVal;
				}
				else if (reference.refid == prevItr->refid)
				{

					if (reference.type > prevItr->type) //Type is greater than prev type, we can add it
					{
						retVal = (unsigned short)(itr - references.begin());
						references.insert(itr,reference);
						addCheck(reference);
						return retVal;
					}
					else if (reference.type == prevItr->type) //Type and ref ids are the same?? Can't add it at all..
					{
						return ERROR_SHRT;
					}
				}
			}
			else if (reference.refid == itr->refid)
			{
				if (reference.type < itr->type)
				{
					retVal = (unsigned short)(itr - references.begin());
					references.insert(itr,reference);
					addCheck(reference);
					return retVal;
				}
				else if (reference.type == itr->type)
				{
					return ERROR_SHRT;
				}
			}
			std::vector<FF_3dmmIndexReference>::iterator nextItr = itr + 1;
			if (nextItr == references.end()) //If our next iterator is the end then just add it to the end
			{
				//MessageBox(NULL,L"SDFSD",L"SDFDSF",MB_OK);
				references.push_back(reference);
				addCheck(reference);
				return int(references.size() - 1);
			}
			
		}
	}
	return ERROR_SHRT;
}

unsigned short CQuadReferences::addReference(quad type, unsigned long id, unsigned long refid)
{
	FF_3dmmIndexReference ref;
	ref.type = type;
	ref.id = id;
	ref.refid = refid;
	return addReference(ref);
}

bool CQuadReferences::removeReference(quad type, unsigned long refid)
{
	for (std::vector<FF_3dmmIndexReference>::iterator itr = references.begin(); itr != references.end(); itr++)
	{
		if (itr->type == type && itr->refid == refid)
		{
			addCheck((*itr));
			references.erase(itr);
			return true;
		}
	}
	return false;
}

bool CQuadReferences::removeReference(unsigned short refnum)
{
	if (refnum < 0 || (unsigned)refnum >= references.size()) return false;
	addCheck(references[refnum]);
	references.erase(references.begin() + refnum);
	return true;
}

bool CQuadReferences::getReference(unsigned short refnum, FF_3dmmIndexReference *retreference)
{
	if (refnum < 0 || (unsigned)refnum >= references.size()) return false;
	retreference->type = references[refnum].type;
	retreference->id = references[refnum].id;
	retreference->refid = references[refnum].refid;
	return true;
}

unsigned short CQuadReferences::getNumOccurances(SQuadID quadid)
{
	return getNumOccurances(quadid.type, quadid.id);
}

unsigned short CQuadReferences::getNumOccurances(quad type, unsigned long id)
{
	unsigned short occurances = 0;
	for (std::vector<FF_3dmmIndexReference>::iterator itr = references.begin(); itr != references.end(); itr++)
	{
		if (itr->type == type && itr->id == id) occurances++;
	}
	return occurances;
}