#include "pencilclass.h"

int wildcmp(wchar_t *wild,wchar_t *string) 
{
	wchar_t *cp, *mp;
	
	while ((*string) && (*wild != L'*')) {
		if ((*wild != *string) && (*wild != L'?')) {
			return 0;
		}
		wild++;
		string++;
	}
		
	while (*string) {
		if (*wild == L'*') {
			if (!*++wild) {
				return 1;
			}
			mp = wild;
			cp = string+1;
		} else if ((*wild == *string) || (*wild == L'?')) {
			wild++;
			string++;
		} else {
			wild = mp;
			string = cp++;
		}
	}
		
	while (*wild == L'*') {
		wild++;
	}
	return !*wild;
}

bool CPencil::checkQuadNumValidity(unsigned long quadnum)
{
	if (quadnum >= quads.size()) return false;
	return true;
}

std::vector<SQuad*>* CPencil::getQuadPointerVector(int quadsort)
{
	switch (quadsort)
	{
	case GETQUAD_SECTIONORDERSORT:
		return &quads;
		break;
	case GETQUAD_QUADORDERSORT:
		return &quadindexsort.quadpointers;
		break;
	case GETQUAD_DIRSORT:
		return &directorysort.quadpointers;
		break;
	}
	return &quads;
}

unsigned long CPencil::findFromQuad_Type(quad type, unsigned long quadnumstart, int direction, int quadsort)
{
	if (!checkQuadNumValidity(quadnumstart)) return FIND_ERR;
	FindQuad find;
	find.type = type;
	std::vector<SQuad*> *searchquads = getQuadPointerVector(quadsort);
	

	if (direction == FIND_DOWN)
	{
		std::vector<SQuad*>::iterator Iter;
		if ((Iter = std::find_if((*searchquads).begin() + quadnumstart + 1,(*searchquads).end(),find)) != (*searchquads).end())
		{
			return unsigned long(Iter - (*searchquads).begin());
		}
	}
	else if (direction == FIND_UP)
	{
		std::vector<SQuad*>::reverse_iterator Iter;
		if ((Iter = std::find_if((*searchquads).rbegin() + ((*searchquads).size()-(quadnumstart)),(*searchquads).rend(),find)) != (*searchquads).rend())
		{
			return unsigned long(-(Iter - (*searchquads).rend())) - 1;
		}
	}
	return FIND_ERR;
}
unsigned long CPencil::findFromQuad_ID(unsigned long id, unsigned long quadnumstart, int direction, int quadsort)
{
	if (!checkQuadNumValidity(quadnumstart)) return FIND_ERR;
	FindID find;
	find.id = id;
	std::vector<SQuad*> *searchquads = getQuadPointerVector(quadsort);
	

	if (direction == FIND_DOWN)
	{
		std::vector<SQuad*>::iterator Iter;
		if ((Iter = std::find_if((*searchquads).begin() + quadnumstart + 1,(*searchquads).end(),find)) != (*searchquads).end())
		{
			return unsigned long(Iter - (*searchquads).begin());
		}
	}
	else if (direction == FIND_UP)
	{
		std::vector<SQuad*>::reverse_iterator Iter;
		if ((Iter = std::find_if((*searchquads).rbegin() + ((*searchquads).size()-(quadnumstart)),(*searchquads).rend(),find)) != (*searchquads).rend())
		{
			return unsigned long(-(Iter - (*searchquads).rend())) - 1;
		}
	}
	return FIND_ERR;
}
unsigned long CPencil::findFromQuad_QuadID(SQuadID quadid, unsigned long quadnumstart, int direction, int quadsort)
{
	if (!checkQuadNumValidity(quadnumstart)) return FIND_ERR;
	FindQuadID find;
	find.quadid = quadid;
	std::vector<SQuad*> *searchquads = getQuadPointerVector(quadsort);
	

	if (direction == FIND_DOWN)
	{
		std::vector<SQuad*>::iterator Iter;
		if ((Iter = std::find_if((*searchquads).begin() + quadnumstart + 1,(*searchquads).end(),find)) != (*searchquads).end())
		{
			return unsigned long(Iter - (*searchquads).begin());
		}
	}
	else if (direction == FIND_UP)
	{
		std::vector<SQuad*>::reverse_iterator Iter;
		if ((Iter = std::find_if((*searchquads).rbegin() + ((*searchquads).size()-(quadnumstart)),(*searchquads).rend(),find)) != (*searchquads).rend())
		{
			return unsigned long(-(Iter - (*searchquads).rend())) - 1;
		}
	}
	return FIND_ERR;
}
unsigned long CPencil::findFromQuad_String(std::wstring str, unsigned long quadnumstart, int direction, int quadsort)
{
if (!checkQuadNumValidity(quadnumstart)) return FIND_ERR;
	FindString find;
	find.str = str;
	std::vector<SQuad*> *searchquads = getQuadPointerVector(quadsort);
	

	if (direction == FIND_DOWN)
	{
		std::vector<SQuad*>::iterator Iter;
		if ((Iter = std::find_if((*searchquads).begin() + quadnumstart + 1,(*searchquads).end(),find)) != (*searchquads).end())
		{
			return unsigned long(Iter - (*searchquads).begin());
		}
	}
	else if (direction == FIND_UP)
	{
		std::vector<SQuad*>::reverse_iterator Iter;
		if ((Iter = std::find_if((*searchquads).rbegin() + ((*searchquads).size()-(quadnumstart)),(*searchquads).rend(),find)) != (*searchquads).rend())
		{
			return unsigned long(-(Iter - (*searchquads).rend())) - 1;
		}
	}
	return FIND_ERR;
}