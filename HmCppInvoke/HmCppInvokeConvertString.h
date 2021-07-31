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
			/// <summary>
			/// 「string型のutf8」を「wstring型のutf16」へ
			/// </summary>
			static std::wstring utf8_to_utf16(const std::string& text);

			/// <summary>
			/// 「wstring型のutf16」を「string型のutf8」へ
			/// </summary>
			static std::string utf16_to_utf8(const std::wstring& text);

			/// <summary>
			/// 「string型のcp932」を「wstring型のutf16」へ
			/// <para>(正確には932ではなくシステムOEMコードページ。Windows日本語モードだと932)</para>
			/// </summary>
			static std::wstring cp932_to_utf16(const std::string& text);

			/// <summary>
			/// 「wstring型のutf16」を「string型のcp932」へ
			/// <para>(正確には932ではなくシステムOEMコードページ。Windows日本語モードだと932)</para>
			/// </summary>
			static std::string utf16_to_cp932(const std::wstring& text);
		};
	}
}