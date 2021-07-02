#include "HmCppInvoke.h"

#include <exception>
#include <stdexcept>

using namespace Hidemaru;

THm Hm = THm();

THm::THm()
{
	bool success = setVersion();
	if (success)
	{
		if (this->getVersion() >= 8.66) {
			this->Edit = TEdit();
			this->Macro = TMacro();
			this->OutputPane = TOutputPane();
			this->ExplorerPane = TExplorerPane();
		}
		else {
			throw std::runtime_error("Hidemaru Version Exception Under 8.66");
		}
	}
}

double THm::getVersion()
{
	// š
	return 0.0;
}

HWND THm::getWindowHandle()
{
	return HWND();
}

bool THm::setVersion()
{
	// š
	return false;
}

















THm::TOutputPane::TOutputPane()
{
}

bool THm::TOutputPane::output(std::wstring message)
{
	// š
	return 0;
}

bool THm::TOutputPane::push()
{
	// š
	return 0;
}

bool THm::TOutputPane::pop()
{
	return 0;
}

bool THm::TOutputPane::clear()
{
	return 0;
}

long THm::TOutputPane::sendMessage(int command_id)
{
	return long();
}

bool THm::TOutputPane::setBaseDir(std::wstring dirpath)
{
	return false;
}

HWND THm::TOutputPane::getWindowHandle()
{
	return HWND();
}



THm::TExplorerPane::TExplorerPane()
{
}

bool THm::TExplorerPane::setMode(int mode)
{
	return false;
}

int THm::TExplorerPane::getMode()
{
	return 0;
}

bool THm::TExplorerPane::loadProject(std::wstring filepath)
{
	return false;
}

bool THm::TExplorerPane::saveProject(std::wstring filepath)
{
	return false;
}

std::wstring THm::TExplorerPane::getProject()
{
	return std::wstring();
}

long THm::TExplorerPane::sendMessage(int command_id)
{
	return long();
}

HWND THm::TExplorerPane::getWindowHandle()
{
	return HWND();
}

bool THm::TExplorerPane::getUpdated()
{
	return false;
}

THm::TEdit::TEdit()
{
}

std::wstring THm::TEdit::getFilePath()
{
	return std::wstring();
}

std::wstring THm::TEdit::getTotalText()
{
	return std::wstring();
}

bool THm::TEdit::setTotalText(std::wstring text)
{
	return false;
}

std::wstring THm::TEdit::getSelectedText()
{
	return std::wstring();
}

bool THm::TEdit::setSelectedText(std::wstring text)
{
	return false;
}

std::wstring THm::TEdit::getLineText()
{
	return std::wstring();
}

bool THm::TEdit::setLineText(std::wstring text)
{
	return false;
}

bool THm::TEdit::isQueueStatus()
{
	return false;
}

THm::TEdit::ICursorPos THm::TEdit::getCursorPos()
{
	ICursorPos pos = ICursorPos(0, 0);
	return pos;
}

THm::TEdit::IMousePos THm::TEdit::getMousePos()
{
	IMousePos pos = IMousePos(0, 0, 0, 0);
	return pos;
}







int THm::TEdit::ICursorPos::getLineNo()
{
	return 0;
}

int THm::TEdit::ICursorPos::getColumn()
{
	return 0;
}

THm::TEdit::ICursorPos::ICursorPos(int lineno, int column)
{
}

int THm::TEdit::IMousePos::getLineNo()
{
	return 0;
}

int THm::TEdit::IMousePos::getColumn()
{
	return 0;
}

int THm::TEdit::IMousePos::getX()
{
	return 0;
}

int THm::TEdit::IMousePos::getY()
{
	return 0;
}

THm::TEdit::IMousePos::IMousePos(int lineno, int column, int x, int y)
{
}

THm::TMacro::TMacro()
{
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
