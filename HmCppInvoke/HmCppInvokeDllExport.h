#pragma once

#include <windows.h>

namespace Hidemaru {
    class THmDllExport {
    public:
        // dllの呼ばれかた、関数の呼ばれ方(返り値の要求型や引数の要求型を判定できる関数)。強力
        using PFNGetDllFuncCalledType = int(WINAPI*)(int n);
        static PFNGetDllFuncCalledType Hidemaru_GetDllFuncCalledType;

        //-------------------------------------------------------------------------
        // nに - 1を指定すると、loaddllのされ方を返します。1以上でloaddll関数の返り値と同じです。 - 1の場合はloaddll文で読み込まれたDLLです。

        //-------------------------------------------------------------------------
        // nに0を指定すると、dllfunc / dllfuncw / dllfuncstr / dllfuncstrwのどれで呼ばれたかによって、returnで返すべき値を示します。
        // 以下の値のいずれかが返ります。
        // dllfunc等が呼ばれているとき、どのような呼ばれ方をしているかを取得します。
        enum class DLLFUNCRETURN {
            NOTCALL = 0,       //呼ばれていない
            INT = 0x0001,  //intを返すべき(dllfuncまたはdllfuncw)
            CHAR_PTR = 0x0002,  //char*を返すべき(dllfuncstr)
            WCHAR_PTR = 0x0003,  //WCHAR*を返すべき(dllfuncstrw)
            DOUBLE = 0x0004  //doubleを返すべき(dllfuncまたはdllfuncwで浮動小数点数が有効)
        };

        //-------------------------------------------------------------------------
        //nに1から数えた順番の値を指定すると、1から数えたパラメータの種類が数値 / 文字列 / Unicode文字列 / 浮動小数点数かを返します。
        // パラメータの最大は64個です。
        // 以下の値のいずれかが返ります。
        enum class DLLFUNCPARAM {
            NOPARAM = 0,       //以降のパラメータなし
            INT = 0x0100,  //intのパラメータ
            CHAR_PTR = 0x0200,  //char*のパラメータ
            WCHAR_PTR = 0x0300,  //WCHAR*のパラメータ
            DOUBLE = 0x0400   //doubleのパラメータ
        };
    };

#pragma pack(1)
    union THmDllParamNumber {
        BYTE bin[64];
        long nint;
        wchar_t* pwstr;
        double f64;

        inline operator double() {
            return f64;
        }

        inline operator long() {
            return nint;
        }

        inline operator wchar_t* () {
            return pwstr;
        }

        inline operator std::wstring() {
            return pwstr;
        }
    };
#pragma pack()

    using THmDllParamVariable = std::variant<nullptr_t, THmDllParamNumber, std::wstring>;
}

