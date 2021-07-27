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

