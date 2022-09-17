/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

 /// <summary>
 /// マクロ関連
 /// </summary>
class TMacro {
    // dllの中から秀丸マクロを実行する
    using PFNEvalMacro = BOOL(WINAPI*)(const wchar_t* pwsz);
    static PFNEvalMacro Hidemaru_EvalMacro;

    using PFNGetStaticVariable = HGLOBAL(WINAPI*)(const wchar_t* pwszName, int sharedFlag);
    static PFNGetStaticVariable Hidemaru_GetStaticVariable;

    using PFNSetStaticVariable = BOOL(WINAPI*)(const wchar_t* pwszName, const wchar_t* pwszValue, int sharedFlag);
    static PFNSetStaticVariable Hidemaru_SetStaticVariable;
public:
    TMacro();
public:
    /// <summary>
    /// マクロを実行中か否かを判定する
    /// </summary>
    /// <returns>実行中ならtrue, そうでなければfalse</returns>
    bool isExecuting();

public:
    /// <summary>
    /// マクロをプログラム内から実行した際の返り値のインターフェイス
    /// </summary>
    class IResult {
        THmNumber result;
        THmMacroResultError error;
        std::wstring message;
    public:

        /// <summary>
        /// マクロ実行が成否。
        /// </summary>
        /// <returns>マクロ実行が成功した場合には、1以上、失敗した場合には0以下の値。-1以下のこともあるため0で決め打ちしないこと。</returns>
        THmNumber getResult();

        /// <summary>
        /// マクロ実行の失敗時の例外。
        /// </summary>
        /// <returns>マクロ実行が失敗している場合にexception型の例外が入っている。例外がなければ、std::nulloptが入っている。</returns>
        THmMacroResultError getException();

        /// <summary>
        /// マクロ実行結果としてのメッセージ。メソッドによって異なる。
        /// </summary>
        std::wstring getMessage();

        IResult(THmNumber result, THmMacroResultError error, std::wstring message);
    };

    /// <summary>
    /// 現在の「マクロ実行中」に、プログラム中で、マクロを文字列で実行。
    /// <para>マクロ実行中のみ実行可能なメソッド</para>
    /// </summary>
    /// <param name = "expression">マクロ内容の文字列</param>
    IResult doEval(std::wstring expression);

    /// <summary>
    /// 対象の「秀丸マクロ変数名」の読み込み
    /// </summary>
    /// <param name = "varname">変数のシンボル名</param>
    /// <returns>THmMacroVariable型の値。原則的にはstd::get&lt;THmNumber&gt;もしくはstd::get&lt;wstring&gt;で値を取得することになる。</returns>
    THmMacroVariable getVar(std::wstring varname);

    /// <summary>
    /// 対象の「秀丸マクロ変数名」の読み込み。テンプレートパラメータとして型を指定することでマニュアルでの値の取得を省略できる。
    /// テンプレートパラメータに指定可能な型は「THmNumber」もしくは「wstring」のみ。
    /// </summary>
    /// <param name = "varname">変数のシンボル名</param>
    /// <param name = "T">Tに指定可能な型は「THmNumber」もしくは「wstring」のみ</param>
    /// <returns>Tで指定した型の値。Tには「THmNumber」もしくは「wstring」値を指定して取得することになる。</returns>
    template<class T>
    T getVar(std::wstring varname) {
        THmMacroVariable v = getVar(varname);
        T r = std::get<T>(v);
        return r;
    }

    /// <summary>
    /// 対象の「秀丸マクロ変数名」への書き込み
    /// </summary>
    /// <param name = "varname">変数のシンボル名</param>
    /// <param name = "value">書き込みの場合、代入する値</param>
    /// <returns>代入の成否。成功すればtrue、失敗すればfalse</returns>
    bool setVar(std::wstring varname, THmMacroVariable value);


    /// <summary>
    /// 対象の「秀丸マクロの静的な変数名」への書き込み (秀丸マクロのsetstaticvariable相当だが常時実行可)
    /// </summary>
    /// <param name = "varname">静的な変数のシンボル名</param>
    /// <param name = "value">代入する文字列</param>
    /// <param name = "shared_flag">共用フラグ</param>
    /// <returns>代入の成否。成功すればtrue、失敗すればfalse</returns>
    bool setStaticVar(std::wstring varname, std::wstring value, int shared_flag);

