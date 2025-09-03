# HmCppInvoke

[![Hm.CppInvoke v2.1.2](https://img.shields.io/badge/Hm.CppInvoke-v2.1.2-6479ff.svg)](https://github.com/komiyamma/hidemaru_cpp_invoke/releases)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
![Hidemaru 8.98](https://img.shields.io/badge/Hidemaru-v8.73-6479ff.svg)
![C++ 17,20](https://img.shields.io/badge/C++-v17_|_v20-6479ff.svg?logo=Cplusplus&logoColor=white)

Hm.CppInvokeとは、秀丸マクロの「loadll」関連機能を用い、「C++記述内で秀丸マクロ変数・秀丸マクロ関数」などを利用するためのライブラリです。

https://秀丸マクロ.net/?page=nobu_tool_hm_cpp_invoke

https://www.nuget.org/packages/HmCppInvoke-vs2022/

https://www.nuget.org/packages/HmCppInvoke-vs2019/

## プロジェクト構造

主要なファイルとディレクトリの構造は以下の通りです。

```
.
├── HmCppInvoke/              # DLL本体のソースコードとプロジェクト
│   ├── HmCppInvoke.h
│   ├── HmCppInvoke.cpp
│   ├── HmCppInvokeConvertString.h
│   ├── HmCppInvokeConvertString.cpp
│   ├── HmCppInvokeDllBindType.h
│   ├── HmCppInvokeDllBindType.cpp
│   ├── HmCppInvokeDllExport.h
│   ├── HmCppInvokeDllFuncCalledType.h
│   ├── HmCppInvokeDllFuncCalledType.cpp
│   ├── HmCppInvokeEdit.h
│   ├── HmCppInvokeEdit.cpp
│   ├── HmCppInvokeExplorerPane.h
│   ├── HmCppInvokeExplorerPane.cpp
│   ├── HmCppInvokeFile.h
│   ├── HmCppInvokeFile.cpp
│   ├── HmCppInvokeMacro.h
│   ├── HmCppInvokeMacro.cpp
│   ├── HmCppInvokeOutputPane.h
│   └── HmCppInvokeOutputPane.cpp
│
├── pack_projet/              # NuGetパッケージ作成用のスクリプトと設定
│   ├── copy_to_nuget-pack-vs2019.bat
│   ├── copy_to_nuget-pack-vs2022.bat
│   ├── nuget-pack-vs2019/
│   │   └── HmCppInvoke-vs2019.nuspec
│   └── nuget-pack-vs2022/
│       └── HmCppInvoke-vs2022.nuspec
│
├── HmCppInvoke.sln           # Visual Studio ソリューションファイル
├── License.txt
└── README.md
```