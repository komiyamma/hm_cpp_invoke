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
