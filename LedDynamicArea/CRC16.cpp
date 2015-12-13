#include "stdafx.h"
#include "CRC16.h"

void GetCRC(BYTE data, WORD &crc)
{
	crc = (crc >> 8) ^ CRCTabel[data ^ (crc & 0x000000FF)];
}

void GetCRCCheck(const char *pData, DWORD nLength, WORD &wcrcValue)
{
	DWORD i;
	DWORD nData;

	for (i = 1; i < nLength; ++i)
	{
		nData = BYTE(*pData);
		++pData;
		GetCRC(nData, wcrcValue);
	}
}