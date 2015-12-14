#include "stdafx.h"
#include "CRC32.h"

void GetCRC32(BYTE data, DWORD &crc)
{
	crc = (crc >> 8) ^ CRC32Tabel[data ^ (crc & 0x000000FF)];
}

void GetCRC32Check(const char *pData, DWORD nLength, DWORD &wcrcValue)
{
	DWORD i;
	BYTE nData;

	for (i = 0; i < nLength; ++i)
	{
		nData = BYTE(*pData);
		++pData;
		GetCRC32(nData, wcrcValue);
	}
}