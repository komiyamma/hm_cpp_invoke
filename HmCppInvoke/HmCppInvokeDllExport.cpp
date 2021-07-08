#include "HmCppInvoke.h"

using namespace std;
using namespace Hidemaru;

THm::PFNGetDllFuncCalledType THmDllExport::Hidemaru_GetDllFuncCalledType = NULL;

void THm::initDllHandle(HMODULE handle) {
	SelfDllInfo::initHandle(handle);
}

std::vector<THmMacroVariable> THm::updateBindInformation(
	THmMacroVariable arg0,
	THmMacroVariable arg1,
	THmMacroVariable arg2,
	THmMacroVariable arg3,
	THmMacroVariable arg4,
	THmMacroVariable arg5,
	THmMacroVariable arg6,
	THmMacroVariable arg7,
	THmMacroVariable arg8
) {
	// これ重要。loadllの形態を保持する。関数が呼ばれる度に更新しておく。
	SelfDllInfo::setBindDllType();

	vector<THmMacroVariable> org_args = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 };
	vector<THmMacroVariable> dst_args;
	for (int ix = 0; ix < (int)org_args.size(); ix++) {
		THm::DLLFUNCPARAM ct = (THm::DLLFUNCPARAM)THm::Hidemaru_GetDllFuncCalledType(ix + 1);
		if (ct == THm::DLLFUNCPARAM::NOPARAM) {
			break;
		}
		else if (ct == THm::DLLFUNCPARAM::INT) {
			long num = std::get<long>(org_args[ix]);
			dst_args.push_back(num);
		}
		else if (ct == THm::DLLFUNCPARAM::WCHAR_PTR) {
			wstring wstr = std::get<std::wstring>(org_args[ix]);
			dst_args.push_back(wstr);
		}
	}

	return dst_args;
}
