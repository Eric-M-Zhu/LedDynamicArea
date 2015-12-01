#include "LedDynamicArea.h"

#include <windows.h>

#include "MyDataType.h"

static CRITICAL_SECTION g_cs;

int __stdcall Initialize()
{
	int result = RETURN_ERROR_OTHER;

	InitializeCriticalSection(&g_cs);
	CreateFrameArray(PROGRAM_FRAME_SINGLE_COLOR_COUNT, PROGRAM_FRAME_MULI_COLOR_COUNT);

	return result;
}

int __stdcall Uninitialize()
{
	return 0;
}