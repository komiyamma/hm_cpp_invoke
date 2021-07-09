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
	return SelfDllInfo::setBindDllType();
}
