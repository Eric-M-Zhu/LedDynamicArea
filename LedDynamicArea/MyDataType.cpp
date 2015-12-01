#include "MyDataType.h"

#include <Windows.h>

#include "resource.h"

static DWORD *g_pSingleColorFrameBitmapIDs = NULL;
static DWORD *g_pMultiColorFrameBitmapIDs = NULL;

void CreateFrameArray(int singleColorCount, int multiColorCount)
{
	g_pSingleColorFrameBitmapIDs = new DWORD[singleColorCount];
	for (int i = 0; i < singleColorCount; i++)
	{
		g_pSingleColorFrameBitmapIDs[i] = IDB_SINGLECOLOR_1 + i;
	}

	g_pMultiColorFrameBitmapIDs = new DWORD[multiColorCount];
	for (int i = 0; i < multiColorCount; i++)
	{
		g_pMultiColorFrameBitmapIDs[i] = IDB_MULTICOLOR_1 + i;
	}
}