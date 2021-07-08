/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include <windows.h>

#include "HmCppInvoke.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		//-------------------------------------------------------------------------
		// 自分自身のdllのハンドルやフルパスを保存しておく
		//-------------------------------------------------------------------------
		Hm.initDllHandle(hModule);

		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
