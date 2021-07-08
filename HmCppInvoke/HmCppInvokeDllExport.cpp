#include "HmCppInvoke.h"


using namespace std;
using namespace Hidemaru;

THm::PFNGetDllFuncCalledType THmDllExport::Hidemaru_GetDllFuncCalledType = NULL;

#ifdef USE_FLOATMACRO
extern "C" __declspec(dllexport) FLOATMACRO {}
#else

#endif
