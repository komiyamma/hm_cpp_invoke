#include "HmCppInvoke.h"

using namespace std;
using namespace Hidemaru;

THm::PFNGetDllFuncCalledType THmDllExport::Hidemaru_GetDllFuncCalledType = NULL;

#ifdef USE_FLOATMACRO
extern "C" __declspec(dllexport) void FLOATMACRO() {}
#else

#endif


void THm::initDllHandle(HMODULE handle) {
	SelfDllInfo::initHandle(handle);
}

std::vector<THmMacroVariable> THm::updateBindInformation(
	THmDllParamVariable arg0,
	THmDllParamVariable arg1,
	THmDllParamVariable arg2,
	THmDllParamVariable arg3,
	THmDllParamVariable arg4,
	THmDllParamVariable arg5,
	THmDllParamVariable arg6,
	THmDllParamVariable arg7,
	THmDllParamVariable arg8
) {
	// これ重要。loadllの形態を保持する。関数が呼ばれる度に更新しておく。
	SelfDllInfo::setBindDllType();

	vector<THmDllParamVariable> org_args = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 };
	vector<THmMacroVariable> dst_args;
	for (int ix = 0; ix < (int)org_args.size(); ix++) {
		THm::DLLFUNCPARAM ct = (THm::DLLFUNCPARAM)THm::Hidemaru_GetDllFuncCalledType(ix);
		if (ct == THm::DLLFUNCPARAM::NOPARAM) {
			break;
		}
#ifdef USE_FLOATMACRO
		else if (ct == THm::DLLFUNCPARAM::DOUBLE) {
			THmDllParamNumber num = std::get<THmDllParamNumber>(org_args[ix]);
			dst_args.push_back((double)num);
		}
#else
		else if (ct == THm::DLLFUNCPARAM::INT) {
			THmDllParamNumber num = std::get<THmDllParamNumber>(org_args[ix]);
			dst_args.push_back((long)num);
		}
#endif
		else if (ct == THm::DLLFUNCPARAM::WCHAR_PTR) {
			wstring wstr = std::get<std::wstring>(org_args[ix]);
			dst_args.push_back(wstr);
		}
	}

	return dst_args;
}
