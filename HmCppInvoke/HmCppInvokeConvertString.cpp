/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include <vector>
#include <windows.h>

#include "HmCppInvokeConvertString.h"

namespace Hidemaru {
	namespace Text {
		std::wstring Encoding::utf8_to_utf16(const std::string& text)
		{
			int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
			std::vector<wchar_t> buf(size);
			size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &buf[0], (int)buf.size());
			return std::wstring(buf.begin(), buf.begin() + size).data(); // このdata()必要
		}

		std::string Encoding::utf16_to_utf8(const std::wstring& text)
		{
			int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, NULL, 0, 0, NULL);
			std::vector<char> buf(size);
			size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, &buf[0], (int)buf.size(), 0, NULL);
			return std::string(buf.begin(), buf.begin() + size).data(); // このdata()必要
		}

		std::wstring Encoding::cp932_to_utf16(const std::string& text)
		{
			int size = MultiByteToWideChar(CP_OEMCP, 0, text.c_str(), -1, NULL, 0);
			std::vector<wchar_t> buf(size);
			size = MultiByteToWideChar(CP_OEMCP, 0, text.c_str(), -1, &buf[0], (int)buf.size());
			return std::wstring(buf.begin(), buf.begin() + size).data(); // このdata()必要
		}

		std::string Encoding::utf16_to_cp932(const std::wstring& text)
		{
			int size = WideCharToMultiByte(CP_OEMCP, 0, text.c_str(), -1, NULL, 0, 0, NULL);
			std::vector<char> buf(size);
			size = WideCharToMultiByte(CP_OEMCP, 0, text.c_str(), -1, &buf[0], (int)buf.size(), 0, NULL);
			return std::string(buf.begin(), buf.begin() + size).data(); // このdata()必要
		}
	}
}