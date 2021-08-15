/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

/// <summary>
/// dllや関数の呼ばれ方のクラス
/// </summary>
class TDllFuncCalledType {
public:
    TDllFuncCalledType();

private:
    using PFNGetDllFuncCalledType = int(WINAPI*)(int n);
    /// <summary>
    /// dllの呼ばれかた、関数の呼ばれ方(返り値の要求型や引数の要求型を判定できる関数)。強力
    /// <para>nに-1を指定すると、loaddllのされ方を返します。1以上でloaddll関数の返り値と同じです。 -1の場合はloaddll文で読み込まれたDLLです。</para>
    /// </summary>
    static PFNGetDllFuncCalledType Hidemaru_GetDllFuncCalledType;

public:
    /// <summary>
    /// 秀丸のバージョンが 8.66正式版未満だと、メソッドがない。この判定のためのメソッド。
    /// </summary>
    static bool isMissingMethod();

    /// <summary>
    /// このdllが秀丸マクロ上の数値ハンドル(#dllなど)で何番に割り当てられているかの数。-1なら、loaddll関数ではなく、loaddll文でbindされている。。
    /// </summary>
    static int getDllBindType();

public:

    enum class DLLFUNCPARAM {
        NOPARAM = 0,        // 以降のパラメータなし
        INT = 0x0100,       // intptr_tのパラメータ
        CHAR_PTR = 0x0200,  // char*のパラメータ
        WCHAR_PTR = 0x0300, // wchar_t*のパラメータ
        DOUBLE = 0x0400     // doubleのパラメータ
    };

    /// <summary>
    /// arg_nに1から数えた順番の値を指定すると、1から数えた引数の型の種類が「数値 / 文字列(char*) / Unicode文字列(wchar_t*) / 浮動小数点数」かを返します。
    /// <para>パラメータ(引数)の最大は64個です。</para> 
    /// <para>以下の値のいずれかが返ります。</para>
    /// </summary>
    /// <returns>「enumのDLLFUNCPARAM」値のいずれか</returns>
    static DLLFUNCPARAM getFuncParamType(int arg_n);

    /// <summary>
    /// マクロからdll関数の呼び出しの引数のタイプリストを得る
    /// </summary>
    /// <returns>引数の順番通りの「enumのDLLFUNCPARAM」値のリスト</returns>
    static std::vector<DLLFUNCPARAM> getFuncParamTypeList();

    enum class DLLFUNCRETURN {
        NOTCALL = 0,        // 呼ばれていない
        INT = 0x0001,       // intを返すべき(dllfuncまたはdllfuncw)
        CHAR_PTR = 0x0002,  // char*を返すべき(dllfuncstr)
        WCHAR_PTR = 0x0003, // WCHAR*を返すべき(dllfuncstrw)
        DOUBLE = 0x0004     // doubleを返すべき(dllfuncまたはdllfuncwで浮動小数点数が有効)
    };

    /// <summary>
    /// dllfunc / dllfuncw / dllfuncstr / dllfuncstrwのどれで呼ばれたかによって、returnで返すべき値を示します。 
    /// <para>以下の値のいずれかが返ります。</para> 
    /// <para>dllfunc等が呼ばれているとき、どのような呼ばれ方をしているかを取得します。</para>
    /// </summary>
    /// <returns>「enumのDLLFUNCRETURN」値のいずれか</returns>
    static DLLFUNCRETURN getFuncReturnType();

};
