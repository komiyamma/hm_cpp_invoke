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


// アウトプットパネル
THm::TOutputPane::PFNHmOutputPane_Output THm::TOutputPane::HmOutputPane_Output = NULL;
THm::TOutputPane::PFNHmOutputPane_OutputW THm::TOutputPane::HmOutputPane_OutputW = NULL;
THm::TOutputPane::PFNHmOutputPane_Push THm::TOutputPane::HmOutputPane_Push = NULL;
THm::TOutputPane::PFNHmOutputPane_Pop THm::TOutputPane::HmOutputPane_Pop = NULL;
THm::TOutputPane::PFNHmOutputPane_GetWindowHandle THm::TOutputPane::HmOutputPane_GetWindowHandle = NULL;
THm::TOutputPane::PFNHmOutputPane_SetBaseDir THm::TOutputPane::HmOutputPane_SetBaseDir = NULL;



THm::TOutputPane::TOutputPane()
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
			hHmOutputPaneDLL = LoadLibrary(hmoutputpane_fullpath.data());

			// あれば、Output関数をセッティングしておく
			if (hHmOutputPaneDLL) {
				HmOutputPane_Output = (PFNHmOutputPane_Output)GetProcAddress(hHmOutputPaneDLL, "Output");
				HmOutputPane_Push = (PFNHmOutputPane_Push)GetProcAddress(hHmOutputPaneDLL, "Push");
				HmOutputPane_Pop = (PFNHmOutputPane_Pop)GetProcAddress(hHmOutputPaneDLL, "Pop");
				HmOutputPane_GetWindowHandle = (PFNHmOutputPane_GetWindowHandle)GetProcAddress(hHmOutputPaneDLL, "GetWindowHandle");
				if (hm_version > 877) {
					HmOutputPane_SetBaseDir = (PFNHmOutputPane_SetBaseDir)GetProcAddress(hHmOutputPaneDLL, "SetBaseDir");
				}
				if (hm_version > 898) {
					HmOutputPane_OutputW = (PFNHmOutputPane_OutputW)GetProcAddress(hHmOutputPaneDLL, "OutputW");
				}
			}
		}
	}
}



bool THm::TOutputPane::output(std::wstring message)
{

	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmOutputPane_Output) {
			auto encode_byte_data = EncodeWStringToOriginalEncodeVector(message);
			BOOL result = HmOutputPane_Output(hHidemaruWindow, encode_byte_data.data());
			return result;
		}
	}

	return false;
}

bool THm::TOutputPane::push()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = THm::Hidemaru_GetCurrentWindowHandle();
		if (HmOutputPane_Push) {
			bool result = HmOutputPane_Push(hHidemaruWindow);
			return result;
		}
	}

	return false;
}

bool THm::TOutputPane::pop()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmOutputPane_Pop) {
			bool result = HmOutputPane_Pop(hHidemaruWindow);
			return result;
		}
	}

	return false;
}

bool THm::TOutputPane::clear()
{
	THmNumber ret = this->sendMessage(1009);
	// 今のところ有意な値は返していない
	return (bool)ret;
}

THmNumber THm::TOutputPane::sendMessage(int command_id)
{
	HWND OutputWindowHandle = this->getWindowHandle();
	if (OutputWindowHandle) {
		// (#h,0x111/*WM_COMMAND*/,1009,0);//1009=クリア
		// 0x111 = WM_COMMAND
		LRESULT r = SendMessageW(OutputWindowHandle, 0x111, command_id, 0);
		return (THmNumber)r;

	}
	return FALSE;
}

bool THm::TOutputPane::setBaseDir(std::wstring dirpath)
{
	auto encode_byte_data = EncodeWStringToOriginalEncodeVector(dirpath);

	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		if (HmOutputPane_SetBaseDir) {
			bool result = HmOutputPane_SetBaseDir(hHidemaruWindow, encode_byte_data.data());
			return result;
		}
	}

	return false;
}

HWND THm::TOutputPane::getWindowHandle()
{
	// ちゃんと関数がある時だけ
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow =Hidemaru_GetCurrentWindowHandle();
		if (HmOutputPane_GetWindowHandle) {
			return HmOutputPane_GetWindowHandle(hHidemaruWindow);
		}
	}

	return NULL;
}




extern unsigned int hidemaru_encode_map[];
std::vector<BYTE> EncodeWStringToOriginalEncodeVector(std::wstring original_string)
{

	// この結果のバイト列(vector.data())を HmOutputPane.dllのOutput関数になげれば、Unicodeでも再現できる
	vector<BYTE> r;
	for (wchar_t ch : original_string) {
		int ix = ch;

		// 変換後のコードの数値が、255以下なら
		unsigned int encode_code = hidemaru_encode_map[ix];
		if (encode_code <= 0xFF) { // １文字で収まる
			r.push_back(encode_code);
		}
		else {
			union EncodeCodeUnion {
				unsigned int code;
				BYTE ch[4];
			};

			EncodeCodeUnion ecu;
			ecu.code = encode_code;
			for (BYTE b : ecu.ch) {
				if (b == 0) {
					break;
				}
				r.push_back(b);
			}
		}
	}

	r.push_back(0);
	return r;
}

