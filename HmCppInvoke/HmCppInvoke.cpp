#include "HmCppInvoke.h"

#include <exception>
#include <stdexcept>
#include <windows.h>
#include <shlwapi.h>
#include <string>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std;
using namespace Hidemaru;


THm Hm = THm();

double THm::hm_version = 0;
THm::PFNGetDllFuncCalledType THm::Hidemaru_GetDllFuncCalledType = NULL;
THm::PFNGetCurrentWindowHandle THm::Hidemaru_GetCurrentWindowHandle = NULL;

HMODULE THm::hHideExeHandle = NULL;
wchar_t THm::szHidemaruFullPath[MAX_PATH] = L"";

HMODULE THm::TOutputPane::hHmOutputPaneDLL = NULL;
HMODULE THm::TExplorerPane::hHmExplorerPaneDLL = NULL;


THm::THm()
{
	GetModuleFileName(NULL, szHidemaruFullPath, _countof(szHidemaruFullPath));

	bool success = setVersion();
	if (success)
	{
		if (this->getVersion() >= 8.66) {

			// 秀丸本体に関数があるのは 8.66以上
			hHideExeHandle = LoadLibrary(szHidemaruFullPath);
			if (hHideExeHandle) {
				Hidemaru_GetDllFuncCalledType = (PFNGetDllFuncCalledType)GetProcAddress(hHideExeHandle, "Hidemaru_GetDllFuncCalledType");
				Hidemaru_GetCurrentWindowHandle = (PFNGetCurrentWindowHandle)GetProcAddress(hHideExeHandle, "Hidemaru_GetCurrentWindowHandle");
			}

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


double THm::QueryFileVersion(wchar_t* path)
{
	VS_FIXEDFILEINFO* v;
	DWORD dwZero = 0;
	UINT len;
	DWORD sz = GetFileVersionInfoSize(path, &dwZero);
	if (sz) {
		void* buf = new char[sz];
		GetFileVersionInfo(path, 0, sz, buf);

		if (VerQueryValue(buf, L"\\", (LPVOID*)&v, &len)) {
			double ret = 0;
			ret = double(HIWORD(v->dwFileVersionMS)) * 100 +
				double(LOWORD(v->dwFileVersionMS)) * 10 +
				double(HIWORD(v->dwFileVersionLS)) +
				double(LOWORD(v->dwFileVersionLS)) * 0.01;
			delete[] buf;
			return ret;
		}
		else {
			delete[] buf;
		}
	}

	return 0;
}

double THm::getVersion()
{
	// ★
	return hm_version;
}

HWND THm::getWindowHandle()
{
	return Hidemaru_GetCurrentWindowHandle();
}

bool THm::setVersion()
{
	hm_version = QueryFileVersion(szHidemaruFullPath);
	if (hm_version > 0.0) {
		return true;
	}
	// ★
	return false;
}

extern unsigned int hidemaru_encode_map[];
std::vector<BYTE> THm::EncodeWStringToOriginalEncodeVector(std::wstring original_string)
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



THm::TEdit::PFNGetTotalTextUnicode THm::TEdit::Hidemaru_GetTotalTextUnicode = NULL;
THm::TEdit::PFNGetSelectedTextUnicode THm::TEdit::Hidemaru_GetSelectedTextUnicode = NULL;
THm::TEdit::PFNGetLineTextUnicode THm::TEdit::Hidemaru_GetLineTextUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicode THm::TEdit::Hidemaru_GetCursorPosUnicode = NULL;
THm::TEdit::PFNGetCursorPosUnicodeFromMousePos THm::TEdit::Hidemaru_GetCursorPosUnicodeFromMousePos = NULL;


THm::TEdit::TEdit()
{
	if (hHideExeHandle) {
		Hidemaru_GetDllFuncCalledType = (PFNGetDllFuncCalledType)GetProcAddress(hHideExeHandle, "Hidemaru_GetDllFuncCalledType");
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










// アウトプットパネル
THm::TOutputPane::PFNHmOutputPane_Output THm::TOutputPane::HmOutputPane_Output = NULL;
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
		wcscpy_s(hidemarudir, szHidemaruFullPath);
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
			}
		}
	}
}

bool THm::TOutputPane::output(std::wstring message)
{
	// ★
	return 0;
}

bool THm::TOutputPane::push()
{
	// ★
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
		wcscpy_s(hidemarudir, szHidemaruFullPath);
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

THm::TFile::PFNAnalyzeEncoding THm::TFile::Hidemaru_AnalyzeEncoding = NULL;
THm::TFile::PFNLoadFileUnicode THm::TFile::Hidemaru_LoadFileUnicode = NULL;

THm::TFile::TFile()
{
	if (hHideExeHandle) {
		Hidemaru_AnalyzeEncoding = (PFNAnalyzeEncoding)GetProcAddress(hHideExeHandle, "Hidemaru_AnalyzeEncoding");
		Hidemaru_LoadFileUnicode = (PFNLoadFileUnicode)GetProcAddress(hHideExeHandle, "Hidemaru_LoadFileUnicode");
	}
}

int THm::TFile::IEncoding::getHmEncode()
{
	return 0;
}

int THm::TFile::IEncoding::getMsCodePage()
{
	return 0;
}

THm::TFile::IEncoding Hidemaru::THm::TFile::getEncoding()
{
	return IEncoding();
}

THm::TFile::IEncoding THm::TFile::IHidemaruStreamReader::getEncoding()
{
	return IEncoding();
}

wstring THm::TFile::IHidemaruStreamReader::read()
{
	return wstring();
}

wstring THm::TFile::IHidemaruStreamReader::getFilePath()
{
	return wstring();
}

void THm::TFile::IHidemaruStreamReader::close()
{
}

THm::TFile::IHidemaruStreamReader THm::TFile::open(std::wstring filepath, int hm_encode)
{
	return IHidemaruStreamReader();
}