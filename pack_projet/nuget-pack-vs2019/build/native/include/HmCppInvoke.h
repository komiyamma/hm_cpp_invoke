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
    /// �G�ۂ̐��l�^
    /// </summary>
    using THmNumber = double;
#else
    /// <summary>
    /// �G�ۂ̐��l�^
    /// </summary>
    using THmNumber = intptr_t;
#endif

    /// <summary>
    /// �G�ۃ}�N���ϐ��̎�肤��^�B��{���l�^��������^�݂̂̑z��B
    /// nullptr_t�^�͗��p���Ȃ����ƁB
    /// </summary>
    using THmMacroVariable = std::variant<nullptr_t, THmNumber, std::wstring>;

    /// <summary>
    /// Macro�n���\�b�h�̕Ԃ�l�̃��\�b�h�Ɋ܂܂��G���[�^�B
    /// std::nullopt �Ȃ�G���[���Ȃ��A���炩�̃G���[�������exception�^�������Ă���B
    /// </summary>
    using THmMacroResultError = std::optional<std::exception>;

    /// <summary>
    /// Hm.Macro.Exec.doMethod(...) ���\�b�h�̑�Q�����ł���֐��̌^�B
    /// </summary>
    using THmMacroDoMethodType = THmNumber (*)(std::wstring message_parameter);
}

namespace Hidemaru {

    /// <summary>
    /// Hm�̊�{�ƂȂ�N���X
    /// </summary>
    class THm {
    public:
        THm();

    private:
        // Hm ���R�s�[�֎~�ɂ��Ă����B
        THm(const THm& src) = delete;
        void operator =(const THm& src) = delete;

    private:
        static double hm_version;
        static double QueryFileVersion(wchar_t* path);
    public:
        /// <summary>
        /// �G�ۃo�[�W�����̎擾
        /// </summary>
        /// <returns>�G�ۃo�[�W����</returns>
        double getVersion();
    private:
        bool setVersion();

    private:
        // �G�ۂ̃E�B���h�E�n���h��
        using PFNGetCurrentWindowHandle = HWND(WINAPI*)();
        static PFNGetCurrentWindowHandle Hidemaru_GetCurrentWindowHandle;
    private:
        static HMODULE hHideExeHandle;
    public:
        /// <summary>
        /// �Ă΂ꂽ�v���Z�X�̌��݂̏G�ۃG�f�B�^�̃E�B���h�E�n���h����Ԃ��܂��B
        /// </summary>
        /// <returns>���݂̏G�ۃG�f�B�^�̃E�B���h�E�n���h��</returns>
        HWND getWindowHandle();

    private:
        static wchar_t wszHidemaruFullPath[MAX_PATH];
    public:
        /// <summary>
        /// ���ݎ��s���̏G�ۃG�f�B�^�{�̂̃t���p�X��Ԃ��܂��B
        /// </summary>
        /// <returns>���ݎ��s���̏G�ۃG�f�B�^�{�̂̃t���p�X</returns>
        std::wstring getHidemaruFullPath();

    public:
        class TEdit;
#include "HmCppInvokeEdit.h"
    public:
        /// <summary>
        /// �G�f�B�^�ҏW�g�n�B���O��Ԃ̑�ցB
        /// </summary>
        TEdit Edit;

    public:
        class TMacro;
#include "HmCppInvokeMacro.h"
    public:
        /// <summary>
        /// �}�N���n�B���O��Ԃ̑�ցB
        /// </summary>
        TMacro Macro;

    public:
        class TFile;
#include "HmCppInvokeFile.h"
    public:
        /// <summary>
        /// �t�@�C���n�B���O��Ԃ̑�ցB
        /// </summary>
        TFile File;

    public:
        class TOutputPane;
#include "HmCppInvokeOutputPane.h"

    public:
        /// <summary>
        /// �A�E�g�v�b�g�g�n�B���O��Ԃ̑�ցB
        /// </summary>
        TOutputPane OutputPane;

    public:
        class TExplorerPane;
#include "HmCppInvokeExplorerPane.h"
    public:
        /// <summary>
        /// �t�@�C���}�l�[�W���g�n�B���O��Ԃ̑�ցB
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


