#pragma once

#include <Windows.h>
#include <string>

namespace Hidemaru {
    class THm {
    public:
        THm();
    public:
        static const double getVersion();
    private:
        static bool setVersion();

    public:
        class TEdit {
        public:
            TEdit();
        public:
            static std::wstring getFilePath();
            static std::wstring getTotalText();
            static bool setTotalText(std::wstring text);
            static std::wstring getSelectedText();
            static bool setSelectedText(std::wstring text);
            static std::wstring getLineText();
            static bool setLineText(std::wstring text);

        public:
            class ICursorPos {
            public:
                virtual int getLineNo() = 0;
                virtual int getColumn() = 0;
            };

            class TCursorPos : public ICursorPos {
            public:
                int getLineNo();
                int getColumn();
            };

            class IMousePos {
            public:
                virtual int getLineNo() = 0;
                virtual int getColumn() = 0;
                virtual int getX() = 0;
                virtual int getY() = 0;
            };

            class TMousePos : public ICursorPos {
            public:
                int getLineNo();
                int getColumn();
                int getX();
                int getY();
            };
        };

    public:
        TEdit Edit;

    public:
        class TOutputPane {
        public:
            TOutputPane();
        public:
            static bool output(std::wstring message);
            static bool push();
            static bool pop();
            static bool clear();
            static LRESULT sendMessage(int command_id);
            static bool setBaseDir(std::wstring dirpath);
            static HWND getWindowHandle();
        };

    public:
        TOutputPane OutputPane;

    public:
        class TExplorerPane {
        public:
            TExplorerPane();
        public:
            static bool setMode(int mode);
            static int getMode();
            static bool loadProject(std::wstring filepath);
            static bool saveProject(std::wstring filepath);
            static std::wstring getProject();
            static LRESULT sendMessage(int command_id);
            static HWND getWindowHandle();
            static bool getUpdated();
        };

    public:
        TExplorerPane ExplorerPane;
    };

};

extern Hidemaru::THm Hm;

