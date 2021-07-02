#pragma once

#include "Windows.h"
#include <string>
#include <vector>
#include <any>


namespace Hidemaru {

    using MacroScopeMethodPointer = long (*)(std::wstring message_parameter);

    class THm {
    public:
        THm();
    public:
        double getVersion();
        HWND getWindowHandle();
    private:
        bool setVersion();

    public:
        class TEdit {
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
            public:
                int getLineNo();
                int getColumn();
                ICursorPos(int lineno, int column);
            };

            class IMousePos {
            public:
                int getLineNo();
                int getColumn();
                int getX();
                int getY();
                IMousePos(int lineno, int column, int x, int y);
            };

            ICursorPos getCursorPos();
            IMousePos getMousePos();
        };

    public:
        TEdit Edit;

    public:
        class TMacro {
        public:
            TMacro();
        public:
            bool isExecuting();

        public:
            class IResult {
            public:
                int getResult();
                std::any getException();
                std::wstring getMessage();
                IResult(int result, std::any error, std::wstring message);
            };

            IResult doEval(std::wstring expression);

            std::any getVar(std::wstring varname);
            bool setVar(std::wstring varname, std::any);

            class IFunctionResult {
            public:
                std::any getResult();
                std::vector<std::any> getArgs();
                std::any getException();
                std::wstring getMessage();
                IFunctionResult(std::any result, std::vector<std::any> args, std::any error, std::wstring message);
            };

            IFunctionResult doFunction(std::wstring func_name, std::any args0 = nullptr, std::any args1 = nullptr, std::any args2 = nullptr, std::any args3 = nullptr, std::any args4 = nullptr, std::any args5 = nullptr, std::any args6 = nullptr, std::any args7 = nullptr, std::any args8 = nullptr, std::any args9 = nullptr);

            class IStatementResult {
            public:
                int getResult();
                std::vector<std::any> getArgs();
                std::any getException();
                std::wstring getMessage();
                IStatementResult(int result, std::vector<std::any> args, std::any error, std::wstring message);
            };

            IStatementResult doStatement(std::wstring statement_name, std::any args0 = nullptr, std::any args1 = nullptr, std::any args2 = nullptr, std::any args3 = nullptr, std::any args4 = nullptr, std::any args5 = nullptr, std::any args6 = nullptr, std::any args7 = nullptr, std::any args8 = nullptr, std::any args9 = nullptr);

        public:
            class TExec {
            public:
                IResult doEval(std::wstring expression);
                IResult doFile(std::wstring filepath);
                IResult doMethod(std::wstring message_parameter, MacroScopeMethodPointer callback_method);
            };

            TExec Exec;
        };

    public:
        TMacro Macro;


    public:
        class TOutputPane {
        public:
            TOutputPane();
        public:
            bool output(std::wstring message);
            bool push();
            bool pop();
            bool clear();
            long sendMessage(int command_id);
            bool setBaseDir(std::wstring dirpath);
            HWND getWindowHandle();
        };

    public:
        TOutputPane OutputPane;

    public:
        class TExplorerPane {
        public:
            TExplorerPane();
        public:
            bool setMode(int mode);
            int getMode();
            bool loadProject(std::wstring filepath);
            bool saveProject(std::wstring filepath);
            std::wstring getProject();
            long sendMessage(int command_id);
            HWND getWindowHandle();
            bool getUpdated();
        };

    public:
        TExplorerPane ExplorerPane;
    };
};

extern Hidemaru::THm Hm;

