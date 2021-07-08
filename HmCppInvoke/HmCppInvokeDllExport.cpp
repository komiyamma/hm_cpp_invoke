/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include "HmCppInvoke.h"

using namespace std;
using namespace Hidemaru;

THm::PFNGetDllFuncCalledType THmDllExport::Hidemaru_GetDllFuncCalledType = NULL;

BOOL THm::DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		//-------------------------------------------------------------------------
		// 自分自身のdllのハンドルやフルパスを保存しておく
		//-------------------------------------------------------------------------
		SelfDllInfo::initHandle(hModule);

		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

bool THm::rebindParameters() {
	// これ重要。loadllの形態を保持する。関数が呼ばれる度に更新しておく。
	return SelfDllInfo::setBindDllType();
}
