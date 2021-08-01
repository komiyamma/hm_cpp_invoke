/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

//-------------------------------------------------------------------------
// dll自身のハンドルやフスパスの情報の保持
//-------------------------------------------------------------------------
class TDllBindType {

    //-------------------------------------------------------------------------
    // 自分自身(このdll)のモジュールインスタンスハンドル
    static HMODULE hModule;

    //-------------------------------------------------------------------------
    // 自分自身(このdll)のフルパス
    static wchar_t wszSelfModuleFullPath[MAX_PATH];

    // 自分自身(このdll)のディレクトリのパス
    static wchar_t wszSelfModuleDirPath[MAX_PATH];

    //-------------------------------------------------------------------------
    // このdllが秀丸マクロからどのような形でloaddllされたのかの情報。
    // この情報があれば、dll内部からマクロを発行することが出来る。
    // -1   :loaddll文で束縛だれた
    // 0    :読めていない。(読めてなかったらdll実行されてないので、これはあり得ない)
    // 1以上:その数値で秀丸マクロ上で束縛されている。
    //-------------------------------------------------------------------------
public:
    TDllBindType();
private:
    static int iSelfBindedType;
private:
    static HMODULE getCurrentModule();
public:
    static int getDllBindType();
    static BOOL setDllBindType();
    static std::wstring getInvocantString();
    static std::wstring getSelfModuleFullPath();
    static std::wstring getSelfModuleDir();
};
