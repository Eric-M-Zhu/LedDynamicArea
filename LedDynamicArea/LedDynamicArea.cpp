#include "LedDynamicArea.h"

#include <windows.h>

static CRITICAL_SECTION g_cs;

int __stdcall Initialize()
{
	int result = RETURN_ERROR_OTHER;

	InitializeCriticalSection(&g_cs);

	return result;
}

int __stdcall Uninitialize()
{
	return 0;
}