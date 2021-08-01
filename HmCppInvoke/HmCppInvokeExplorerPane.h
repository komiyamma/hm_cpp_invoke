/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


//-------------------------------------------------------------------------
// ファイルマネージャパネル
//-------------------------------------------------------------------------
class TExplorerPane {
    static HMODULE hHmExplorerPaneDLL;

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

    /// <summary>
    /// ファイルマネージャ枠のモードの設定
    /// </summary>
    /// <param name = "mode">新たなモード値</param>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool setMode(int mode);

    /// <summary>
    /// ファイルマネージャ枠のモードの取得
    /// </summary>
    /// <returns>モードの値</returns>
    int getMode();

    /// <summary>
    /// ファイルマネージャ枠に指定のファイルのプロジェクトを読み込む
    /// </summary>
    /// <param name = "filepath">読み込み対象のプロジェクトのファイルのフルパス</param>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool loadProject(std::wstring filepath);

    /// <summary>
    /// ファイルマネージャ枠のプロジェクトを指定ファイルに保存
    /// </summary>
    /// <param name = "filepath">保存対象のプロジェクトのファイルのフルパス</param>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool saveProject(std::wstring filepath);

    /// <summary>
    /// ファイルマネージャ枠にプロジェクトを読み込んでいるならば、そのファイルパスを取得する
    /// </summary>
    /// <returns>ファイルのフルパス。読み込んでいなければ空文字</returns>
    std::wstring getProject();

    /// <summary>
    /// ファイルマネージャ枠へのSendMessage
    /// <para>「コマンド値」はhttps://help.maruo.co.jp/hidemac/html/240_PaneDll.html</para>
    /// </summary>
    /// <param name = "command_id">コマンド値</param>
    /// <returns>SendMessageの返り値(LRESULT型)と同じ</returns>
    THmNumber sendMessage(int command_id);

    /// <summary>
    /// ファイルマネージャ枠のWindowHandle
    /// </summary>
    /// <returns>ファイルマネージャ枠のWindowHandle</returns>
    HWND getWindowHandle();

    /// <summary>
    /// ファイルマネージャ枠が「プロジェクト」表示のとき、更新された状態であるかどうかを返します
    /// </summary>
    /// <returns>更新状態なら1、それ以外は0</returns>
    bool getUpdated();
};
