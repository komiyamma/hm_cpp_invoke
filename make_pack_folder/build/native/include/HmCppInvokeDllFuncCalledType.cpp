/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

#include "HmCppInvoke.h"
#include "HmCppInvokeDllExport.h"
#include "HmCppInvokeDllFuncCalledType.h"

using namespace std;
using namespace Hidemaru;

THm::TDllFuncCalledType::PFNGetDllFuncCalledType THm::TDllFuncCalledType::Hidemaru_GetDllFuncCalledType = NULL;

THmMacroVariable Hidemaru::TestDynamicVar = nullptr;

Hidemaru::THm::TDllFuncCalledType::TDllFuncCalledType()
{
	if (hHideExeHandle) {
		Hidemaru_GetDllFuncCalledType = (PFNGetDllFuncCalledType)GetProcAddress(hHideExeHandle, "Hidemaru_GetDllFuncCalledType");
	}
}

bool THm::TDllFuncCalledType::isMissingMethod()
{
	if (!Hidemaru_GetDllFuncCalledType) {
		return true;
	}
	return false;
}

int THm::TDllFuncCalledType::getDllBindType()
{
	if (!Hidemaru_GetDllFuncCalledType) {
		return 0;
	}

	int dll = Hidemaru_GetDllFuncCalledType(-1); // 自分のdllの呼ばれ方をチェック
	return dll;
}

vector<THm::TDllFuncCalledType::DLLFUNCPARAM> THm::TDllFuncCalledType::getFuncParamTypeList()
{
	vector<DLLFUNCPARAM> param_type_list;
	if (!Hidemaru_GetDllFuncCalledType) {
		return param_type_list;
	}

	// 1番目の引数から99番目の引数まで型を調べる
	for (int i = 1; i < 99; i++) {
		auto param_type = (DLLFUNCPARAM)Hidemaru_GetDllFuncCalledType(i);
		if (param_type == DLLFUNCPARAM::NOPARAM) {
			break;
		}
		else {
			param_type_list.push_back(param_type);
		}
	}

	return param_type_list;
}

THm::TDllFuncCalledType::DLLFUNCPARAM THm::TDllFuncCalledType::getFuncParamType(int arg_n)
{
	if (!Hidemaru_GetDllFuncCalledType) {
		return DLLFUNCPARAM::NOPARAM;
	}

	auto param_type = Hidemaru_GetDllFuncCalledType(arg_n);

	return (DLLFUNCPARAM)param_type;
}

THm::TDllFuncCalledType::DLLFUNCRETURN THm::TDllFuncCalledType::getFuncReturnType()
{
	auto return_type = Hidemaru_GetDllFuncCalledType(0);

	return (DLLFUNCRETURN)return_type;
}

// 秀丸の変数が文字列か数値かの判定用
HM_DLLEXPORT THmNumber SetDynamicVar(TDYNAMICVARIABLE dynamic_value) {

	if (Hm.DllFuncCalledType.isMissingMethod()) {
		return 0;
	}
	auto param_type = Hm.DllFuncCalledType.getFuncParamType(1);
	if (param_type == THm::TDllFuncCalledType::DLLFUNCPARAM::WCHAR_PTR) {
		Hidemaru::TestDynamicVar = wstring((wchar_t*)dynamic_value.pszStr);
		return 1;
	}
	else {
#ifdef FLOATMACRO_COMPILE
		Hidemaru::TestDynamicVar = (THmNumber)dynamic_value.fValue;
#else
		Hidemaru::TestDynamicVar = (THmNumber)dynamic_value.nValue;
#endif
		return 1;
	}
}


static THmNumber popnumvar = 0;
// スタックした変数を秀丸マクロから取り出す。内部処理用
HM_DLLEXPORT THmNumber PopNumVar() {
	return popnumvar;
}

// 変数を秀丸マクロから取り出すためにスタック。内部処理用
HM_DLLEXPORT THmNumber PushNumVar(THmNumber i_tmp_num) {
	popnumvar = i_tmp_num;
	return 1;
}

// スタックした変数を秀丸マクロから取り出す。内部処理用
static wstring popstrvar;
HM_DLLEXPORT const wchar_t* PopStrVar() {
	return popstrvar.data();
}

// 変数を秀丸マクロから取り出すためにスタック。内部処理用
HM_DLLEXPORT THmNumber PushStrVar(const wchar_t* wsz_tmp_str) {
	popstrvar = wsz_tmp_str;
	return 1;
}

