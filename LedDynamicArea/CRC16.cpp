#include "stdafx.h"
#include "CRC16.h"

void GetCRC(BYTE data, WORD &crc)
{
	crc = (crc >> 8) ^ CRCTabel[data ^ (crc & 0x000000FF)];
}

void GetCRCCheck(const char *pData, DWORD nLength, WORD &wcrcValue)
{
	DWORD i;
	BYTE nData;

	for (i = 0; i < nLength; ++i)
	{
		nData = BYTE(*pData);
		++pData;
		GetCRC(nData, wcrcValue);
	}
}