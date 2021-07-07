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

std::any THm::TMacro::getVar(std::wstring varname)
{
	TestDynamicVar = nullptr;
	auto dll_invocant = SelfDllInfo::GetInvocantString();
	wstring cmd =
		L"##_tmp_dll_id_ret = dllfuncw( " + dll_invocant + L"\"SetDynamicVar\", " + varname + L");\n"
		L"##_tmp_dll_id_ret = 0;\n";
	BOOL success = Hidemaru_EvalMacro(cmd.c_str());

	return TestDynamicVar;
}







// 秀丸の変数が文字列か数値かの判定用
extern "C" __declspec(dllexport) intptr_t SetDynamicVar(const void* dynamic_value);
extern "C" __declspec(dllexport) intptr_t PopNumVar();
extern "C" __declspec(dllexport) intptr_t PushNumVar(intptr_t i_tmp_num);
extern "C" __declspec(dllexport) const wchar_t* PopStrVar();
extern "C" __declspec(dllexport) intptr_t PushStrVar(const wchar_t* sz_tmp_str);


bool THm::TMacro::setVar(std::wstring varname, std::any value)
{
	BOOL success = 0;

	auto dll_invocant = SelfDllInfo::GetInvocantString();

	wchar_t start = varname[0];
	if (start == L'#') {

		// 数字を数値にトライ。ダメなら0だよ。
		intptr_t n = 0;
		try {
			n = std::any_cast<intptr_t>(value);

			PushNumVar(n);
			wstring cmd = L" = dllfuncw( " + dll_invocant + L"\"PopNumVar\" );\n";
			cmd = varname + cmd;
			success = Hidemaru_EvalMacro(cmd.c_str());
		}
		catch (...) {}
	}
	else if (start == L'$') {
		try {
			wstring s = std::any_cast<wstring>(value);
			PushStrVar(s.data());
			wstring cmd = L" = dllfuncstrw( " + dll_invocant + L"\"PopStrVar\" );\n";
			cmd = varname + cmd;
			success = Hidemaru_EvalMacro(cmd.c_str());
		}
		catch (...) {}
	}

	return success;
}

THm::TMacro::IFunctionResult Hidemaru::THm::TMacro::doFunction(std::wstring func_name, std::any args0, std::any args1, std::any args2, std::any args3, std::any args4, std::any args5, std::any args6, std::any args7, std::any args8, std::any args9)
{
	std::vector<std::any> v;
	IFunctionResult r = IFunctionResult(0, v, nullptr, L"");
	return r;
}

THm::TMacro::IStatementResult Hidemaru::THm::TMacro::doStatement(std::wstring statement_name, std::any args0, std::any args1, std::any args2, std::any args3, std::any args4, std::any args5, std::any args6, std::any args7, std::any args8, std::any args9)
{
	std::vector<std::any> v;
	IStatementResult r = IStatementResult(0, v, nullptr, L"");
	return r;
}


int THm::TMacro::IResult::getResult()
{
	return 0;
}

std::any THm::TMacro::IResult::getException()
{
	return std::exception();
}

std::wstring THm::TMacro::IResult::getMessage()
{
	return std::wstring();
}

THm::TMacro::IResult::IResult(int result, std::any error, std::wstring message)
{
}

std::any THm::TMacro::IFunctionResult::getResult()
{
	return 0;
}

std::vector<std::any> THm::TMacro::IFunctionResult::getArgs()
{
	return std::vector<std::any>();
}

std::any THm::TMacro::IFunctionResult::getException()
{
	return std::any();
}

std::wstring THm::TMacro::IFunctionResult::getMessage()
{
	return std::wstring();
}

Hidemaru::THm::TMacro::IFunctionResult::IFunctionResult(std::any result, std::vector<std::any> args, std::any error, std::wstring message)
{
}

int Hidemaru::THm::TMacro::IStatementResult::getResult()
{
	return 0;
}

std::vector<std::any> Hidemaru::THm::TMacro::IStatementResult::getArgs()
{
	return std::vector<std::any>();
}

std::any Hidemaru::THm::TMacro::IStatementResult::getException()
{
	return std::any();
}

std::wstring Hidemaru::THm::TMacro::IStatementResult::getMessage()
{
	return std::wstring();
}

Hidemaru::THm::TMacro::IStatementResult::IStatementResult(int result, std::vector<std::any> args, std::any error, std::wstring message)
{
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doEval(std::wstring expression)
{
	std::exception e = std::exception();
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doFile(std::wstring filepath)
{
	std::exception e = std::exception();
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

Hidemaru::THm::TMacro::IResult Hidemaru::THm::TMacro::TExec::doMethod(std::wstring message_parameter, MacroScopeMethodPointer callback_method)
{
	std::exception e = std::exception();
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

