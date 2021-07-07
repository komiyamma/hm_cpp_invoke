#include "HmCppInvoke.h"


using namespace std;
using namespace Hidemaru;


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



