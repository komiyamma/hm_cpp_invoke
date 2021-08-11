/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include "HmCppInvoke.h"
#include "HmCppInvokeDllExport.h"


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
		std::exception e = std::runtime_error("Hidemaru_MacroEvalException");
		THm::TMacro::IResult r = THm::TMacro::IResult(success, e, L"");
		return r;
	}
}

THmMacroVariable THm::TMacro::getVar(std::wstring varname)
{
	TestDynamicVar = nullptr;
	auto dll_invocant = TDllBindType::getInvocantString();
	wstring cmd =
		L"##_tmp_dll_id_ret = dllfuncw( " + dll_invocant + L"\"SetDynamicVar\", " + varname + L");\n"
		L"##_tmp_dll_id_ret = 0;\n";
	auto ret = Hm.Macro.doEval(cmd);

	return TestDynamicVar;
}

bool THm::TMacro::setVar(std::wstring varname, THmMacroVariable value)
{
	BOOL success = 0;

	auto dll_invocant = TDllBindType::getInvocantString();

	wchar_t start = varname[0];
	if (start == L'#') {

		// 数字を数値にトライ。ダメなら0だよ。
		THmNumber n = 0;
		try {
			n = std::get<THmNumber>(value);

			PushNumVar(n);
			wstring cmd = L" = dllfuncw( " + dll_invocant + L"\"PopNumVar\" );\n";
			cmd = varname + cmd;
			auto ret = Hm.Macro.doEval(cmd);
			success = (ret.getResult() > 0);
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
			success = (ret.getResult() > 0);
		}
		catch (exception& e) {
			OutputDebugStringA(e.what());
		}
	}

	return success;
}

/** joins a vector of strings into a single string */
std::wstring StringJoin(const std::vector<std::wstring>& strs, const std::wstring delim)
{
	if (strs.size() == 0) return L"";
	std::vector<wchar_t> res;
	for (size_t i = 0; i < strs.size() - 1; ++i)
	{
		for (auto c : strs[i]) res.push_back(c);
		for (auto c : delim) res.push_back(c);
	}
	for (auto c : strs[strs.size() - 1]) res.push_back(c);
	return std::wstring{ res.begin(), res.end() };
}

void Hidemaru::THm::TMacro::SetMacroVarAndMakeMacroKeyArray(const vector<THmMacroVariable> value_args, std::vector<std::wstring>& varname_list)
{
	int cur_random = rand() + 1;
	for (size_t i = 0; i < value_args.size(); i++) {
		auto o = value_args[i];

		try {
			THmNumber v = std::get<THmNumber>(o);
			// 型エラーならここでbad_variant_accessとなる。

			wstring varname = L"#AsMacroArs_" + to_wstring(cur_random + i);
			varname_list.push_back(varname);
			setVar(varname, o);
		}
		catch (std::bad_variant_access&/* e*/) {
		}

		try {
			wstring v = std::get<wstring>(o);
			// 型エラーならここでbad_variant_accessとなる。

			wstring varname = L"$AsMacroArs_" + to_wstring(cur_random + i);
			varname_list.push_back(varname);
			setVar(varname, o);
		}
		catch (std::bad_variant_access&/* e*/) {
		}
	}
}

void Hidemaru::THm::TMacro::ClearMacroVarAndUpdateArgs(const vector<THmMacroVariable> value_args, const std::vector<std::wstring> varname_list, vector<THmMacroVariable>& updated_value_args)
{
	for (size_t i = 0; i < varname_list.size(); i++) {
		wstring varname = varname_list[i];
		if (varname.find(L"#AsMacroArs_") != wstring::npos) {
			try {
				updated_value_args.push_back(getVar(varname));
			}
			catch (...) {
				updated_value_args.push_back(value_args[i]);
			}

			setVar(varname, (THmNumber)0);
		}
		else if (varname.find(L"$AsMacroArs_") != wstring::npos) {
			try {
				updated_value_args.push_back(getVar(varname));
			}
			catch (...) {
				updated_value_args.push_back(value_args[i]);
			}
			setVar(varname, L"");
		}
	}
}

