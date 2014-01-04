#include "pencilclass.h"

void CQuadSection::setSectionData(BYTE *_sectiondata, size_t _size)
{
	sectiondata = _sectiondata; size = _size;
}
bool CQuadSection::isSectionCompressed()
{
	if (size >= 8)
	{
		if (sectiondata[0] == 'K' && sectiondata[1] == 'C' && sectiondata[2] == 'D')
		{
			return true;
		}
	}
	return false;
}

size_t CQuadSection::getUncompressedSectionDataSize()
{
	if (!isSectionCompressed()) return size;
	size_t uncompressedsize = sectiondata[7]+(((int)sectiondata[6])<<8)+(((int)sectiondata[5])<<16) + (((int)sectiondata[4])<<24);
	return uncompressedsize;
}