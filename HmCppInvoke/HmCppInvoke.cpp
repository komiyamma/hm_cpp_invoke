#include "HmCppInvoke.h"

using namespace Hidemaru;

THm Hm = THm();

THm::THm()
{
	Edit = TEdit();
	OutputPane = TOutputPane();
	ExplorerPane = TExplorerPane();
}

const double THm::getVersion()
{
	// Åö
	return 0.0;
}

bool THm::setVersion()
{
	// Åö
	return false;
}

















THm::TOutputPane::TOutputPane()
{
}

bool THm::TOutputPane::output(std::wstring message)
{
	// Åö
	return 0;
}

bool THm::TOutputPane::push()
{
	// Åö
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

LRESULT THm::TOutputPane::sendMessage(int command_id)
{
	return intptr_t();
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

LRESULT THm::TExplorerPane::sendMessage(int command_id)
{
	return LRESULT();
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







int THm::TEdit::TCursorPos::getLineNo()
{
	return 0;
}

int THm::TEdit::TCursorPos::getColumn()
{
	return 0;
}

int THm::TEdit::TMousePos::getLineNo()
{
	return 0;
}

int THm::TEdit::TMousePos::getColumn()
{
	return 0;
}

int THm::TEdit::TMousePos::getX()
{
	return 0;
}

int THm::TEdit::TMousePos::getY()
{
	return 0;
}
