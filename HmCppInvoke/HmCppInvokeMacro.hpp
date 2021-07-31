/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

class TMacro {
    // dllの中から秀丸マクロを実行する
    using PFNEvalMacro = BOOL(WINAPI*)(const wchar_t* pwsz);
    static PFNEvalMacro Hidemaru_EvalMacro;
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
    IResult doEval(std::wstring expression);

    /// <summary>
    /// 対象の「秀丸マクロ変数名」の読み込み
    /// </summary>
    /// <param name = "varname">変数のシンボル名</param>
    /// <returns>THmMacroVariable型の値。原則的にはstd::get&lt;THmNumber&gt;もしくはstd::get&lt;wstring&gt;で値を取得することになる。</returns>
    THmMacroVariable getVar(std::wstring varname);

    /// <summary>
    /// 対象の「秀丸マクロ変数名」への書き込み
    /// </summary>
    /// <param name = "varname">変数のシンボル名</param>
    /// <param name = "value">書き込みの場合、代入する値</param>
    /// <returns>代入の成否。成功すればtrue、失敗すればfalse</returns>
    bool setVar(std::wstring varname, THmMacroVariable value);


    class IFunctionResult {
        THmMacroVariable result;
        THmMacroResultError error;
        std::vector<THmMacroVariable> args;
        std::wstring message;
    public:
        THmMacroVariable getResult();
        THmMacroResultError getException();
        std::vector<THmMacroVariable> getArgs();
        std::wstring getMessage();
        IFunctionResult(THmMacroVariable result, std::vector<THmMacroVariable> args, THmMacroResultError error, std::wstring message);
    };

    /// <summary>
    /// 秀丸マクロの「関数」を実行
    /// </summary>
    /// <param name = "func_name">関数名</param>
    /// <param name = "args0">マクロ関数への引数1</param>
    /// <param name = "args1">マクロ関数への引数2</param>
    /// <param name = "args2">マクロ関数への引数3</param>
    /// <param name = "args3">マクロ関数への引数4</param>
    /// <param name = "args4">マクロ関数への引数5</param>
    /// <param name = "args5">マクロ関数への引数6</param>
    /// <param name = "args6">マクロ関数への引数7</param>
    /// <param name = "args7">マクロ関数への引数8</param>
    /// <param name = "args8">マクロ関数への引数9</param>
    /// <param name = "args9">マクロ関数への引数10</param>
    IFunctionResult doFunction(std::wstring func_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

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

    /// <summary>
    /// 秀丸マクロの関数のような「命令文」を実行
    /// </summary>
    /// <param name = "statement_name">（関数のような）命令文名</param>
    /// <param name = "args0">マクロ命令文への引数1</param>
    /// <param name = "args1">マクロ命令文への引数2</param>
    /// <param name = "args2">マクロ命令文への引数3</param>
    /// <param name = "args3">マクロ命令文への引数4</param>
    /// <param name = "args4">マクロ命令文への引数5</param>
    /// <param name = "args5">マクロ命令文への引数6</param>
    /// <param name = "args6">マクロ命令文への引数7</param>
    /// <param name = "args7">マクロ命令文への引数8</param>
    /// <param name = "args8">マクロ命令文への引数9</param>
    /// <param name = "args9">マクロ命令文への引数10</param>
    IStatementResult doStatement(std::wstring statement_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

private:
    void SetMacroVarAndMakeMacroKeyArray(const std::vector<THmMacroVariable> value_args, std::vector<std::wstring>& varname_list);
    void ClearMacroVarAndUpdateArgs(const std::vector<THmMacroVariable> value_args, const std::vector<std::wstring> varname_list, std::vector<THmMacroVariable>& updated_value_args);
public:
    class TExec {
    public:
        /// <summary>
        /// 「マクロを実行していない時」に、プログラム中で、文字列で新たなマクロを実行。
        /// <para>マクロを実行していない時のみ実行可能なメソッド。</para>
        /// </summary>
        IResult doEval(std::wstring expression);

        /// <summary>
        /// 「マクロを実行していない時」に、プログラム中で、マクロファイルを与えて新たなマクロを実行。
        /// <para>マクロを実行していない時のみ実行可能なメソッド。</para>
        /// </summary>
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
};

