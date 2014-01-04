/* 3DMM Pencil++ 2 
   Copyright (C) 2005 Frankie Weindel */
#include "pencilclass.h"

CQuadPointerSort::CQuadPointerSort()
{
	sorttype = GETQUAD_SECTIONORDERSORT;
}

unsigned long CQuadPointerSort::getItemNumber(SQuad *quad)
{
	//std::wstringstream ss;
	//ss << L"SORT SIZE: " << (unsigned long)quadpointers.size();
	//MessageBox (NULL, ss.str().c_str(), L"EWFREW", MB_OK);
	for (std::vector<SQuad*>::iterator itr = quadpointers.begin(); itr != quadpointers.end(); itr++)
	{
		if ((*itr) == quad)
		{
			return unsigned long(itr - quadpointers.begin());
		}
	}
}
void CQuadPointerSort::removePointer(SQuad *quad)
{
	
	for (std::vector<SQuad*>::iterator itr = quadpointers.begin(); itr != quadpointers.end(); itr++)
	{
		SQuad *pointer = (*itr);
		if (pointer == quad)
		{
			quadpointers.erase(itr);
			return;
		}
	}

}

void CQuadPointerSort::addPointer(SQuad *quad)
{
	switch (sorttype)
	{
	case GETQUAD_SECTIONORDERSORT:
		{
			if (quadpointers.size() == 0)
			{
				quadpointers.push_back(quad);
				return;
			}
			for (std::vector<SQuad*>::iterator itr = quadpointers.begin(); itr != quadpointers.end(); itr++)
			{
				std::vector<SQuad*>::iterator itrnext = itr + 1;
				if (quad->temp.sectionoffset < (*itr)->temp.sectionoffset)
				{
					quadpointers.insert(itr,quad);
					return;
				}
				if (itrnext == quadpointers.end())
				{
					quadpointers.push_back(quad);
					return;
				}
			}
		}
		break;
	case GETQUAD_QUADORDERSORT:
		{
			if (quadpointers.size() == 0)
			{
				quadpointers.push_back(quad);
				return;
			}
			for (std::vector<SQuad*>::iterator itr = quadpointers.begin(); itr != quadpointers.end(); itr++)
			{
				std::vector<SQuad*>::iterator itrnext = itr + 1;
				if (quad->temp.quadoffset < (*itr)->temp.quadoffset)
				{
					quadpointers.insert(itr,quad);
					return;
				}
				if (itrnext == quadpointers.end())
				{
					quadpointers.push_back(quad);
					return;
				}
			}
		}
		break;
	case GETQUAD_DIRSORT:
		{
			if (quadpointers.size() == 0)
			{
				quadpointers.push_back(quad);
				return;
			}
			for (std::vector<SQuad*>::iterator itr = quadpointers.begin(); itr != quadpointers.end(); itr++)
			{
				std::vector<SQuad*>::iterator itrnext = itr + 1;
				if (quad->quadid.type == (*itr)->quadid.type)
				{
					if (quad->quadid.id < (*itr)->quadid.id)
					{
						quadpointers.insert(itr,quad);
						return;
					}
				}
				else
				{
					if (quad->quadid.type < (*itr)->quadid.type)
					{
						quadpointers.insert(itr,quad);
						return;
					}
				}
				if (itrnext == quadpointers.end())
				{
					quadpointers.push_back(quad);
					return;
				}
			}
		}
	}
}

void CQuadPointerSort::insertPointer(SQuad *quad, unsigned long where)
{
	quadpointers.insert(quadpointers.begin() + where,quad);
}

void CQuadPointerSort::addPointers(std::vector<SQuad*> *quads)
{
	for (std::vector<SQuad*>::iterator itr = quads->begin(); itr != quads->end(); itr++)
	{
		quadpointers.push_back((*itr));
	}
	switch (sorttype)
	{
	case GETQUAD_SECTIONORDERSORT:
		std::sort(quadpointers.begin(), quadpointers.end(),SectionOffsetSortPT());
		break;
	case GETQUAD_QUADORDERSORT:
		std::sort(quadpointers.begin(), quadpointers.end(),IndexAscendingQuadOffsetSort());
		break;
	case GETQUAD_DIRSORT:
		std::sort(quadpointers.begin(), quadpointers.end(),DirectorySort());
		break;
	
	}
}

SQuad* CQuadPointerSort::getQuadPT(unsigned int quadnum)
{
	if (quadnum < quadpointers.size())
	{
		return quadpointers[quadnum];
	}
	else
	{
		 return NULL;
	}
};