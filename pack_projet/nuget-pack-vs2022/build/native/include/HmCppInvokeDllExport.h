#pragma once
/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */


#define HM_DLLEXPORT extern "C" __declspec(dllexport)

namespace Hidemaru {
	extern THmMacroVariable TestDynamicVar;
    union TDYNAMICVARIABLE {
        double fValue;
        wchar_t* pszStr;
        intptr_t nValue;
    };
}



// 秀丸マクロ上の変数と、C++内部との自動やりとりに使用される。
HM_DLLEXPORT Hidemaru::THmNumber SetDynamicVar(Hidemaru::TDYNAMICVARIABLE dynamic_value);
HM_DLLEXPORT Hidemaru::THmNumber PopNumVar();
HM_DLLEXPORT Hidemaru::THmNumber PushNumVar(Hidemaru::THmNumber i_tmp_num);
HM_DLLEXPORT const wchar_t* PopStrVar();
HM_DLLEXPORT Hidemaru::THmNumber PushStrVar(const wchar_t* wsz_tmp_str);

HM_DLLEXPORT Hidemaru::THmNumber DoDelegateMethod(Hidemaru::THmNumber func_address, const wchar_t* message_parameter);
