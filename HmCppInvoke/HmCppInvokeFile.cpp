/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


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

THm::TFile::IEncoding Hidemaru::THm::TFile::getEncoding(wstring filepath)
{
	return IEncoding();
}

const vector<int> Hidemaru::THm::TFile::getEncodingList()
{
	const vector<int> ret = {
		0,      // Unknown
		932,	// encode = 1 ANSI/OEM Japanese; Japanese (Shift-JIS)
		1200,	// encode = 2 Unicode UTF-16, little-endian
		51932,	// encode = 3 EUC
		50221,	// encode = 4 JIS
		65000,	// encode = 5 UTF-7
		65001,	// encode = 6 UTF-8
		1201,	// encode = 7 Unicode UTF-16, big-endian
		1252,	// encode = 8 欧文 ANSI Latin 1; Western European (Windows)
		936,	// encode = 9 簡体字中国語 ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)
		950,	// encode =10 繁体字中国語 ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)
		949,	// encode =11 韓国語 ANSI/OEM Korean (Unified Hangul Code)
		1361,	// encode =12 韓国語 Korean (Johab)
		1250,	// encode =13 中央ヨーロッパ言語 ANSI Central European; Central European (Windows)
		1257,	// encode =14 バルト語 ANSI Baltic; Baltic (Windows)
		1253,	// encode =15 ギリシャ語 ANSI Greek; Greek (Windows)
		1251,	// encode =16 キリル言語 ANSI Cyrillic; Cyrillic (Windows)
		42,		// encode =17 シンボル
		1254,	// encode =18 トルコ語 ANSI Turkish; Turkish (Windows)
		1255,	// encode =19 ヘブライ語 ANSI Hebrew; Hebrew (Windows)
		1256,	// encode =20 アラビア語 ANSI Arabic; Arabic (Windows)
		874,	// encode =21 タイ語 ANSI/OEM Thai (same as 28605, ISO 8859-15); Thai (Windows)
		1258,	// encode =22 ベトナム語 ANSI/OEM Vietnamese; Vietnamese (Windows)
		10001,	// encode =23 x-mac-japanese Japanese (Mac)
		850,	// encode =24 OEM/DOS
		0,		// encode =25 その他
		12000,	// encode =26 Unicode (UTF-32) little-endian
		12001	// encode =27 Unicode (UTF-32) big-endian
	};

	return ret;
}

int Hidemaru::THm::TFile::getMsCodePageFromHmEncode(int hm_encode)
{
	auto& encode_list = this->getEncodingList();
	if (0 <= hm_encode && hm_encode < encode_list.size()) {
		return encode_list[hm_encode];
	}
	return 0;
}

int Hidemaru::THm::TFile::getHmEncodeFromMsCodePage(int ms_codepage)
{
	auto& encode_list = this->getEncodingList();
	for (int hm_enc = 0; hm_enc < (int)encode_list.size(); hm_enc++) {
		if (encode_list[hm_enc] == ms_codepage) {
			return hm_enc;
		}
	}
	return 0;
}


THm::TFile::IEncoding THm::TFile::IHidemaruStreamReader::getEncoding()
{
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