THm::TMacro::IFunctionResult Hidemaru::THm::TMacro::doFunction(std::wstring func_name, THmMacroVariable args0, THmMacroVariable args1, THmMacroVariable args2, THmMacroVariable args3, THmMacroVariable args4, THmMacroVariable args5, THmMacroVariable args6, THmMacroVariable args7, THmMacroVariable args8, THmMacroVariable args9)
{
	vector<THmMacroVariable> value_args = { args0, args1, args2, args3, args4, args5, args6, args7, args8, args9 };
	for (int i = 0; i < (int)value_args.size(); i++) {
		try {
			auto value = std::get<nullptr_t>(value_args[i]);
			value_args.resize(i);
			break;
		}
		catch (...) {
		}
	}
	vector<wstring> varname_list;
	SetMacroVarAndMakeMacroKeyArray(value_args, varname_list);

	wstring arg_varname_list = StringJoin(varname_list, L", ");
	wstring utf16_expression = func_name + L"(" + arg_varname_list + L")";

	// GetVar相当-------------------------
	TestDynamicVar = nullptr;
	auto dll_invocant = TDllBindType::getInvocantString();
	wstring cmd =
		L"##_tmp_dll_id_ret = dllfuncw( " + dll_invocant + L"\"SetDynamicVar\", " + utf16_expression + L");\n"
		L"##_tmp_dll_id_ret = 0;\n";
	auto ret = doEval(cmd);
	auto retDynamicVal = TestDynamicVar;
	vector<THmMacroVariable> updated_value_args;
	ClearMacroVarAndUpdateArgs(value_args, varname_list, updated_value_args);

	if (ret.getResult() > 0) {
		IFunctionResult r = IFunctionResult(retDynamicVal, updated_value_args, std::nullopt, L"");
		return r;
	}
	else {
		OutputDebugString(L"マクロの実行に失敗しました。\n");
		OutputDebugString(L"マクロ内容:\n");
		OutputDebugString(utf16_expression.c_str());
		exception e = exception("Hidemaru_MacroEvalException");
		IFunctionResult r = IFunctionResult(nullptr, updated_value_args, e, L"");
		return r;
	}
}

THm::TMacro::IStatementResult Hidemaru::THm::TMacro::doStatement(std::wstring statement_name, THmMacroVariable args0, THmMacroVariable args1, THmMacroVariable args2, THmMacroVariable args3, THmMacroVariable args4, THmMacroVariable args5, THmMacroVariable args6, THmMacroVariable args7, THmMacroVariable args8, THmMacroVariable args9)
{
	vector<THmMacroVariable> value_args = { args0, args1, args2, args3, args4, args5, args6, args7, args8, args9 };
	for (int i = 0; i < (int)value_args.size(); i++) {
		try {
			auto value = std::get<nullptr_t>(value_args[i]);
			value_args.resize(i);
			break;
		}
		catch (...) {
		}
	}
	vector<wstring> varname_list;
	SetMacroVarAndMakeMacroKeyArray(value_args, varname_list);

	wstring arg_varname_list = StringJoin(varname_list, L", ");
	wstring utf16_expression = statement_name + L" " + arg_varname_list;
	wstring cmd = statement_name + L" " + arg_varname_list + L";\n";

	auto ret = doEval(cmd);

	vector<THmMacroVariable> updated_value_args;
	ClearMacroVarAndUpdateArgs(value_args, varname_list, updated_value_args);

	if (ret.getResult() > 0) {
		IStatementResult r = IStatementResult(ret.getResult(), updated_value_args, std::nullopt, L"");
		return r;
	}
	else {
		OutputDebugString(L"マクロの実行に失敗しました。\n");
		OutputDebugString(L"マクロ内容:\n");
		OutputDebugString(utf16_expression.c_str());
		exception e = exception("Hidemaru_MacroEvalException");
		IStatementResult r = IStatementResult(ret.getResult(), updated_value_args, e, L"");
		return r;
	}
}


THmNumber THm::TMacro::IResult::getResult()
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

THm::TMacro::IResult::IResult(THmNumber result, THmMacroResultError error, std::wstring message)
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

THmNumber Hidemaru::THm::TMacro::IStatementResult::getResult()
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

