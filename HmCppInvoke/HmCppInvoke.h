#pragma once

#include <windows.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <any>



namespace Hidemaru {

    using MacroScopeMethodPointer = long (*)(std::wstring message_parameter);

    class THm {
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
            static void InitializeHandle(HMODULE handle);
        private:
            static int GetBindDllType();
            static BOOL SetBindDllHandle();
            static std::wstring GetInvocantString();
            static std::wstring GetSelfModuleFullPath();
            static std::wstring GetSelfModuleDir();
        };

        public:
            void InitializeSelfDllHandle(HMODULE handle) {
                SelfDllInfo::InitializeHandle(handle);
            }
    };

    extern std::any TestDynamicVar;
};



extern Hidemaru::THm Hm;


#pragma once


