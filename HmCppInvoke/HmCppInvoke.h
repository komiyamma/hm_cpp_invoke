#pragma once
/*
 * Copyright (c) 2021 Akitsugu Komiyama
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

namespace Hidemaru {
    using THmMacroVariable = std::variant<nullptr_t, long, std::wstring>;
    using THmMacroResultError = std::optional<std::exception>;
}

namespace Hidemaru {
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
        HWND getWindowHandle();

    private:
        static wchar_t wszHidemaruFullPath[MAX_PATH];
    public:
        std::wstring getHidemaruFullPath();

    public:
        class TEdit;
#include "HmCppInvokeEdit.hpp"
    public:
        TEdit Edit;

    public:
        class TMacro;
#include "HmCppInvokeMacro.hpp"
    public:
        TMacro Macro;

    public:
        class TFile;
#include "HmCppInvokeFile.hpp"
    public:
        TFile File;

    public:
        class TOutputPane;
#include "HmCppInvokeOutputPane.hpp"
         
    public:
        TOutputPane OutputPane;

    public:
        class TExplorerPane;
#include "HmCppInvokeExplorerPane.hpp"
    public:
        TExplorerPane ExplorerPane;


    private:
        class TDllBindAttribute;
#include "HmCppInvokeDllBindAttribute.hpp"
    private:
        TDllBindAttribute DllBindAttribute;

    public:
        bool rebindParameters();

    public:
        class TDllMacroVariant;
#include "HmCppInvokeDllmacroVariant.hpp"
    private:
        TDllMacroVariant DllMacroVariant;
    };
};

extern Hidemaru::THm Hm;


