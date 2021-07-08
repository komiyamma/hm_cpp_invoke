#pragma once

#include <windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <variant>

#include "HmCppInvokeDllExport.h"


namespace Hidemaru {
#ifdef USE_FLOATMACRO
    using THmMacroNumber = double;
#else
    using THmMacroNumber = long;
#endif
    using THmMacroVariable = std::variant<nullptr_t, THmMacroNumber, std::wstring>;
    using THmMacroResultError = std::variant<nullptr_t, std::exception>;

    using THmMacroScopeMethodPointer = THmMacroNumber (*)(std::wstring message_parameter);

    class THm : public THmDllExport {
    public:
        THm();
    // Hm をコピー禁止にしておく。
    private:
        THm(const THm& src) {}
        void operator =(const THm& src);
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
            public:
                int getResult();
                THmMacroResultError getException();
                std::wstring getMessage();
                IResult(int result, THmMacroResultError error, std::wstring message);
            };

            IResult doEval(std::wstring expression);

            THmMacroVariable getVar(std::wstring varname);
            bool setVar(std::wstring varname, THmMacroVariable value);

            class IFunctionResult {
            public:
                THmMacroVariable getResult();
                std::vector<THmMacroVariable> getArgs();
                THmMacroResultError getException();
                std::wstring getMessage();
                IFunctionResult(THmMacroVariable result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
            };

            IFunctionResult doFunction(std::wstring func_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

            class IStatementResult {
            public:
                int getResult();
                std::vector<THmMacroVariable> getArgs();
                THmMacroResultError getException();
                std::wstring getMessage();
                IStatementResult(int result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
            };

            IStatementResult doStatement(std::wstring statement_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

        public:
            class TExec {
            public:
                IResult doEval(std::wstring expression);
                IResult doFile(std::wstring filepath);
                IResult doMethod(std::wstring message_parameter, THmMacroScopeMethodPointer callback_method);
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
            public:
                int getHmEncode();
                int getMsCodePage();
            };

            IEncoding getEncoding();

            class IHidemaruStreamReader {
            public:
                IEncoding getEncoding();
                std::wstring read();
                std::wstring getFilePath();
                void close();
            };

            IHidemaruStreamReader open(std::wstring filepath, int hm_encode = -1);
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
            using PFNHmOutputPane_OutputW = int(_cdecl*)(HWND hwnd, wchar_t *message);
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
        class SelfDllInfo {

            //-------------------------------------------------------------------------
            // 自分自身(hmPerl.dll)のモジュールインスタンスハンドル
            static HMODULE hModule;

            //-------------------------------------------------------------------------
            // 自分自身(hmPerl.dll)のフルパス
            static wchar_t szSelfModuleFullPath[MAX_PATH];

            static wchar_t szSelfModuleDirPath[MAX_PATH];

            //-------------------------------------------------------------------------
            // このdllが秀丸マクロからどのような形でloaddllされたのかの情報。
            // この情報があれば、dll内部からマクロを発行することが出来る。
            // -1   :loaddll文で束縛だれた
            // 0    :読めていない。(読めてなかったらdll実行されてないので、これはあり得ない)
            // 1以上:その数値で秀丸マクロ上で束縛されている。
            //-------------------------------------------------------------------------
        private:
            static int iSelfBindedType;
        public:
            static void initHandle(HMODULE handle);
        public:
            static int getBindDllType();
            static BOOL setBindDllType();
            static std::wstring getInvocantString();
            static std::wstring getSelfModuleFullPath();
            static std::wstring getSelfModuleDir();
        };

    public:
        void initDllHandle(HMODULE handle);

        std::vector<THmMacroVariable> updateBindInformation(
            THmDllParamVariable arg0 = nullptr,
            THmDllParamVariable arg1 = nullptr,
            THmDllParamVariable arg2 = nullptr,
            THmDllParamVariable arg3 = nullptr,
            THmDllParamVariable arg4 = nullptr,
            THmDllParamVariable arg5 = nullptr,
            THmDllParamVariable arg6 = nullptr,
            THmDllParamVariable arg7 = nullptr,
            THmDllParamVariable arg8 = nullptr
        );
    };

    extern THmMacroVariable TestDynamicVar;
};



extern Hidemaru::THm Hm;


#pragma once


