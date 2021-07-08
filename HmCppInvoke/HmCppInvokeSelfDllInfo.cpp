/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"

#include <shlwapi.h>

using namespace std;
using namespace Hidemaru;



HMODULE THm::SelfDllInfo::hModule = NULL;

wchar_t THm::SelfDllInfo::szSelfModuleFullPath[MAX_PATH] = L"";
wchar_t THm::SelfDllInfo::szSelfModuleDirPath[MAX_PATH] = L"";

int THm::SelfDllInfo::iSelfBindedType = 0;

void THm::SelfDllInfo::initHandle(HMODULE hModule) {

	SelfDllInfo::hModule = hModule;
	GetModuleFileName(hModule, SelfDllInfo::szSelfModuleFullPath, _countof(SelfDllInfo::szSelfModuleFullPath));
	wcscpy_s(SelfDllInfo::szSelfModuleDirPath, SelfDllInfo::szSelfModuleFullPath);
	PathRemoveFileSpec(SelfDllInfo::szSelfModuleDirPath);
}

int THm::SelfDllInfo::getBindDllType() {
	return iSelfBindedType;
}

BOOL THm::SelfDllInfo::setBindDllType() {

	// 秀丸8.66以上
	if (Hm.Hidemaru_GetDllFuncCalledType) {
		int dll = Hm.Hidemaru_GetDllFuncCalledType(-1); // 自分のdllの呼ばれ方をチェック
		SelfDllInfo::iSelfBindedType = dll;
		return TRUE;
	}
	else {
		MessageBox(NULL, L"loadllのパターンが認識出来ませんでした。", L"loadllのパターンが認識出来ませんでした。", MB_ICONERROR);
	}

	return FALSE;
}

wstring THm::SelfDllInfo::getInvocantString() {
	if (iSelfBindedType == -1) {
		return L"";
	}
	else {
		return to_wstring(iSelfBindedType) + L",";
	}
}

wstring THm::SelfDllInfo::getSelfModuleFullPath() {
	return szSelfModuleFullPath;
}

wstring THm::SelfDllInfo::getSelfModuleDir() {
	return szSelfModuleDirPath;
}



