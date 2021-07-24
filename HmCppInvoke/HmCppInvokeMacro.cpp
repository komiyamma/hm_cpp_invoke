/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include "HmCppInvoke.h"


using namespace std;
using namespace Hidemaru;



THm::TMacro::PFNEvalMacro THm::TMacro::Hidemaru_EvalMacro = NULL;

THm::TMacro::TMacro()
{
	if (hHideExeHandle) {
		Hidemaru_EvalMacro = (PFNEvalMacro)GetProcAddress(hHideExeHandle, "Hidemaru_EvalMacro");
	}

	this->Exec = TExec();
}

bool THm::TMacro::isExecuting()
{
	if (Hidemaru_GetCurrentWindowHandle) {
		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		const int WM_ISMACROEXECUTING = WM_USER + 167;
		LRESULT r = SendMessageW(hHidemaruWindow, WM_ISMACROEXECUTING, 0, 0);
		return (bool)r;
	}

	return false;
}

THm::TMacro::IResult THm::TMacro::doEval(std::wstring expression)
{
	BOOL success = Hidemaru_EvalMacro(expression.c_str());
	if (success) {
		std::exception e = std::exception();
		THm::TMacro::IResult r = THm::TMacro::IResult(success, e, L"");
		return r;
	}
	else {
		OutputDebugString(L"マクロの実行に失敗しました。\n");
		OutputDebugString(L"マクロ内容:\n");
		OutputDebugString(expression.c_str());
		std::exception e = std::runtime_error("HidemaruMacroEvalException");
		THm::TMacro::IResult r = THm::TMacro::IResult(success, e, L"");
		return r;
	}
}

THmMacroVariable THm::TMacro::getVar(std::wstring varname)
{
	TestDynamicVar = nullptr;
	auto dll_invocant = TSelfDllAttribute::getInvocantString();
	wstring cmd =
		L"##_tmp_dll_id_ret = dllfuncw( " + dll_invocant + L"\"SetDynamicVar\", " + varname + L");\n"
		L"##_tmp_dll_id_ret = 0;\n";
	auto ret = Hm.Macro.doEval(cmd);

	return TestDynamicVar;
}




bool THm::TMacro::setVar(std::wstring varname, THmMacroVariable value)
{
	BOOL success = 0;

	auto dll_invocant = TSelfDllAttribute::getInvocantString();

	wchar_t start = varname[0];
	if (start == L'#') {

		// 数字を数値にトライ。ダメなら0だよ。
		long n = 0;
		try {
			n = std::get<long>(value);

			PushNumVar(n);
			wstring cmd = L" = dllfuncw( " + dll_invocant + L"\"PopNumVar\" );\n";
			cmd = varname + cmd;
			auto ret = Hm.Macro.doEval(cmd);
			success = ret.getResult();
		}
		catch (exception& e) {
			OutputDebugStringA(e.what());
		}
	}
	else if (start == L'$') {
		try {
			wstring s = std::get<wstring>(value);
			PushStrVar(s.data());
			wstring cmd = L" = dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n";
			cmd = varname + cmd;
			auto ret = Hm.Macro.doEval(cmd);
			success = ret.getResult();
		}
		catch (exception& e) {
			OutputDebugStringA(e.what());
		}
	}

	return success;
}

THm::TMacro::IFunctionResult Hidemaru::THm::TMacro::doFunction(std::wstring func_name, THmMacroVariable args0, THmMacroVariable args1, THmMacroVariable args2, THmMacroVariable args3, THmMacroVariable args4, THmMacroVariable args5, THmMacroVariable args6, THmMacroVariable args7, THmMacroVariable args8, THmMacroVariable args9)
{
	std::vector<THmMacroVariable> v;
	IFunctionResult r = IFunctionResult(0, v, nullptr, L"");
	return r;
}

THm::TMacro::IStatementResult Hidemaru::THm::TMacro::doStatement(std::wstring statement_name, THmMacroVariable args0, THmMacroVariable args1, THmMacroVariable args2, THmMacroVariable args3, THmMacroVariable args4, THmMacroVariable args5, THmMacroVariable args6, THmMacroVariable args7, THmMacroVariable args8, THmMacroVariable args9)
{
	std::vector<THmMacroVariable> v;
	IStatementResult r = IStatementResult(0, v, nullptr, L"");
	return r;
}


long THm::TMacro::IResult::getResult()
{
	return this->result;
}

THmMacroResultError THm::TMacro::IResult::getException()
{
	return this->error;
}

