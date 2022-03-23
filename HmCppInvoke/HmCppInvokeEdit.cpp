/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */


#include "HmCppInvoke.h"
#include "HmCppInvokeDllExport.h"

using namespace std;
using namespace Hidemaru;




THm::TEdit::PFNGetTotalTextUnicode THm::TEdit::Hidemaru_GetTotalTextUnicode = NULL;
THm::TEdit::PFNGetSelectedTextUnicode THm::TEdit::Hidemaru_GetSelectedTextUnicode = NULL;
THm::TEdit::PFNGetLineTextUnicode THm::TEdit::Hidemaru_GetLineTextUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicode THm::TEdit::Hidemaru_GetCursorPosUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicodeFromMousePos THm::TEdit::Hidemaru_GetCursorPosUnicodeFromMousePos = NULL;
THm::TEdit::PFNCheckQueueStatus THm::TEdit::Hidemaru_CheckQueueStatus = NULL;



THm::TEdit::TEdit()
{
	if (hHideExeHandle) {
		Hidemaru_GetTotalTextUnicode = (PFNGetTotalTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetTotalTextUnicode");
		Hidemaru_GetSelectedTextUnicode = (PFNGetSelectedTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetSelectedTextUnicode");
		Hidemaru_GetLineTextUnicode = (PFNGetLineTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetLineTextUnicode");
		Hidemaru_GetCursorPosUnicode = (PFNGetCursorPosUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetCursorPosUnicode");
		Hidemaru_GetCursorPosUnicodeFromMousePos = (PFNGetCursorPosUnicodeFromMousePos)GetProcAddress(hHideExeHandle, "Hidemaru_GetCursorPosUnicodeFromMousePos");
		Hidemaru_CheckQueueStatus = (PFNCheckQueueStatus)GetProcAddress(hHideExeHandle, "Hidemaru_CheckQueueStatus");
	}
}

std::wstring THm::TEdit::getFilePath()
{
	if (!Hidemaru_GetCurrentWindowHandle) {
		return L"";
	}

	const int WM_HIDEMARUINFO = WM_USER + 181;
	const int HIDEMARUINFO_GETFILEFULLPATH = 4;

	HWND hWndHidemaru = Hidemaru_GetCurrentWindowHandle();
	if (hWndHidemaru) {
		wchar_t filepath[MAX_PATH * 3] = L"";
		LRESULT cwch = SendMessageW(hWndHidemaru, WM_HIDEMARUINFO, HIDEMARUINFO_GETFILEFULLPATH, (LPARAM)filepath);
		return filepath;
	}

	return L"";
}

int THm::TEdit::getUpdateCount()
{
	if (!Hidemaru_GetCurrentWindowHandle) {
		return -1;
	}
	const int WM_HIDEMARUINFO = WM_USER + 181;
	const int HIDEMARUINFO_GETUPDATECOUNT = 7;

	HWND hWndHidemaru = Hidemaru_GetCurrentWindowHandle();
	if (hWndHidemaru) {
		LRESULT updatecount = SendMessageW(hWndHidemaru, WM_HIDEMARUINFO, HIDEMARUINFO_GETUPDATECOUNT, (LPARAM)0);
		return (int)updatecount;
	}

	return -1;
}


std::wstring THm::TEdit::getTotalText()
{
	if (!Hidemaru_GetTotalTextUnicode) {
		return L"";
	}

	HGLOBAL hGlobal = Hidemaru_GetTotalTextUnicode();
	if (hGlobal) {
		wchar_t* pwsz = (wchar_t*)GlobalLock(hGlobal);
		if (pwsz) {
			wstring text(pwsz); // コピー
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal); // 元のは解放
			return text;
		}
	}
	return L"";
}

bool THm::TEdit::setTotalText(std::wstring text)
{
	BOOL success = 0;

	auto dll_invocant = TDllBindType::getInvocantString();

	PushStrVar(text.data());
	wstring cmd =
		L"begingroupundo;\n"
		L"selectall;\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"endgroupundo;\n";
	if (Hm.Macro.isExecuting()) {
		auto ret = Hm.Macro.doEval(cmd.c_str());
		return (bool)ret.getResult();
	}
	else {
		auto ret = Hm.Macro.Exec.doEval(cmd.c_str());
		return (bool)ret.getResult();
	}

}

std::wstring THm::TEdit::getSelectedText()
{
	if (!Hidemaru_GetSelectedTextUnicode) {
		return L"";
	}

	HGLOBAL hGlobal = Hidemaru_GetSelectedTextUnicode();
	if (hGlobal) {
		wchar_t* pwsz = (wchar_t*)GlobalLock(hGlobal);
		if (pwsz) {
			wstring text(pwsz); // コピー
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal); // 元のは解放
			return text;
		}
	}
	return L"";
}

bool THm::TEdit::setSelectedText(std::wstring text)
{
	BOOL success = 0;

	auto dll_invocant = TDllBindType::getInvocantString();

	PushStrVar(text.data());
	wstring cmd =
		L"if (selecting) {\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"};\n";
	if (Hm.Macro.isExecuting()) {
		auto ret = Hm.Macro.doEval(cmd.c_str());
		return (bool)ret.getResult();
	}
	else {
		auto ret = Hm.Macro.Exec.doEval(cmd.c_str());
		return (bool)ret.getResult();
	}
}

std::wstring THm::TEdit::getLineText()
{
	if (!Hidemaru_GetLineTextUnicode) {
		return L"";
	}

	auto pos = this->getCursorPos();
	int lineno = pos.getLineNo();

	HGLOBAL hGlobal = Hidemaru_GetLineTextUnicode(lineno);
	if (hGlobal) {
		wchar_t* pwsz = (wchar_t*)GlobalLock(hGlobal);
		if (pwsz) {
			wstring text(pwsz); // コピー
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal); // 元のは解放
			return text;
		}
	}

	return L"";
}

