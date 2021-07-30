/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"

#include <shlwapi.h>

using namespace std;
using namespace Hidemaru;

// この結果のバイト列(vector.data())を HmOutputPane.dllのOutput関数になげれば、Unicodeでも再現できる
extern std::vector<BYTE> EncodeWStringToOriginalEncodeVector(std::wstring original_string);


// ファイルマネージャパネル
THm::TExplorerPane::PFNHmExplorerPane_SetMode THm::TExplorerPane::HmExplorerPane_SetMode = NULL;
THm::TExplorerPane::PFNHmExplorerPane_GetMode THm::TExplorerPane::HmExplorerPane_GetMode = NULL;
THm::TExplorerPane::PFNHmExplorerPane_LoadProject THm::TExplorerPane::HmExplorerPane_LoadProject = NULL;
THm::TExplorerPane::PFNHmExplorerPane_SaveProject THm::TExplorerPane::HmExplorerPane_SaveProject = NULL;
THm::TExplorerPane::PFNHmExplorerPane_GetProject THm::TExplorerPane::HmExplorerPane_GetProject = NULL;
THm::TExplorerPane::PFNHmExplorerPane_GetWindowHandle THm::TExplorerPane::HmExplorerPane_GetWindowHandle = NULL;
THm::TExplorerPane::PFNHmExplorerPane_GetUpdated THm::TExplorerPane::HmExplorerPane_GetUpdated = NULL;
THm::TExplorerPane::PFNHmExplorerPane_GetCurrentDir THm::TExplorerPane::HmExplorerPane_GetCurrentDir = NULL;

THm::TExplorerPane::TExplorerPane()
{
	// 少なくともGetWindowsCurrentHandleが無いと、役に立たない
	if (Hidemaru_GetCurrentWindowHandle) {
		// hidemaru.exeのディレクトリを求める
		wchar_t hidemarudir[512] = L"";
		wcscpy_s(hidemarudir, wszHidemaruFullPath);
		PathRemoveFileSpec(hidemarudir);

		// ディレクトリある？ （まぁあるよね）
		if (PathFileExists(hidemarudir)) {
			// HmOutputPane.dllがあるかどうか。
			wstring hmoutputpane_fullpath = wstring(hidemarudir) + wstring(L"\\HmOutputPane.dll");
			hHmExplorerPaneDLL = LoadLibrary(hmoutputpane_fullpath.data());

			// あれば、ExplorerPane関数をセッティングしておく
			if (hHmExplorerPaneDLL) {
				HmExplorerPane_SetMode = (PFNHmExplorerPane_SetMode)GetProcAddress(hHmExplorerPaneDLL, "SetMode");
				HmExplorerPane_GetMode = (PFNHmExplorerPane_GetMode)GetProcAddress(hHmExplorerPaneDLL, "GetMode");
				HmExplorerPane_LoadProject = (PFNHmExplorerPane_LoadProject)GetProcAddress(hHmExplorerPaneDLL, "LoadProject");
				HmExplorerPane_SaveProject = (PFNHmExplorerPane_SaveProject)GetProcAddress(hHmExplorerPaneDLL, "SaveProject");
				HmExplorerPane_GetProject = (PFNHmExplorerPane_GetProject)GetProcAddress(hHmExplorerPaneDLL, "GetProject");
				HmExplorerPane_GetWindowHandle = (PFNHmExplorerPane_GetWindowHandle)GetProcAddress(hHmExplorerPaneDLL, "GetWindowHandle");
				HmExplorerPane_GetUpdated = (PFNHmExplorerPane_GetUpdated)GetProcAddress(hHmExplorerPaneDLL, "GetUpdated");
				if (hm_version > 885) {
					HmExplorerPane_GetCurrentDir = (PFNHmExplorerPane_GetCurrentDir)GetProcAddress(hHmExplorerPaneDLL, "GetCurrentDir");
				}
			}
		}
	}

}

bool THm::TExplorerPane::setMode(int mode)
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_SetMode) {
			bool result = HmExplorerPane_SetMode(hHidemaruWindow, mode);
			return result;
		}
	}

	return false;
}

int THm::TExplorerPane::getMode()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_GetMode) {
			bool result = HmExplorerPane_GetMode(hHidemaruWindow);
			return result;
		}
	}

	return false;
}

bool THm::TExplorerPane::loadProject(std::wstring filepath)
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_LoadProject) {
			auto encode_byte_data = EncodeWStringToOriginalEncodeVector(filepath);
			bool result = HmExplorerPane_LoadProject(hHidemaruWindow, encode_byte_data.data());
			return result;
		}
	}

	return false;
}

bool THm::TExplorerPane::saveProject(std::wstring filepath)
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_SaveProject) {
			auto encode_byte_data = EncodeWStringToOriginalEncodeVector(filepath);
			bool result = HmExplorerPane_SaveProject(hHidemaruWindow, encode_byte_data.data());
			return result;
		}
	}

	return false;
}

std::wstring THm::TExplorerPane::getProject()
{
	if (HmExplorerPane_GetProject) {
		if (Hm.Macro.isExecuting()) {
			auto ret = Hm.Macro.getVar(LR"RAW(dllfuncstr(loaddll("HmExplorerPane"), "GetProject", hidemaruhandle(0)))RAW");
			wstring project_name = std::get<wstring>(ret);
			return project_name;
		}
		else {
			auto ret = Hm.Macro.Exec.doEval(LR"RAW(endmacro dllfuncstr(loaddll("HmExplorerPane"), "GetProject", hidemaruhandle(0));)RAW");
			return ret.getMessage();
		}
	}

	return L"";
}

THmNumber THm::TExplorerPane::sendMessage(int command_id)
{
	HWND ExplorerWindowHandle = this->getWindowHandle();
	if (ExplorerWindowHandle) {
		//251=１つ上のフォルダ
		// 0x111 = WM_COMMAND
		LRESULT r = SendMessageW(ExplorerWindowHandle, 0x111, command_id, 0);
		return (THmNumber)r;

	}
	return FALSE;
}

HWND THm::TExplorerPane::getWindowHandle()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_GetWindowHandle) {
			return HmExplorerPane_GetWindowHandle(hHidemaruWindow);
		}
	}

	return NULL;
}

bool THm::TExplorerPane::getUpdated()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmExplorerPane_GetUpdated) {
			bool result = HmExplorerPane_GetUpdated(hHidemaruWindow);
			return result;
		}
	}

	return false;
}



