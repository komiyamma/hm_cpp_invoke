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
        int hm_encode;
        int ms_codepage;
    public:
        IEncoding(int hm_encode, int ms_codepage);
        IEncoding() = default;
        IEncoding(const IEncoding&) = default;
        int getHmEncode();
        int getMsCodePage();
    };

    IEncoding getEncoding(int hm_encode);
    IEncoding getEncoding(std::wstring filepath);
    const std::map<int, int> getEncodingMap();

    class IHidemaruStreamReader {
        IEncoding encoding;
        std::wstring filepath;
    public:
        IHidemaruStreamReader(std::wstring filepath, IEncoding encoding);
        IEncoding getEncoding();
        std::wstring read();
        std::wstring getFilePath();
        void close();
    };

    IHidemaruStreamReader open(std::wstring filepath, int hm_encode = -1);
};