bool THm::TEdit::setLineText(std::wstring text)
{
	bool success = false;

	auto dll_invocant = TDllBindType::getInvocantString();

	auto pos = getCursorPos();

	PushStrVar(text.data());
	wstring cmd =
		L"begingroupundo;\n"
		L"selectline;\n"
		L"insert dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n"
		L"moveto2 " + std::to_wstring(pos.getColumn()) + L", " + std::to_wstring(pos.getLineNo()) + L";\n" +
		L"endgroupundo;\n";
	if (Hm.Macro.isExecuting()) {
		auto ret = Hm.Macro.doEval(cmd.c_str());
		(bool)ret.getResult();
	}
	else {
		auto ret = Hm.Macro.Exec.doEval(cmd.c_str());
		(bool)ret.getResult();
	}

	return success;
}

bool THm::TEdit::isQueueStatus()
{
	if (Hidemaru_CheckQueueStatus) {
		return (bool)Hidemaru_CheckQueueStatus();
	}
	return false;
}

THm::TEdit::ICursorPos THm::TEdit::getCursorPos()
{
	int nLineNo = -1;
	int nColumn = -1;

	if (!Hidemaru_GetCursorPosUnicode) {
		ICursorPos pos(nLineNo, nColumn);
		return pos;
	}

	BOOL success = Hidemaru_GetCursorPosUnicode(&nLineNo, &nColumn);
	if (!success) {
		nLineNo = -1;
		nColumn = -1;
	}

	ICursorPos pos(nLineNo, nColumn);
	return pos;
}

THm::TEdit::IMousePos THm::TEdit::getMousePos()
{
	POINT point;
	int s = ::GetCursorPos(&point);
	if (!s) {
		point.x = -1;
		point.y = -1;
	}
	int nLineNo = -1;
	int nColumn = -1;

	// 該当の関数が存在していない時(秀丸 8.73未満)
	if (!Hidemaru_GetCursorPosUnicodeFromMousePos) {
		IMousePos pos(nLineNo, nColumn, point.x, point.y);
		return pos;
	}

	// 該当の関数が存在している時だけ値を更新(秀丸 8.73以上)
	if (Hidemaru_GetCursorPosUnicodeFromMousePos) {
		// このsuccessはnLineNoもしくは、nColumnのどちらか１つが失敗するとFalseを返してしまうので、返り値は使わない
		BOOL _ = Hidemaru_GetCursorPosUnicodeFromMousePos(NULL, &nLineNo, &nColumn);
	}

	IMousePos pos(nLineNo, nColumn, point.x, point.y);
	return pos;
}

THm::TEdit::ICursorPos::ICursorPos(int lineno, int column)
{
	this->lineno = lineno;
	this->column = column;
}

int THm::TEdit::ICursorPos::getLineNo()
{
	return this->lineno;
}

int THm::TEdit::ICursorPos::getColumn()
{
	return this->column;
}


THm::TEdit::IMousePos::IMousePos(int lineno, int column, int x, int y)
{
	this->lineno = lineno;
	this->column = column;
	this->x = x;
	this->y = y;
}




int THm::TEdit::IMousePos::getLineNo()
{
	return this->lineno;
}

int THm::TEdit::IMousePos::getColumn()
{
	return this->column;
}

int THm::TEdit::IMousePos::getX()
{
	return this->x;
}

int THm::TEdit::IMousePos::getY()
{
	return this->y;
}
