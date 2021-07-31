/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


class TDllMacroVariant {
public:
    TDllMacroVariant();

public:
    using PFNGetDllFuncCalledType = int(WINAPI*)(int n);
    /// <summary>
    /// dllの呼ばれかた、関数の呼ばれ方(返り値の要求型や引数の要求型を判定できる関数)。強力
    /// <para>nに-1を指定すると、loaddllのされ方を返します。1以上でloaddll関数の返り値と同じです。 -1の場合はloaddll文で読み込まれたDLLです。</para>
    /// </summary>
    static PFNGetDllFuncCalledType Hidemaru_GetDllFuncCalledType;

public:
    /// <summary>
    /// nに0を指定すると、dllfunc / dllfuncw / dllfuncstr / dllfuncstrwのどれで呼ばれたかによって、returnで返すべき値を示します。 
    /// <para>以下の値のいずれかが返ります。</para> 
    /// <para>dllfunc等が呼ばれているとき、どのような呼ばれ方をしているかを取得します。</para>
    /// </summary>
    enum class DLLFUNCRETURN {
        NOTCALL = 0,       //呼ばれていない
        INT = 0x0001,  //intを返すべき(dllfuncまたはdllfuncw)
        CHAR_PTR = 0x0002,  //char*を返すべき(dllfuncstr)
        WCHAR_PTR = 0x0003,  //WCHAR*を返すべき(dllfuncstrw)
        DOUBLE = 0x0004  //doubleを返すべき(dllfuncまたはdllfuncwで浮動小数点数が有効)
    };

    /// <summary>
    /// nに1から数えた順番の値を指定すると、1から数えたパラメータの種類が数値 / 文字列(char*) / Unicode文字列(wchar_t*) / 浮動小数点数かを返します。
    /// <para>パラメータ(引数)の最大は64個です。</para> 
    /// <para>以下の値のいずれかが返ります。</para>
    /// </summary>
    enum class DLLFUNCPARAM {
        NOPARAM = 0,       //以降のパラメータなし
        INT = 0x0100,  //intのパラメータ
        CHAR_PTR = 0x0200,  //char*のパラメータ
        WCHAR_PTR = 0x0300,  //WCHAR*のパラメータ
        DOUBLE = 0x0400   //doubleのパラメータ
    };
};
