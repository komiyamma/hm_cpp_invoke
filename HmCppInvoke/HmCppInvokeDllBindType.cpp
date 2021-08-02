/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"

#include <shlwapi.h>

using namespace std;
using namespace Hidemaru;



HMODULE THm::TDllBindType::hModule = NULL;

wchar_t THm::TDllBindType::wszSelfModuleFullPath[MAX_PATH] = L"";
wchar_t THm::TDllBindType::wszSelfModuleDirPath[MAX_PATH] = L"";

int THm::TDllBindType::iSelfBindedType = 0;

bool THm::rebind() {
	// これ重要。loadllの形態を保持する。関数が呼ばれる度に更新しておく。
	return TDllBindType::setDllBindType();
}


HMODULE THm::TDllBindType::getCurrentModule()
{
	DWORD flags = GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT;
	HMODULE hm = 0;
	::GetModuleHandleEx(flags, reinterpret_cast<LPCTSTR>(THm::TDllBindType::getCurrentModule), &hm);
	return hm;
}



Hidemaru::THm::TDllBindType::TDllBindType()
{
	HMODULE hCurrentModule = getCurrentModule();
	TDllBindType::hModule = hCurrentModule;
	GetModuleFileName(hModule, TDllBindType::wszSelfModuleFullPath, _countof(TDllBindType::wszSelfModuleFullPath));
	wcscpy_s(TDllBindType::wszSelfModuleDirPath, TDllBindType::wszSelfModuleFullPath);
	PathRemoveFileSpec(TDllBindType::wszSelfModuleDirPath);
}


int THm::TDllBindType::getDllBindType() {
	return iSelfBindedType;
}

BOOL THm::TDllBindType::setDllBindType() {

	// 秀丸8.66以上
	if (! Hm.DllFuncCalledType.isMissingMethod() ) {
		int dll = Hm.DllFuncCalledType.getDllBindType(); // 自分のdllの呼ばれ方をチェック
		TDllBindType::iSelfBindedType = dll;
		return TRUE;
	}
	else {
		MessageBox(NULL, L"loadllのパターンが認識出来ませんでした。", L"loadllのパターンが認識出来ませんでした。", MB_ICONERROR);
	}

	return FALSE;
}

wstring THm::TDllBindType::getInvocantString() {
	if (iSelfBindedType == -1) {
		return L"";
	}
	else {
		return to_wstring(iSelfBindedType) + L",";
	}
}

wstring THm::TDllBindType::getSelfModuleFullPath() {
	return wszSelfModuleFullPath;
}

wstring THm::TDllBindType::getSelfModuleDir() {
	return wszSelfModuleDirPath;
}