    /// <summary>
    /// 対象の「秀丸マクロの静的な変数名」の読み込み (秀丸マクロのgetstaticvariable相当だが常時実行可)
    /// </summary>
    /// <param name = "varname">静的な変数のシンボル名</param>
    /// <param name = "shared_flag">共用フラグ</param>
    /// <returns>対象のキーに格納された文字列型の値。</returns>
    std::wstring getStaticVar(std::wstring varname, int shared_flag);


    class IFunctionResult {
        THmMacroVariable result;
        THmMacroResultError error;
        std::vector<THmMacroVariable> args;
        std::wstring message;
    public:
        THmMacroVariable getResult();
        template<class T>
        T getResult() {
            THmMacroVariable v = getResult();
            T r = std::get<T>(v);
            return r;
        }
        THmMacroResultError getException();
        std::vector<THmMacroVariable> getArgs();
        std::wstring getMessage();
        IFunctionResult(THmMacroVariable result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
    };

private:
    IFunctionResult doFunctionHelper(std::wstring func_name, std::vector<THmMacroVariable> value_args);

public:

    /// <summary>
    /// 秀丸マクロの「関数」を実行
    /// </summary>
    /// <param name = "func_name">関数名</param>
    /// <param name = "args">マクロ関数への可変引数</param>
    template<class... THmMacroFunctionPARAMS>
    IFunctionResult doFunction(std::wstring func_name, THmMacroFunctionPARAMS... args) {

        std::vector<THmMacroVariable> value_args;
        for (THmMacroVariable arg : std::initializer_list<THmMacroVariable>{ args... }) {
            try {
                value_args.push_back(arg);
            }
            catch (...) {
            }
        }

        return doFunctionHelper(func_name, value_args);
    }

    class IStatementResult {
        THmNumber result;
        THmMacroResultError error;
        std::vector<THmMacroVariable> args;
        std::wstring message;
    public:
        THmNumber getResult();
        THmMacroResultError getException();
        std::vector<THmMacroVariable> getArgs();
        std::wstring getMessage();
        IStatementResult(THmNumber result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
    };


private:
    IStatementResult doStatementHelper(std::wstring statement_name, std::vector<THmMacroVariable> value_args);

public:
    /// <summary>
    /// 秀丸マクロの関数のような「命令文」を実行
    /// </summary>
    /// <param name = "statement_name">（関数のような）命令文名</param>
    /// <param name = "args">マクロ命令文への可変引数</param>
    template<class... THmMacroStatementPARAMS>
    IStatementResult doStatement(std::wstring statement_name, THmMacroStatementPARAMS... args) {

        std::vector<THmMacroVariable> value_args;
        for (THmMacroVariable arg : std::initializer_list<THmMacroVariable>{ args... }) {
            try {
                value_args.push_back(arg);
            }
            catch (...) {
            }
        }

        return doStatementHelper(statement_name, value_args);
    }


private:
    void setMacroVarAndMakeMacroKeyArray(const std::vector<THmMacroVariable> value_args, std::vector<std::wstring>& varname_list);
    void clearMacroVarAndUpdateArgs(const std::vector<THmMacroVariable> value_args, const std::vector<std::wstring> varname_list, std::vector<THmMacroVariable>& updated_value_args);
public:
    class TExec {
    public:
        /// <summary>
        /// 「マクロを実行していない時」に、プログラム中で、文字列で新たなマクロを実行。
        /// <para>マクロを実行していない時のみ実行可能なメソッド。</para>
        /// </summary>
        /// <param name = "expression">マクロ内容の文字列</param>
        IResult doEval(std::wstring expression);

        /// <summary>
        /// 「マクロを実行していない時」に、プログラム中で、マクロファイルを与えて新たなマクロを実行。
        /// <para>マクロを実行していない時のみ実行可能なメソッド。</para>
        /// </summary>
        /// <param name = "filepath">マクロファイルのフルパス</param>
        IResult doFile(std::wstring filepath);

        /// <summary>
        /// 指定のユーザー定義のstaticメソッドを「新たなマクロ実行空間」として呼び出す
        /// </summary>
        /// <param name = "message_parameter">文字列パラメータ</param>
        /// <param name = "delegate_method">呼び出したい関数もしくはメソッド「THmNumber methodname(wstring message_parameter)の型」に従うメソッドであること</param>
        IResult doMethod(std::wstring message_parameter, THmMacroDoMethodType delegate_method);
    };

