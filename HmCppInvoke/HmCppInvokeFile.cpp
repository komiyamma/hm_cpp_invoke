/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include <shlwapi.h>

#include "HmCppInvoke.h"
#include "HmCppInvokeConvertString.h"

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

Hidemaru::THm::TFile::IEncoding::IEncoding(int hm_encode, int ms_codepage)
{
	this->hm_encode = hm_encode;
	this->ms_codepage = ms_codepage;
}


int THm::TFile::IEncoding::getHmEncode()
{
	return hm_encode;
}

int THm::TFile::IEncoding::getMsCodePage()
{
	return ms_codepage;
}

THm::TFile::IEncoding Hidemaru::THm::TFile::getEncoding(int hm_encode)
{
	auto to_mscp_list = getEncodingMap();
	if (to_mscp_list.count(hm_encode) > 0) {
		IEncoding encode = IEncoding(hm_encode, to_mscp_list[hm_encode]);
		return encode;
	}

	IEncoding no_encode = IEncoding(-1, -1);
	return no_encode;

}

THm::TFile::IEncoding Hidemaru::THm::TFile::getEncoding(wstring filepath)
{
	if (Hidemaru_AnalyzeEncoding) {
		int exists = PathFileExists(filepath.c_str());
		if (exists == 0) {
			wstring error_msg = L"FileNotFoundException:" + filepath;
			throw runtime_error(Text::Encoding::utf16_to_utf8(error_msg).c_str());
		}
		int hm_encode = Hidemaru_AnalyzeEncoding(filepath.c_str(), NULL, NULL);
		if (hm_encode >= 0) {
			return getEncoding(hm_encode);
		}
	}

	return getEncoding(-1);
}

const map<int, int> Hidemaru::THm::TFile::getEncodingMap()
{
	const map<int, int> ret = {
		{0,   0},       // Unknown
		{1,   932},	    // encode = 1 ANSI/OEM Japanese; Japanese (Shift-JIS)
		{2,   1200},	// encode = 2 Unicode UTF-16, little-endian
		{3,   51932},	// encode = 3 EUC
		{4,   50221},	// encode = 4 JIS
		{5,   65000},	// encode = 5 UTF-7
		{6,   65001},	// encode = 6 UTF-8
		{7,   1201},	// encode = 7 Unicode UTF-16, big-endian
		{8,   1252},	// encode = 8 欧文 ANSI Latin 1; Western European (Windows)
		{9,   936},	    // encode = 9 簡体字中国語 ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
		{10,  950},	    // encode =10 繁体字中国語 ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)
		{11,  949},	    // encode =11 韓国語 ANSI/OEM Korean (Unified Hangul Code)
		{12,  1361},	// encode =12 韓国語 Korean (Johab)
		{13,  1250},	// encode =13 中央ヨーロッパ言語 ANSI Central European; Central European (Windows)
		{14,  1257},	// encode =14 バルト語 ANSI Baltic; Baltic (Windows)
		{15,  1253},	// encode =15 ギリシャ語 ANSI Greek; Greek (Windows)
		{16,  1251},	// encode =16 キリル言語 ANSI Cyrillic; Cyrillic (Windows)
		{17,  42},		// encode =17 シンボル
		{18,  1254},	// encode =18 トルコ語 ANSI Turkish; Turkish (Windows)
		{19,  1255},	// encode =19 ヘブライ語 ANSI Hebrew; Hebrew (Windows)
		{20,  1256},	// encode =20 アラビア語 ANSI Arabic; Arabic (Windows)
		{21,  874},	    // encode =21 タイ語 ANSI/OEM Thai (same as 28605, ISO 8859-15); Thai (Windows)
		{22,  1258},	// encode =22 ベトナム語 ANSI/OEM Vietnamese; Vietnamese (Windows)
		{23,  10001},	// encode =23 x-mac-japanese Japanese (Mac)
		{24,  850},	    // encode =24 OEM/DOS
		{25,  0},		// encode =25 その他
		{26,  12000},	// encode =26 Unicode (UTF-32) little-endian
		{27,  12001}	// encode =27 Unicode (UTF-32) big-endian
	};

	return ret;
}


Hidemaru::THm::TFile::IHidemaruStreamReader::IHidemaruStreamReader(std::wstring filepath, IEncoding encoding)
{
	this->filepath = filepath;
	this->encoding = encoding;
}

Hidemaru::THm::TFile::IHidemaruStreamReader::~IHidemaruStreamReader()
{
	this->close();
}

THm::TFile::IEncoding THm::TFile::IHidemaruStreamReader::getEncoding()
{
	return this->encoding;
}

wstring THm::TFile::IHidemaruStreamReader::read()
{
	if (Hidemaru_LoadFileUnicode) {
		UINT pcwchOut = 0;
		HGLOBAL hGlobal = Hidemaru_LoadFileUnicode(filepath.c_str(), this->encoding.getHmEncode(), &pcwchOut, NULL, NULL);
		if (hGlobal) {
			wchar_t* pwsz = (wchar_t*)GlobalLock(hGlobal);
			if (pwsz) {
				wstring text(pwsz); // コピー
				GlobalUnlock(hGlobal);
				GlobalFree(hGlobal); // 元のは解放
				return text;
			}
		}
	}

	return L"";
}

wstring THm::TFile::IHidemaruStreamReader::getFilePath()
{
	return this->filepath;
}

void THm::TFile::IHidemaruStreamReader::close()
{
	this->filepath = L"";
	this->encoding = IEncoding(-1, -1);
}

THm::TFile::IHidemaruStreamReader THm::TFile::open(std::wstring filepath, int hm_encode)
{
	if (hm_encode == -1) {
		IEncoding encoding = getEncoding(filepath);
		IHidemaruStreamReader sr = IHidemaruStreamReader(filepath, encoding);
		return sr;
	}
	else {
		IEncoding encoding = getEncoding(hm_encode);
		IHidemaruStreamReader sr = IHidemaruStreamReader(filepath, encoding);
		return sr;
	}

}



