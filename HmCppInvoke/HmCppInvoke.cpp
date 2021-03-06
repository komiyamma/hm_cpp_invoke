/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"

#include <shlwapi.h>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std;
using namespace Hidemaru;

#ifdef FLOATMACRO_COMPILE
extern "C" __declspec(dllexport) void FLOATMACRO() {}
#endif

namespace Hidemaru {
    THm Hm = THm();
}


double THm::hm_version = 0;
THm::PFNGetCurrentWindowHandle THm::Hidemaru_GetCurrentWindowHandle = NULL;

HMODULE THm::hHideExeHandle = NULL;
wchar_t THm::wszHidemaruFullPath[MAX_PATH] = L"";

HMODULE THm::TOutputPane::hHmOutputPaneDLL = NULL;
HMODULE THm::TExplorerPane::hHmExplorerPaneDLL = NULL;


THm::THm()
{
	// 自分自身のdllの情報は、一番先に設定しておく
	this->DllBindType = TDllBindType();

	GetModuleFileName(NULL, wszHidemaruFullPath, _countof(wszHidemaruFullPath));

	bool success = setVersion();
	if (success)
	{
		if (hm_version >= 873.99) {

			// 秀丸本体に関数があるのは 8.73.99以上
			hHideExeHandle = LoadLibrary(wszHidemaruFullPath);
			if (hHideExeHandle) {
				Hidemaru_GetCurrentWindowHandle = (PFNGetCurrentWindowHandle)GetProcAddress(hHideExeHandle, "Hidemaru_GetCurrentWindowHandle");
			}

			this->DllFuncCalledType = TDllFuncCalledType();
			this->Edit = TEdit();
			this->File = TFile();
			this->Macro = TMacro();
			this->OutputPane = TOutputPane();
			this->ExplorerPane = TExplorerPane();
		}
		else {
			throw std::runtime_error("Hidemaru_MissingVersionException:" + to_string( this->getVersion() ) + "<873.99");
		}
	}
}

double THm::QueryFileVersion(wchar_t* path)
{
	VS_FIXEDFILEINFO* v;
	UINT len;
	DWORD sz = GetFileVersionInfoSize(path, NULL);
	if (sz) {
		unique_ptr<BYTE[]> mngBuf = make_unique<BYTE[]>(sz);
		LPVOID buf = (LPVOID)mngBuf.get();
		GetFileVersionInfo(path, NULL, sz, buf);

		if (VerQueryValue(buf, L"\\", (LPVOID*)&v, &len)) {
			double ret = 0;
			ret = double(HIWORD(v->dwFileVersionMS)) * 100 +
				double(LOWORD(v->dwFileVersionMS)) * 10 +
				double(HIWORD(v->dwFileVersionLS)) +
				double(LOWORD(v->dwFileVersionLS)) * 0.01;
			return ret;
		}
	}

	return 0;
}

double THm::getVersion()
{
	// ★
	return hm_version;
}

HWND THm::getWindowHandle()
{
	return Hidemaru_GetCurrentWindowHandle();
}

wstring THm::getHidemaruFullPath()
{
	return this->wszHidemaruFullPath;
}

bool THm::setVersion()
{
	hm_version = QueryFileVersion(wszHidemaruFullPath);
	if (hm_version > 0.0) {
		return true;
	}
	return false;
}

