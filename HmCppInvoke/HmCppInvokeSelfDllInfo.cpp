/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"

#include <shlwapi.h>

using namespace std;
using namespace Hidemaru;



HMODULE THm::TSelfDllInfo::hModule = NULL;

wchar_t THm::TSelfDllInfo::szSelfModuleFullPath[MAX_PATH] = L"";
wchar_t THm::TSelfDllInfo::szSelfModuleDirPath[MAX_PATH] = L"";

int THm::TSelfDllInfo::iSelfBindedType = 0;


HMODULE THm::TSelfDllInfo::getCurrentModule()
{
	DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
	HMODULE hm = 0;
	::GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(THm::TSelfDllInfo::getCurrentModule), &hm);
	return hm;
}



Hidemaru::THm::TSelfDllInfo::TSelfDllInfo()
{
	this->initHandle();
}

void THm::TSelfDllInfo::initHandle() {
	HMODULE hCurrentModule = getCurrentModule();
	TSelfDllInfo::hModule = hCurrentModule;
	GetModuleFileName(hModule, TSelfDllInfo::szSelfModuleFullPath, _countof(TSelfDllInfo::szSelfModuleFullPath));
	wcscpy_s(TSelfDllInfo::szSelfModuleDirPath, TSelfDllInfo::szSelfModuleFullPath);
	PathRemoveFileSpec(TSelfDllInfo::szSelfModuleDirPath);
}

int THm::TSelfDllInfo::getBindDllType() {
	return iSelfBindedType;
}

BOOL THm::TSelfDllInfo::setBindDllType() {

	// 秀丸8.66以上
	if (Hm.Hidemaru_GetDllFuncCalledType) {
		int dll = Hm.Hidemaru_GetDllFuncCalledType(-1); // 自分のdllの呼ばれ方をチェック
		TSelfDllInfo::iSelfBindedType = dll;
		return TRUE;
	}
	else {
		MessageBox(NULL, L"loadllのパターンが認識出来ませんでした。", L"loadllのパターンが認識出来ませんでした。", MB_ICONERROR);
	}

	return FALSE;
}

wstring THm::TSelfDllInfo::getInvocantString() {
	if (iSelfBindedType == -1) {
		return L"";
	}
	else {
		return to_wstring(iSelfBindedType) + L",";
	}
}

wstring THm::TSelfDllInfo::getSelfModuleFullPath() {
	return szSelfModuleFullPath;
}

wstring THm::TSelfDllInfo::getSelfModuleDir() {
	return szSelfModuleDirPath;
}



