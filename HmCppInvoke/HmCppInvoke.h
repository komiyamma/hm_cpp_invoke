#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <any>



namespace Hidemaru {

    using MacroScopeMethodPointer = long (*)(std::wstring message_parameter);

    class THm {
    public:
        THm();
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

    private:
        // dllの呼ばれかた、関数の呼ばれ方(返り値の要求型や引数の要求型を判定できる関数)。強力
        using PFNGetDllFuncCalledType = int(WINAPI*)(int n);
        static PFNGetDllFuncCalledType Hidemaru_GetDllFuncCalledType;
        // 秀丸のウィンドウハンドル
        using PFNGetCurrentWindowHandle = HWND(WINAPI*)();
        static PFNGetCurrentWindowHandle Hidemaru_GetCurrentWindowHandle;

        // この結果のバイト列(vector.data())を HmOutputPane.dllのOutput関数になげれば、Unicodeでも再現できる
        std::vector<BYTE> EncodeWStringToOriginalEncodeVector(std::wstring original_string);

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
            public:
                int getLineNo();
                int getColumn();
                ICursorPos(int lineno, int column);
            };

            class IMousePos {
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
                std::any getException();
                std::wstring getMessage();
                IResult(int result, std::any error, std::wstring message);
            };

            IResult doEval(std::wstring expression);

            std::any getVar(std::wstring varname);
            bool setVar(std::wstring varname, std::any);

            class IFunctionResult {
            public:
                std::any getResult();
                std::vector<std::any> getArgs();
                std::any getException();
                std::wstring getMessage();
                IFunctionResult(std::any result, std::vector<std::any> args, std::any error, std::wstring message);
            };

            IFunctionResult doFunction(std::wstring func_name, std::any args0 = nullptr, std::any args1 = nullptr, std::any args2 = nullptr, std::any args3 = nullptr, std::any args4 = nullptr, std::any args5 = nullptr, std::any args6 = nullptr, std::any args7 = nullptr, std::any args8 = nullptr, std::any args9 = nullptr);

            class IStatementResult {
            public:
                int getResult();
                std::vector<std::any> getArgs();
                std::any getException();
                std::wstring getMessage();
                IStatementResult(int result, std::vector<std::any> args, std::any error, std::wstring message);
            };

            IStatementResult doStatement(std::wstring statement_name, std::any args0 = nullptr, std::any args1 = nullptr, std::any args2 = nullptr, std::any args3 = nullptr, std::any args4 = nullptr, std::any args5 = nullptr, std::any args6 = nullptr, std::any args7 = nullptr, std::any args8 = nullptr, std::any args9 = nullptr);

        public:
            class TExec {
            public:
                IResult doEval(std::wstring expression);
                IResult doFile(std::wstring filepath);
                IResult doMethod(std::wstring message_parameter, MacroScopeMethodPointer callback_method);
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


    public:
        class TOutputPane {
            static HMODULE hHmOutputPaneDLL;
            //-------------------------------------------------------------------------
            // アウトプットパネル
            //-------------------------------------------------------------------------
            using PFNHmOutputPane_Output = int(_cdecl*)(HWND hwnd, BYTE*);
            static PFNHmOutputPane_Output HmOutputPane_Output;
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
            using PFNHmExplorerPane_SetMode = int(_cdecl*)(HWND hwnd, intptr_t);
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
    };
};

extern Hidemaru::THm Hm;

