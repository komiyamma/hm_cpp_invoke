/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

 /// <summary>
 /// アウトプット枠
 /// </summary>
class TOutputPane {
    static HMODULE hHmOutputPaneDLL;
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
    /// <summary>
    /// アウトプット枠への文字列の出力。
    /// 改行するには「\r\n」といったように「\r」も必要。
    /// </summary>
    /// <param name = "message">アウトプット枠へと表示するテキスト内容</param>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool output(std::wstring message);

    /// <summary>
    /// アウトプット枠にある文字列の一時退避
    /// </summary>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool push();

    /// <summary>
    /// Pushによって一時退避した文字列の復元
    /// </summary>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool pop();

    /// <summary>
    /// アウトプット枠にある文字列のクリア
    /// </summary>
    /// <returns>現在のところ、成否を指し示す値は返ってこない</returns>
    bool clear();

    /// <summary>
    /// アウトプット枠へのSendMessage
    /// <para>「コマンド値」はhttps://help.maruo.co.jp/hidemac/html/240_PaneDll.html</para>
    /// </summary>
    /// <param name = "command_id">コマンド値</param>
    /// <returns>SendMessageの返り値(LRESULT型)と同じ</returns>
    THmNumber sendMessage(int command_id);

    /// <summary>
    /// アウトプット枠のベースとなるディレクトリの設定
    /// </summary>
    /// <param name = "dirpath">ディレクトリのパス</param>
    /// <returns>失敗ならfalse、成功ならtrue</returns>
    bool setBaseDir(std::wstring dirpath);

    /// <summary>
    /// アウトプット枠のWindowHandle
    /// </summary>
    /// <returns>アウトプット枠のWindowHandle</returns>
    HWND getWindowHandle();
};

