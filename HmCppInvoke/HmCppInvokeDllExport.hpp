#pragma once
/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#define HM_DLLEXPORT extern "C" __declspec(dllexport)

namespace Hidemaru {
	extern THmMacroVariable TestDynamicVar;
}

using namespace Hidemaru;

// 秀丸マクロ上の変数と、C++内部との自動やりとりに使用される。
HM_DLLEXPORT THmNumber SetDynamicVar(const void* dynamic_value);
HM_DLLEXPORT THmNumber PopNumVar();
HM_DLLEXPORT THmNumber PushNumVar(THmNumber i_tmp_num);
HM_DLLEXPORT const wchar_t* PopStrVar();
HM_DLLEXPORT THmNumber PushStrVar(const wchar_t* wsz_tmp_str);

HM_DLLEXPORT THmNumber DoDelegateMethod(THmNumber func_address, const wchar_t* message_parameter);