    /// <summary>
    /// 「マクロが実行されていない時限定で、改めてマクロを実行できる」系のメソッド群このクラスにある。名前空間の代替。
    /// </summary>
    TExec Exec;

public:
    /// <summary>
    /// 秀丸マクロで用いられる特定の関数でのみ使用可能なフラグ値。主に検索・置換・Grep系、そしてエンコード系。
    /// </summary>
    class TFlags {
    public:

        /// <summary>
        /// openfile等のencode相当
        /// </summary>
        class TEncode {
        public:
            static const int Sjis = 0x01;
            static const int Utf16 = 0x02;
            static const int Euc = 0x03;
            static const int Jis = 0x04;
            static const int Utf7 = 0x05;
            static const int Utf8 = 0x06;
            static const int Utf16_be = 0x07;
            static const int Euro = 0x08;
            static const int Gb2312 = 0x09;
            static const int Big5 = 0x0a;
            static const int Euckr = 0x0b;
            static const int Johab = 0x0c;
            static const int Easteuro = 0x0d;
            static const int Baltic = 0x0e;
            static const int Greek = 0x0f;
            static const int Russian = 0x10;
            static const int Symbol = 0x11;
            static const int Turkish = 0x12;
            static const int Hebrew = 0x13;
            static const int Arabic = 0x14;
            static const int Thai = 0x15;
            static const int Vietnamese = 0x16;
            static const int Mac = 0x17;
            static const int Oem = 0x18;
            static const int Default = 0x19;
            static const int Utf32 = 0x1b;
            static const int Utf32_be = 0x1c;
            static const int Binary = 0x1a;
            static const int LF = 0x40;
            static const int CR = 0x80;

            //SAVEASの他のオプションの数値指定
            static const int Bom = 0x0600;
            static const int NoBom = 0x0400;
            static const int Selection = 0x2000;

            //OPENFILEの他のオプションの数値指定
            static const int NoAddHist = 0x0100;
            static const int WS = 0x0800;
            static const int WB = 0x1000;
        };

        TEncode Encode;

        class TSearchOption {
        public:
            //searchoption(検索関係)
            static const int Word = 0x00000001;
            static const int Casesense = 0x00000002;
            static const int NoCasesense = 0x00000000;
            static const int Regular = 0x00000010;
            static const int NoRegular = 0x00000000;
            static const int Fuzzy = 0x00000020;
            static const int Hilight = 0x00003800;
            static const int NoHilight = 0x00002000;
            static const int LinkNext = 0x00000080;
            static const int Loop = 0x01000000;

            //searchoption(マスク関係)
            static const int MaskComment = 0x00020000;
            static const int MaskIfdef = 0x00040000;
            static const int MaskNormal = 0x00010000;
            static const int MaskScript = 0x00080000;
            static const int MaskString = 0x00100000;
            static const int MaskTag = 0x00200000;
            static const int MaskOnly = 0x00400000;
            static const int FEnableMaskFlags = 0x00800000;

            //searchoption(置換関係)
            static const int FEnableReplace = 0x00000004;
            static const int Ask = 0x00000008;
            static const int NoClose = 0x02000000;

            //searchoption(grep関係)
            static const int SubDir = 0x00000100;
            static const int Icon = 0x00000200;
            static const int Filelist = 0x00000040;
            static const int FullPath = 0x00000400;
            static const int OutputSingle = 0x10000000;
            static const int OutputSameTab = 0x20000000;

            //searchoption(grepして置換関係)
            static const int BackUp = 0x04000000;
            static const int Preview = 0x08000000;

            // searchoption2を使うよ、というフラグ。なんと、int32_maxを超えているので、特殊な処理が必要。
            const THmNumber FEnableSearchOption2 = (THmNumber)0x80000000;

        };

        TSearchOption SearchOption;

        class TSearchOption2 {
        public:
            //searchoption2
            static const int UnMatch = 0x00000001;
            static const int InColorMarker = 0x00000002;
            static const int FGrepFormColumn = 0x00000008;
            static const int FGrepFormHitOnly = 0x00000010;
            static const int FGrepFormSortDate = 0x00000020;
        };

        TSearchOption2 SearchOption2;
    };

    TFlags Flags;
};

