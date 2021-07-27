/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


#pragma once

#include <windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>
#include <variant>
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
#include "HmCppInvokeEdit.h"
    public:
        TEdit Edit;

    public:
        class TMacro;
#include "HmCppInvokeMacro.h"
    public:
        TMacro Macro;

    public:
        class TFile;
#include "HmCppInvokeFile.h"
    public:
        TFile File;

    public:
        class TOutputPane;
#include "HmCppInvokeOutputPane.h"
         
    public:
        TOutputPane OutputPane;

    public:
        class TExplorerPane;
#include "HmCppInvokeExplorerPane.h"
    public:
        TExplorerPane ExplorerPane;


    private:
        class TDllBindAttribute;
#include "HmCppInvokeDllBindAttribute.h"
    private:
        TDllBindAttribute DllBindAttribute;

    public:
        bool rebindParameters();

    public:
        class TDllMacroVariant;
#include "HmCppInvokeDllmacroVariant.h"
    private:
        TDllMacroVariant DllMacroVariant;
    };
};

extern Hidemaru::THm Hm;


