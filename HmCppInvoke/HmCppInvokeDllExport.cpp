/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#include "HmCppInvoke.h"

using namespace std;
using namespace Hidemaru;

THm::PFNGetDllFuncCalledType THmDllExport::Hidemaru_GetDllFuncCalledType = NULL;

bool THm::rebindParameters() {
	// これ重要。loadllの形態を保持する。関数が呼ばれる度に更新しておく。
	return TSelfDllAttribute::setBindDllType();
}




THmMacroVariable Hidemaru::TestDynamicVar = nullptr;


// 秀丸の変数が文字列か数値かの判定用
HM_DLLEXPORT long SetDynamicVar(const void* dynamic_value) {

	auto param_type = (THm::DLLFUNCPARAM)THm::Hidemaru_GetDllFuncCalledType(1);
	if (param_type == THm::DLLFUNCPARAM::WCHAR_PTR) {
		Hidemaru::TestDynamicVar = wstring((wchar_t*)dynamic_value);
		return 1;
	}
	else {
		Hidemaru::TestDynamicVar = (long)(intptr_t)dynamic_value;
		return 1;
	}
}


long popnumvar = 0;
// スタックした変数を秀丸マクロから取り出す。内部処理用
HM_DLLEXPORT long PopNumVar() {
	return popnumvar;
}

// 変数を秀丸マクロから取り出すためにスタック。内部処理用
HM_DLLEXPORT long PushNumVar(long i_tmp_num) {
	popnumvar = i_tmp_num;
	return 1;
}

// スタックした変数を秀丸マクロから取り出す。内部処理用
static wstring popstrvar;
HM_DLLEXPORT const wchar_t* PopStrVar() {
	return popstrvar.data();
}

// 変数を秀丸マクロから取り出すためにスタック。内部処理用
HM_DLLEXPORT long PushStrVar(const wchar_t* sz_tmp_str) {
	popstrvar = sz_tmp_str;
	return 1;
}