std::wstring THm::TMacro::IResult::getMessage()
{
	return this->message;
}

THm::TMacro::IResult::IResult(long result, THmMacroResultError error, std::wstring message)
{
	this->result = result;
	this->error = error;
	this->message = message;
}

THmMacroVariable THm::TMacro::IFunctionResult::getResult()
{
	return this->result;
}

std::vector<THmMacroVariable> THm::TMacro::IFunctionResult::getArgs()
{
	return this->args;
}

THmMacroResultError THm::TMacro::IFunctionResult::getException()
{
	return this->error;
}

std::wstring THm::TMacro::IFunctionResult::getMessage()
{
	return this->message;
}

Hidemaru::THm::TMacro::IFunctionResult::IFunctionResult(THmMacroVariable result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message)
{
	this->result = result;
	this->args = args;
	this->error = error;
	this->message = message;
}

long Hidemaru::THm::TMacro::IStatementResult::getResult()
{
	return this->result;
}

std::vector<THmMacroVariable> Hidemaru::THm::TMacro::IStatementResult::getArgs()
{
	return this->args;
}

THmMacroResultError Hidemaru::THm::TMacro::IStatementResult::getException()
{
	return this->error;
}

std::wstring Hidemaru::THm::TMacro::IStatementResult::getMessage()
{
	return this->message;
}

Hidemaru::THm::TMacro::IStatementResult::IStatementResult(long result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message)
{
	this->result = result;
	this->args = args;
	this->error = error;
	this->message = message;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doEval(std::wstring expression)
{
	if (Hidemaru_GetCurrentWindowHandle) {

		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		const int WM_REMOTE_EXECMACRO_MEMORY = WM_USER + 272;

		wchar_t wszReturn[0xFFFF];
		*(WORD*)wszReturn = _countof(wszReturn); // 最初のバイトにバッファーのサイズを格納することで秀丸本体がバッファーサイズの上限を知る。
		LRESULT lRet = SendMessage(hHidemaruWindow, WM_REMOTE_EXECMACRO_MEMORY, (WPARAM)wszReturn, (LPARAM)expression.c_str());
		if (lRet) {
			wstring wstrreturn = wszReturn;
			std::exception e = std::exception();
			THm::TMacro::IResult r = THm::TMacro::IResult((long)lRet, nullptr, L"");
			return r;
		}
		else {
			OutputDebugString(L"マクロの実行に失敗しました。\n");
			OutputDebugString(L"マクロ内容:\n");
			OutputDebugString(expression.c_str());
			std::exception e = std::runtime_error("HidemaruMacroExecEvalException");
			THm::TMacro::IResult r = THm::TMacro::IResult((long)lRet, nullptr, L"");
			return r;
		}
	}

	std::exception e = std::runtime_error("HidemaruMacroExecEvalException");
	THm::TMacro::IResult r = THm::TMacro::IResult(0, nullptr, L"");
	return r;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doFile(std::wstring filepath)
{
	if (Hidemaru_GetCurrentWindowHandle) {

		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		const int WM_REMOTE_EXECMACRO_FILE = WM_USER + 271;

		wchar_t wszReturn[0xFFFF];
		*(WORD*)wszReturn = _countof(wszReturn); // 最初のバイトにバッファーのサイズを格納することで秀丸本体がバッファーサイズの上限を知る。
		LRESULT lRet = SendMessage(hHidemaruWindow, WM_REMOTE_EXECMACRO_FILE, (WPARAM)wszReturn, (LPARAM)filepath.c_str());
		if (lRet) {
			wstring wstrreturn = wszReturn;
			std::exception e = std::exception();
			THm::TMacro::IResult r = THm::TMacro::IResult((long)lRet, nullptr, L"");
			return r;
		}
		else {
			OutputDebugString(L"マクロの実行に失敗しました。\n");
			OutputDebugString(L"マクロ内容:\n");
			OutputDebugString(filepath.c_str());
			std::exception e = std::runtime_error("HidemaruMacroExecEvalException");
			THm::TMacro::IResult r = THm::TMacro::IResult((long)lRet, nullptr, L"");
			return r;
		}
	}

	std::exception e = std::runtime_error("HidemaruMacroExecEvalException");
	THm::TMacro::IResult r = THm::TMacro::IResult(0, nullptr, L"");
	return r;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doMethod(std::wstring message_parameter, function<long(wstring)> callback_method)
{
	std::exception e = std::exception();
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

