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
#include <variant>
#include <functional>
#include <set>

#include "HmCppInvokeDllExport.h"


namespace Hidemaru {
    using THmMacroVariable = std::variant<nullptr_t, long, std::wstring>;
    using THmMacroResultError = std::variant<nullptr_t, std::exception>;

    class THm : public THmDllExport {
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
        static HMODULE hHideExeHandle;
    public:
        HWND getWindowHandle();
    private:
        static wchar_t szHidemaruFullPath[MAX_PATH];
    private:
        bool setVersion();

    public:
        // 秀丸のウィンドウハンドル
        using PFNGetCurrentWindowHandle = HWND(WINAPI*)();
        static PFNGetCurrentWindowHandle Hidemaru_GetCurrentWindowHandle;

    public:
        class TEdit {
            // 現在編集中の全てのテキストを得る
            using PFNGetTotalTextUnicode = HGLOBAL(WINAPI*)(void);
            static PFNGetTotalTextUnicode Hidemaru_GetTotalTextUnicode;
            // 現在編集中の選択テキストを得る(単純選択のみ)
            using PFNGetSelectedTextUnicode = HGLOBAL(WINAPI*)(void);
            static PFNGetSelectedTextUnicode Hidemaru_GetSelectedTextUnicode;
            // 現在編集中のテキストのカーソルがある行のテキストを得る
            using PFNGetLineTextUnicode = HGLOBAL(WINAPI*)(int nLineNo);
            static PFNGetLineTextUnicode Hidemaru_GetLineTextUnicode;
            // 現在編集中のテキストのカーソルの位置を取得する。マクロのcolumnとlineno相当(厳密には異なる)
            using PFNGetCursorPosUnicode = BOOL(WINAPI*)(int* pnLineNo, int* pnColumn);
            static PFNGetCursorPosUnicode Hidemaru_GetCursorPosUnicode;
            // 現在編集中のテキストのマウスの位置に対応するカーソルの位置を取得する。
            using PFNGetCursorPosUnicodeFromMousePos = BOOL(WINAPI*)(POINT* ppt, int* pnLineNo, int* pnColumn);
            static PFNGetCursorPosUnicodeFromMousePos Hidemaru_GetCursorPosUnicodeFromMousePos;
            // キー入力があるなどの理由で処理を中断するべきかを返します。
            using PFNCheckQueueStatus = BOOL(WINAPI*)();
            static PFNCheckQueueStatus Hidemaru_CheckQueueStatus;

        public:
            TEdit();
        public:
            std::wstring getFilePath();
            std::wstring getTotalText();
            bool setTotalText(std::wstring text);
            std::wstring getSelectedText();
            bool setSelectedText(std::wstring text);
            std::wstring getLineText();
            bool setLineText(std::wstring text);
            bool isQueueStatus();

        public:
            class ICursorPos {
                int lineno;
                int column;
            public:
                int getLineNo();
                int getColumn();
                ICursorPos(int lineno, int column);
            };

            class IMousePos {
                int lineno;
                int column;
                int x;
                int y;
            public:
                int getLineNo();
                int getColumn();
                int getX();
                int getY();
                IMousePos(int lineno, int column, int x, int y);
            };

            ICursorPos getCursorPos();
            IMousePos getMousePos();
        };

    public:
        TEdit Edit;

    public:
        class TMacro {
            // dllの中から秀丸マクロを実行する
            using PFNEvalMacro = BOOL(WINAPI*)(const wchar_t* pwsz);
            static PFNEvalMacro Hidemaru_EvalMacro;
        public:
            TMacro();
        public:
            bool isExecuting();

        public:
            class IResult {
                long result;
                THmMacroResultError error;
                std::wstring message;
            public:
                long getResult();
                THmMacroResultError getException();
                std::wstring getMessage();
                IResult(long result, THmMacroResultError error, std::wstring message);
            };

            IResult doEval(std::wstring expression);

            THmMacroVariable getVar(std::wstring varname);
            bool setVar(std::wstring varname, THmMacroVariable value);

            class IFunctionResult {
                THmMacroVariable result;
                THmMacroResultError error;
                std::wstring message;
                std::vector<THmMacroVariable> args;
            public:
                THmMacroVariable getResult();
                std::vector<THmMacroVariable> getArgs();
                THmMacroResultError getException();
                std::wstring getMessage();
                IFunctionResult(THmMacroVariable result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
            };