Hidemaru::THm::TMacro::IStatementResult::IStatementResult(THmNumber result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message)
{
	this->result = result;
	this->args = args;
	this->error = error;
	this->message = message;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doEval(std::wstring expression)
{
	if (Hm.Macro.isExecuting()) {
		std::exception e = std::runtime_error("Hidemaru_MacroIsExecutingException");
		THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
		return r;
	}

	if (Hidemaru_GetCurrentWindowHandle) {

		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		const int WM_REMOTE_EXECMACRO_MEMORY = WM_USER + 272;

		const WORD bufSize = 0xFFFF;
		unique_ptr<wchar_t[]> mngWszReturn = make_unique<wchar_t[]>(bufSize);
		wchar_t* wszReturn = mngWszReturn.get();
		*(WORD*)wszReturn = bufSize; // 最初のバイトにバッファーのサイズを格納することで秀丸本体がバッファーサイズの上限を知る。

		LRESULT lRet = SendMessage(hHidemaruWindow, WM_REMOTE_EXECMACRO_MEMORY, (WPARAM)wszReturn, (LPARAM)expression.c_str());
		if (lRet) {
			wstring wstrreturn = wszReturn;
			std::exception e = std::exception();
			THm::TMacro::IResult r = THm::TMacro::IResult((THmNumber)lRet, std::nullopt, wstrreturn);
			return r;
		}
		else {
			OutputDebugString(L"マクロの実行に失敗しました。\n");
			OutputDebugString(L"マクロ内容:\n");
			OutputDebugString(expression.c_str());
			std::exception e = std::runtime_error("Hidemaru_MacroExecEvalException");
			THm::TMacro::IResult r = THm::TMacro::IResult((THmNumber)lRet, std::nullopt, L"");
			return r;
		}
	}

	std::exception e = std::runtime_error("Hidemaru_MacroExecEvalException");
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doFile(std::wstring filepath)
{
	if (Hm.Macro.isExecuting()) {
		std::exception e = std::runtime_error("Hidemaru_MacroIsExecutingException");
		THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
		return r;
	}

	if (Hidemaru_GetCurrentWindowHandle) {

		HWND hHidemaruWindow = Hidemaru_GetCurrentWindowHandle();
		const int WM_REMOTE_EXECMACRO_FILE = WM_USER + 271;

		const WORD bufSize = 0xFFFF;
		unique_ptr<wchar_t[]> mngWszReturn = make_unique<wchar_t[]>(bufSize);
		wchar_t* wszReturn = mngWszReturn.get();
		*(WORD*)wszReturn = bufSize; // 最初のバイトにバッファーのサイズを格納することで秀丸本体がバッファーサイズの上限を知る。

		LRESULT lRet = SendMessage(hHidemaruWindow, WM_REMOTE_EXECMACRO_FILE, (WPARAM)wszReturn, (LPARAM)filepath.c_str());
		if (lRet) {
			wstring wstrreturn = wszReturn;
			std::exception e = std::exception();
			THm::TMacro::IResult r = THm::TMacro::IResult((THmNumber)lRet, std::nullopt, wstrreturn);
			return r;
		}
		else {
			OutputDebugString(L"マクロの実行に失敗しました。\n");
			OutputDebugString(L"マクロ内容:\n");
			OutputDebugString(filepath.c_str());
			std::exception e = std::runtime_error("Hidemaru_MacroExecFileException");
			THm::TMacro::IResult r = THm::TMacro::IResult((THmNumber)lRet, std::nullopt, L"");
			return r;
		}
	}

	std::exception e = std::runtime_error("Hidemaru_MacroExecFileException");
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

// 関数を実行する
HM_DLLEXPORT THmNumber DoDelegateMethod(THmNumber func_address, const wchar_t* message_parameter ) {
	THmMacroDoMethodType callback_method = (THmMacroDoMethodType)(intptr_t)func_address;
	return callback_method(message_parameter);

}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doMethod(std::wstring message_parameter, THmMacroDoMethodType delegate_method)
{
	if (Hm.Macro.isExecuting()) {
		std::exception e = std::runtime_error("Hidemaru_MacroIsExecutingException");
		THm::TMacro::IResult ret = THm::TMacro::IResult(0, e, L"");
		return ret;
	}
	wstring strDllFullPath = Hm.DllBindType.getSelfModuleFullPath();
	wstring &scopeName = message_parameter;

	intptr_t func_address = (intptr_t)(*delegate_method);

	wstring expression = LR"EXP(
		#_dll_dotnet_newscope = loaddll(R"DLLPATH()EXP" + strDllFullPath + LR"EXP()DLLPATH");
    	#_r_dotnet_newscope = dllfuncw(#_dll_dotnet_newscope, "DoDelegateMethod", )EXP" + std::to_wstring(func_address) + LR"EXP(, R"MACRO_OF_SCOPENAME()EXP" + scopeName + LR"EXP()MACRO_OF_SCOPENAME");
        endmacro R"MACRO_OF_SCOPENAME()EXP" + scopeName + LR"EXP()MACRO_OF_SCOPENAME";
    )EXP";
	auto ret = Hm.Macro.Exec.doEval(expression);
	return ret;
}

