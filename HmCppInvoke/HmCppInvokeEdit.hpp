/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

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

        operator std::tuple<int, int>();
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

        operator std::tuple<int, int, int, int>();
    };

    ICursorPos getCursorPos();
    IMousePos getMousePos();
};