            IFunctionResult doFunction(std::wstring func_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

            class IStatementResult {
                long result;
                THmMacroResultError error;
                std::wstring message;
                std::vector<THmMacroVariable> args;
            public:
                long getResult();
                std::vector<THmMacroVariable> getArgs();
                THmMacroResultError getException();
                std::wstring getMessage();
                IStatementResult(long result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
            };

            IStatementResult doStatement(std::wstring statement_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

        public:
            class TExec {
            public:
                IResult doEval(std::wstring expression);
                IResult doFile(std::wstring filepath);
                IResult doMethod(std::wstring message_parameter, std::function<long(std::wstring)> callback_method);
            };

            TExec Exec;
        };

    public:
        TMacro Macro;


    public:
        class TFile {
            // 何のエンコードで開かれるのかを取得する機能となる。
            using PFNAnalyzeEncoding = int(WINAPI*)(const WCHAR* pwszFileName, DWORD_PTR lParam1, DWORD_PTR lParam2);
            static PFNAnalyzeEncoding Hidemaru_AnalyzeEncoding;

            // 指定の秀丸のencodeを指定して、ファイル内容を読み込む
            using PFNLoadFileUnicode = HGLOBAL(WINAPI*)(const WCHAR* pwszFileName, int nEncode, UINT* pcwchOut, DWORD_PTR lParam1, DWORD_PTR lParam2);
            static PFNLoadFileUnicode Hidemaru_LoadFileUnicode;
        public:
            TFile();

            class IEncoding {
                int hm_encode;
                int ms_codepage;
            public:
                IEncoding(int hm_encode, int ms_codepage);
                IEncoding() = default;
                IEncoding(const IEncoding&) = default;
                int getHmEncode();
                int getMsCodePage();
            };

            IEncoding getEncoding(int hm_encode);
            IEncoding getEncoding(std::wstring filepath);
            const std::vector<int> getEncodingTable();

            class IHidemaruStreamReader {
                IEncoding encoding;
                std::wstring filepath;
            public:
                IHidemaruStreamReader(std::wstring filepath, IEncoding encoding);
                IEncoding getEncoding();
                std::wstring read();
                std::wstring getFilePath();
                void close();
            };

            IHidemaruStreamReader open(std::wstring filepath, int hm_encode=-1);
        };

    public:
        TFile File;


    private:
        // この結果のバイト列(vector.data())を HmOutputPane.dllのOutput関数になげれば、Unicodeでも再現できる
        static std::vector<BYTE> EncodeWStringToOriginalEncodeVector(std::wstring original_string);

    public:
        class TOutputPane {
            static HMODULE hHmOutputPaneDLL;
            //-------------------------------------------------------------------------
            // アウトプットパネル
            //-------------------------------------------------------------------------
            using PFNHmOutputPane_Output = int(_cdecl*)(HWND hwnd, BYTE*);
            static PFNHmOutputPane_Output HmOutputPane_Output;
            using PFNHmOutputPane_OutputW = int(_cdecl*)(HWND hwnd, wchar_t* message);
            static PFNHmOutputPane_OutputW HmOutputPane_OutputW;
            using PFNHmOutputPane_Push = int(_cdecl*)(HWND hwnd);
            static PFNHmOutputPane_Push HmOutputPane_Push;
            using PFNHmOutputPane_Pop = int(_cdecl*)(HWND hwnd);
            static PFNHmOutputPane_Pop HmOutputPane_Pop;
            using PFNHmOutputPane_GetWindowHandle = HWND(_cdecl*)(HWND hwnd);
            static PFNHmOutputPane_GetWindowHandle HmOutputPane_GetWindowHandle;
            using PFNHmOutputPane_SetBaseDir = int(_cdecl*)(HWND hwnd, BYTE*);
            static PFNHmOutputPane_SetBaseDir HmOutputPane_SetBaseDir;
        public:
            TOutputPane();
        public:
            bool output(std::wstring message);
            bool push();
            bool pop();
            bool clear();
            long sendMessage(int command_id);
            bool setBaseDir(std::wstring dirpath);
            HWND getWindowHandle();
        };

    public:
        TOutputPane OutputPane;

    public:
        class TExplorerPane {
            static HMODULE hHmExplorerPaneDLL;
            //-------------------------------------------------------------------------
            // ファイルマネージャパネル
            //-------------------------------------------------------------------------
            using PFNHmExplorerPane_SetMode = int(_cdecl*)(HWND hwnd, int mode);
            static PFNHmExplorerPane_SetMode HmExplorerPane_SetMode;
            using PFNHmExplorerPane_GetMode = int(_cdecl*)(HWND hwnd);
            static PFNHmExplorerPane_GetMode HmExplorerPane_GetMode;
            using PFNHmExplorerPane_LoadProject = int(_cdecl*)(HWND hwnd, BYTE*);
            static PFNHmExplorerPane_LoadProject HmExplorerPane_LoadProject;
            using PFNHmExplorerPane_SaveProject = int(_cdecl*)(HWND hwnd, BYTE*);
            static PFNHmExplorerPane_SaveProject HmExplorerPane_SaveProject;
            using PFNHmExplorerPane_GetProject = BYTE * (_cdecl*)(HWND hwnd);
            static PFNHmExplorerPane_GetProject HmExplorerPane_GetProject;
            using PFNHmExplorerPane_GetWindowHandle = HWND(_cdecl*)(HWND hwnd);
            static PFNHmExplorerPane_GetWindowHandle HmExplorerPane_GetWindowHandle;
            using PFNHmExplorerPane_GetUpdated = int(_cdecl*)(HWND hwnd);
            static PFNHmExplorerPane_GetUpdated HmExplorerPane_GetUpdated;
            using PFNHmExplorerPane_GetCurrentDir = BYTE * (_cdecl*)(HWND hwnd);
            static PFNHmExplorerPane_GetCurrentDir HmExplorerPane_GetCurrentDir;
        public:
            TExplorerPane();
        public:
            bool setMode(int mode);
            int getMode();
            bool loadProject(std::wstring filepath);
            bool saveProject(std::wstring filepath);
            std::wstring getProject();
            long sendMessage(int command_id);
            HWND getWindowHandle();
            bool getUpdated();
        };

    public:
        TExplorerPane ExplorerPane;

        //-------------------------------------------------------------------------
        // dll自身のハンドルやフスパスの情報の保持
        //-------------------------------------------------------------------------
    private:
        class TSelfDllAttribute {

            //-------------------------------------------------------------------------
            // 自分自身(このdll)のモジュールインスタンスハンドル
            static HMODULE hModule;

            //-------------------------------------------------------------------------
            // 自分自身(このdll)のフルパス
            static wchar_t szSelfModuleFullPath[MAX_PATH];

            // 自分自身(このdll)のディレクトリのパス
            static wchar_t szSelfModuleDirPath[MAX_PATH];

            //-------------------------------------------------------------------------
            // このdllが秀丸マクロからどのような形でloaddllされたのかの情報。
            // この情報があれば、dll内部からマクロを発行することが出来る。
            // -1   :loaddll文で束縛だれた
            // 0    :読めていない。(読めてなかったらdll実行されてないので、これはあり得ない)
            // 1以上:その数値で秀丸マクロ上で束縛されている。
            //-------------------------------------------------------------------------
        public:
            TSelfDllAttribute();
        private:
            static int iSelfBindedType;
        private:
            static void initHandle();
            static HMODULE getCurrentModule();
        public:
            static int getBindDllType();
            static BOOL setBindDllType();
            static std::wstring getInvocantString();
            static std::wstring getSelfModuleFullPath();
            static std::wstring getSelfModuleDir();
        };

    private:
        TSelfDllAttribute SelfDllInfo;

    public:
        bool rebindParameters();
    };

    extern THmMacroVariable TestDynamicVar;
};


// 秀丸マクロ上の変数と、C++内部との自動やりとりに使用される。
HM_DLLEXPORT long SetDynamicVar(const void* dynamic_value);
HM_DLLEXPORT long PopNumVar();
HM_DLLEXPORT long PushNumVar(long i_tmp_num);
HM_DLLEXPORT const wchar_t* PopStrVar();
HM_DLLEXPORT long PushStrVar(const wchar_t* sz_tmp_str);




extern Hidemaru::THm Hm;

#pragma once


