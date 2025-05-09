#pragma once
/*
 * Hm.CppInvoke ver 2.125
 * Copyright (c) 2021-2025 Akitsugu Komiyama
 * under the MIT License
 */

#include <windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>
#include <variant>
#include <tuple>
#include <functional>
#include <set>
#include <optional>
#include <memory>

#include "HmCppInvokeConvertString.h"

namespace Hidemaru {
#ifdef FLOATMACRO_COMPILE
    /// <summary>
    /// 秀丸の数値型
    /// </summary>
    using THmNumber = double;
#else
    /// <summary>
    /// 秀丸の数値型
    /// </summary>
    using THmNumber = intptr_t;
#endif

    /// <summary>
    /// 秀丸マクロ変数の取りうる型。基本数値型か文字列型のみの想定。
    /// nullptr_t型は利用しないこと。
    /// </summary>
    using THmMacroVariable = std::variant<nullptr_t, THmNumber, std::wstring>;

    /// <summary>
    /// Macro系メソッドの返り値のメソッドに含まれるエラー型。
    /// std::nullopt ならエラーがなく、何らかのエラーがあればexception型が入っている。
    /// </summary>
    using THmMacroResultError = std::optional<std::exception>;

    /// <summary>
    /// Hm.Macro.Exec.doMethod(...) メソッドの第２引数である関数の型。
    /// </summary>
    using THmMacroDoMethodType = THmNumber (*)(std::wstring message_parameter);
}

namespace Hidemaru {

    /// <summary>
    /// Hmの基本となるクラス
    /// </summary>
    class THm {
    public:
        THm();

    private:
        // Hm をコピー禁止にしておく。
        THm(const THm& src) = delete;
        void operator =(const THm& src) = delete;

    private:
        static double hm_version;
        static double QueryFileVersion(wchar_t* path);
    public:
        /// <summary>
        /// 秀丸バージョンの取得
        /// </summary>
        /// <returns>秀丸バージョン</returns>
        double getVersion();
    private:
        bool setVersion();

    private:
        // 秀丸のウィンドウハンドル
        using PFNGetCurrentWindowHandle = HWND(WINAPI*)();
        static PFNGetCurrentWindowHandle Hidemaru_GetCurrentWindowHandle;
    private:
        static HMODULE hHideExeHandle;
    public:
        /// <summary>
        /// 呼ばれたプロセスの現在の秀丸エディタのウィンドウハンドルを返します。
        /// </summary>
        /// <returns>現在の秀丸エディタのウィンドウハンドル</returns>
        HWND getWindowHandle();

    private:
        static wchar_t wszHidemaruFullPath[MAX_PATH];
    public:
        /// <summary>
        /// 現在実行中の秀丸エディタ本体のフルパスを返します。
        /// </summary>
        /// <returns>現在実行中の秀丸エディタ本体のフルパス</returns>
        std::wstring getHidemaruFullPath();

    public:
        class TEdit;
#include "HmCppInvokeEdit.h"
    public:
        /// <summary>
        /// エディタ編集枠系。名前空間の代替。
        /// </summary>
        TEdit Edit;

    public:
        class TMacro;
#include "HmCppInvokeMacro.h"
    public:
        /// <summary>
        /// マクロ系。名前空間の代替。
        /// </summary>
        TMacro Macro;

    public:
        class TFile;
#include "HmCppInvokeFile.h"
    public:
        /// <summary>
        /// ファイル系。名前空間の代替。
        /// </summary>
        TFile File;

    public:
        class TOutputPane;
#include "HmCppInvokeOutputPane.h"

    public:
        /// <summary>
        /// アウトプット枠系。名前空間の代替。
        /// </summary>
        TOutputPane OutputPane;

    public:
        class TExplorerPane;
#include "HmCppInvokeExplorerPane.h"
    public:
        /// <summary>
        /// ファイルマネージャ枠系。名前空間の代替。
        /// </summary>
        TExplorerPane ExplorerPane;


    private:
        class TDllBindType;
#include "HmCppInvokeDllBindType.h"
    private:
        TDllBindType DllBindType;

    public:
        bool funcDllExport();

    public:
        class TDllFuncCalledType;
#include "HmCppInvokeDllFuncCalledType.h"
    public:
        TDllFuncCalledType DllFuncCalledType;
    };
};

namespace Hidemaru {
    extern Hidemaru::THm Hm;
}


