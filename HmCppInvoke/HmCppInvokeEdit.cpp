#include "HmCppInvoke.h"


using namespace std;
using namespace Hidemaru;



THm::TEdit::PFNGetTotalTextUnicode THm::TEdit::Hidemaru_GetTotalTextUnicode = NULL;
THm::TEdit::PFNGetSelectedTextUnicode THm::TEdit::Hidemaru_GetSelectedTextUnicode = NULL;
THm::TEdit::PFNGetLineTextUnicode THm::TEdit::Hidemaru_GetLineTextUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicode THm::TEdit::Hidemaru_GetCursorPosUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicodeFromMousePos THm::TEdit::Hidemaru_GetCursorPosUnicodeFromMousePos = NULL;


THm::TEdit::TEdit()
{
	if (hHideExeHandle) {
		Hidemaru_GetTotalTextUnicode = (PFNGetTotalTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetTotalTextUnicode");
		Hidemaru_GetSelectedTextUnicode = (PFNGetSelectedTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetSelectedTextUnicode");
		Hidemaru_GetLineTextUnicode = (PFNGetLineTextUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetLineTextUnicode");
		Hidemaru_GetCursorPosUnicode = (PFNGetCursorPosUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_GetCursorPosUnicode");
		Hidemaru_GetCursorPosUnicodeFromMousePos = (PFNGetCursorPosUnicodeFromMousePos)GetProcAddress(hHideExeHandle, "Hidemaru_GetCursorPosUnicodeFromMousePos");
	}
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



