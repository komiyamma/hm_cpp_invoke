#include "HmCppInvoke.h"

#include <shlwapi.h>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std;
using namespace Hidemaru;


THm Hm = THm();


double THm::hm_version = 0;
THm::PFNGetCurrentWindowHandle THm::Hidemaru_GetCurrentWindowHandle = NULL;

HMODULE THm::hHideExeHandle = NULL;
wchar_t THm::wszHidemaruFullPath[MAX_PATH] = L"";

HMODULE THm::TOutputPane::hHmOutputPaneDLL = NULL;
HMODULE THm::TExplorerPane::hHmExplorerPaneDLL = NULL;


THm::THm()
{
	// Ž©•ªŽ©g‚Ìdll‚Ìî•ñ‚ÍAˆê”Ôæ‚ÉÝ’è‚µ‚Ä‚¨‚­
	this->DllBindAttribute = TDllBindAttribute();

	GetModuleFileName(NULL, wszHidemaruFullPath, _countof(wszHidemaruFullPath));

	bool success = setVersion();
	if (success)
	{
		if (hm_version >= 873.99) {

			// GŠÛ–{‘Ì‚ÉŠÖ”‚ª‚ ‚é‚Ì‚Í 8.73.99ˆÈã
			hHideExeHandle = LoadLibrary(wszHidemaruFullPath);
			if (hHideExeHandle) {
				Hidemaru_GetCurrentWindowHandle = (PFNGetCurrentWindowHandle)GetProcAddress(hHideExeHandle, "Hidemaru_GetCurrentWindowHandle");
			}

			this->DllMacroVariant = TDllMacroVariant();
			this->Edit = TEdit();
			this->File = TFile();
			this->Macro = TMacro();
			this->OutputPane = TOutputPane();
			this->ExplorerPane = TExplorerPane();
		}
		else {
			throw std::runtime_error("Hidemaru_MissingVersionException:" + to_string( this->getVersion() ) + "<873.99");
		}
	}
}

double THm::QueryFileVersion(wchar_t* path)
{
	VS_FIXEDFILEINFO* v;
	UINT len;
	DWORD sz = GetFileVersionInfoSize(path, NULL);
	if (sz) {
		unique_ptr<BYTE[]> mngBuf = make_unique<BYTE[]>(sz);
		LPVOID buf = (LPVOID)mngBuf.get();
		GetFileVersionInfo(path, NULL, sz, buf);

		if (VerQueryValue(buf, L"\\", (LPVOID*)&v, &len)) {
			double ret = 0;
			ret = double(HIWORD(v->dwFileVersionMS)) * 100 +
				double(LOWORD(v->dwFileVersionMS)) * 10 +
				double(HIWORD(v->dwFileVersionLS)) +
				double(LOWORD(v->dwFileVersionLS)) * 0.01;
			return ret;
		}
	}

	return 0;
}

double THm::getVersion()
{
	// š
	return hm_version;
}

HWND THm::getWindowHandle()
{
	return Hidemaru_GetCurrentWindowHandle();
}

bool THm::setVersion()
{
	hm_version = QueryFileVersion(wszHidemaruFullPath);
	if (hm_version > 0.0) {
		return true;
	}
	return false;
}

