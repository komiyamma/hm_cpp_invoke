/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */

class TMacro {
    // dllÇÃíÜÇ©ÇÁèGä€É}ÉNÉçÇé¿çsÇ∑ÇÈ
    using PFNEvalMacro = BOOL(WINAPI*)(const wchar_t* pwsz);
    static PFNEvalMacro Hidemaru_EvalMacro;
public:
    TMacro();
public:
    bool isExecuting();

public:
    class IResult {
        THmNumber result;
        THmMacroResultError error;
        std::wstring message;
    public:
        THmNumber getResult();
        THmMacroResultError getException();
        std::wstring getMessage();
        IResult(THmNumber result, THmMacroResultError error, std::wstring message);
    };

    IResult doEval(std::wstring expression);

    THmMacroVariable getVar(std::wstring varname);
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

    IStatementResult doStatement(std::wstring statement_name, THmMacroVariable args0 = nullptr, THmMacroVariable args1 = nullptr, THmMacroVariable args2 = nullptr, THmMacroVariable args3 = nullptr, THmMacroVariable args4 = nullptr, THmMacroVariable args5 = nullptr, THmMacroVariable args6 = nullptr, THmMacroVariable args7 = nullptr, THmMacroVariable args8 = nullptr, THmMacroVariable args9 = nullptr);

private:
    void SetMacroVarAndMakeMacroKeyArray(const std::vector<THmMacroVariable> value_args, std::vector<std::wstring>& varname_list);
    void ClearMacroVarAndUpdateArgs(const std::vector<THmMacroVariable> value_args, const std::vector<std::wstring> varname_list, std::vector<THmMacroVariable>& updated_value_args);
public:
    class TExec {
    public:
        IResult doEval(std::wstring expression);
        IResult doFile(std::wstring filepath);
        IResult doMethod(std::wstring message_parameter, THmMacroDoMethodType callback_method);
    };

    TExec Exec;
};

