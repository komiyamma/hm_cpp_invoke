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
	return false;
}

THm::TMacro::IResult THm::TMacro::doEval(std::wstring expression)
{
	std::exception e = std::exception();
	THm::TMacro::IResult r = THm::TMacro::IResult(0, e, L"");
	return r;
}

std::any THm::TMacro::getVar(std::wstring varname)
{
	return std::any();
}

bool THm::TMacro::setVar(std::wstring varname, std::any)
{
	return false;
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

