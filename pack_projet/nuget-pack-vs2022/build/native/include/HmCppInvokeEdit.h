/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

/// <summary>
/// エディタ枠
/// </summary>
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
    /// <summary>
    /// 現在アクティブな編集ファイル名を返す。無題なら空文字を返す。
    /// </summary>
    /// <returns>ファイル名。無題なら空文字。</returns>
    std::wstring getFilePath();

    /// <summary>
    /// マクロのupdatecount相当の値を返す。
    /// </summary>
    /// <returns>updatecount相当の値。</returns>
    int getUpdateCount();

    /// <summary>
    /// 現在アクティブな編集領域のテキスト全体を返す。
    /// </summary>
    /// <returns>編集領域のテキスト全体</returns>
    std::wstring getTotalText();

    /// <summary>
    /// 現在アクティブな編集領域のテキスト内容を指定の文字列とする。
    /// </summary>
    /// <param name = "text">新たなテキスト内容</param>
    /// <returns>入れ替えの成否</returns>
    bool setTotalText(std::wstring text);

    /// <summary>
    /// 現在、単純選択している場合、その選択中のテキスト内容を返す。
    /// </summary>
    /// <returns>選択中のテキスト内容</returns>
    std::wstring getSelectedText();

    /// <summary>
    /// 現在、単純選択している場合、その選択中のテキスト内容を指定の文字列とする。
    /// </summary>
    /// <param name = "text">新たなテキスト内容</param>
    /// <returns>入れ替えの成否</returns>
    bool setSelectedText(std::wstring text);

    /// <summary>
    /// 現在、カーソルがある行(エディタ的)のテキスト内容を返す。
    /// </summary>
    /// <returns>カーソルがある行のテキスト内容</returns>
    std::wstring getLineText();
 
    /// <summary>
    /// 現在、カーソルがある行(エディタ的)のテキスト内容を指定の文字列とする。
    /// </summary>
    /// <param name = "text">新たなテキスト内容</param>
    /// <returns>入れ替えの成否</returns>
    bool setLineText(std::wstring text);

    /// <summary>
    /// キー入力があるなどの理由で処理を中断するべきかを返す。
    /// </summary>
    /// <returns>中断するべきならtrue、そうでなければfalse</returns>
    bool isQueueStatus();

public:
    /// <summary>
    /// getCursorPos()の返り値の型。
    /// </summary>
    class ICursorPos {
        int lineno;
        int column;
    public:
        /// <summary>
        /// Unicode的に考えたLineNo。
        /// </summary>
        /// <returns>Unicode的に考えた行。0行目から</returns>
        int getLineNo();

        /// <summary>
        /// Unicode的に考えたColumn。
        /// </summary>
        /// <returns>Column的に考えた桁。0桁目から</returns>
        int getColumn();

        ICursorPos(int lineno, int column);
    };

    /// <summary>
    /// getMousePos()の返り値の型。
    /// </summary>
    class IMousePos {
        int lineno;
        int column;
        int x;
        int y;
    public:
        /// <summary>
        /// Unicode的に考えたLineNo。
        /// </summary>
        /// <returns>Unicode的に考えた行。0行目から</returns>
        int getLineNo();

        /// <summary>
        /// Unicode的に考えたColumn。
        /// </summary>
        /// <returns>Column的に考えた桁。0桁目から</returns>
        int getColumn();

        
        /// <summary>
        /// 「win32apiのGetCursorPos」で得られるxと基本同じもの。
        /// </summary>
        /// <returns>マウスのx座標</returns>
        int getX();

        /// <summary>
        /// 「win32apiのGetCursorPos」で得られるyと基本同じもの。
        /// </summary>
        /// <returns>マウスのy座標</returns>
        int getY();
        IMousePos(int lineno, int column, int x, int y);
    };

    /// <summary>
    /// ユニコードのエディタ的な換算でのカーソルの位置を返す。
    /// </summary>
    /// <returns>ユニコードのエディタ的な換算でのカーソルの位置</returns>
    ICursorPos getCursorPos();

    /// <summary>
    /// ユニコードのエディタ的な換算でのマウスの位置に対応するカーソルの位置を返す。
    /// </summary>
    /// <returns>ユニコードのエディタ的な換算でのマウスの位置に対応するカーソルの位置</returns>
    IMousePos getMousePos();
};

