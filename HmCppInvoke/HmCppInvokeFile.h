/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

 /// <summary>
 /// ファイル関連
 /// </summary>
class TFile {
    // 何のエンコードで開かれるのかを取得する機能となる。
    using PFNAnalyzeEncoding = int(WINAPI*)(const WCHAR* pwszFileName, DWORD_PTR lParam1, DWORD_PTR lParam2);
    static PFNAnalyzeEncoding Hidemaru_AnalyzeEncoding;

    // 指定の秀丸のencodeを指定して、ファイル内容を読み込む
    using PFNLoadFileUnicode = HGLOBAL(WINAPI*)(const WCHAR* pwszFileName, int nEncode, UINT* pcwchOut, DWORD_PTR lParam1, DWORD_PTR lParam2);
    static PFNLoadFileUnicode Hidemaru_LoadFileUnicode;
public:
    TFile();

    /// <summary>
    /// MsCodePage と HmEncode を一括で扱うためのクラス
    /// </summary>
    class IEncoding {
        int hm_encode;
        int ms_codepage;
    public:
        IEncoding(int hm_encode, int ms_codepage);
        IEncoding() = default;
        IEncoding(const IEncoding&) = default;

        /// <summary>
        /// 秀丸のencode値相当を得る。厳密には マクロのencode値 「&amp;  0x3F」のANDビット演算した値）
        /// <para>https://help.maruo.co.jp/hidemac/html/060_Keyword_State.html#encode の0～28と同じ</para>
        /// </summary>
        int getHmEncode();

        /// <summary>
        /// マイクロソフトのCodePage値相当を得る。
        /// <para>Win32apiを使って文字エンコードを処理するにはこの値が必要となるだろう。</para>
        /// </summary>
        int getMsCodePage();
    };

    /// <summary>
    /// 秀丸のEncode値からMsCodePageの値を得るためのメソッド
    /// 秀丸のencode値とは、厳密には マクロのencode値 「&amp; 0x3F」のANDビット演算した値）
    /// <para>https://help.maruo.co.jp/hidemac/html/060_Keyword_State.html#encode の0～28と同じ</para>
    /// </summary>
    /// <param name = "hm_encode">秀丸のEncode値</param>
    /// <returns>IEncoding型のオブジェクト。MsCodePage と HmEncode を得られる。</returns>
    IEncoding getEncoding(int hm_encode);

    /// <summary>
    /// 指定のファイルの絶対パスからIEncoding型オブジェクを得るためのメソッド
    /// <para>（あなたの秀丸のファイルのエンコード判断の設定に従う。汎用的なファイルエンコード判定関数ではない）</para>
    /// </summary>
    /// <returns>IEncoding型のオブジェクト。MsCodePage と HmEncode を得られる。</returns>
    IEncoding getEncoding(std::wstring filepath);
    const std::map<int, int> getEncodingMap();

    class IHidemaruStreamReader {
        IEncoding encoding;
        std::wstring filepath;
    public:
        IHidemaruStreamReader(std::wstring filepath, IEncoding encoding);
        ~IHidemaruStreamReader();

        /// <summary>
        /// 対象となっているファイルのIEncodingを得る。秀丸ベースのencoding値とマイクロソフトベースのCodePage値を得ることが出来る。
        /// </summary>
        IEncoding getEncoding();

        /// <summary>
        /// 対象となっているファイルのテキスト内容を一括で読み込む。
        /// </summary>
        std::wstring read();

        /// <summary>
        /// 対象となっているファイルの絶対パスの取得。
        /// </summary>
        std::wstring getFilePath();

        /// <summary>
        /// 現在ひらいている、この IHidemaruStreamReader オブジェクトを閉じる。
        /// </summary>
        void close();
    };

    /// <summary>
    /// 秀丸でファイルのエンコードを判断し、その判断結果に基づいてファイルのテキスト内容を取得する。
    /// （秀丸に設定されている内容に従う）
    /// </summary>
    /// <param name = "filepath">読み込み対象のファイルのパス</param>
    /// <param name = "hm_encode">エンコード(秀丸マクロの「encode」の値)が分かっているならば指定する、指定しない場合秀丸APIの自動判定に任せる。</param>
    /// <returns>IHidemaruStreamReader型のオブジェクト。</returns>
    IHidemaruStreamReader open(std::wstring filepath, int hm_encode = -1);
};

