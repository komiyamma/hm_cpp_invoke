#pragma once
/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

#include <string>

namespace Hidemaru {
	namespace Text {
		class Encoding {
		public:
			// utf8→utf16へ
			static std::wstring utf8_to_utf16(const std::string& text);

			// utf16→utf8へ
			static std::string utf16_to_utf8(const std::wstring& text);

			// cp932→utf16へ(正確には932ではなくシステムOEMコードページ。Windows日本語モードだと932)
			static std::wstring cp932_to_utf16(const std::string& text);

			// utf16→cp932へ(正確には932ではなくシステムOEMコードページ。Windows日本語モードだと932)
			static std::string utf16_to_cp932(const std::wstring& text);
		};
	}
}