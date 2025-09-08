# HmCppInvoke

[![Hm.CppInvoke v2.1.2](https://img.shields.io/badge/Hm.CppInvoke-v2.1.2-6479ff.svg)](https://github.com/komiyamma/hidemaru_cpp_invoke/releases)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
![Hidemaru 8.98](https://img.shields.io/badge/Hidemaru-v8.73-6479ff.svg)
![C++ 17,20](https://img.shields.io/badge/C++-v17_|_v20-6479ff.svg?logo=Cplusplus&logoColor=white)

Hm.CppInvokeとは、秀丸マクロの「loadll」関連機能を用い、「C++記述内で秀丸マクロ変数・秀丸マクロ関数」などを利用するためのライブラリです。

## 概要

`HmCppInvoke` は、秀丸エディタのDLLインターフェースをC++から簡単かつモダンなスタイルで利用するためのラッパーライブラリです。
秀丸マクロの `loadll` を通じてC++で書かれたDLLを読み込み、そのDLL内から秀丸エディタのテキスト操作、マクロの実行、変数や関数の呼び出し、さらにはアウトプット枠やファイルマネージャ枠といった各種ペインの操作まで、多岐にわたる機能を直感的に扱えるように設計されています。
C++17/20の機能を活用しており、`std::wstring` や `std::variant`、`std::optional` といった現代的なC++の型をネイティブにサポートしているため、面倒な文字列変換や型管理の手間を大幅に削減します。
このライブラリを使うことで、複雑な処理や高速性が求められる機能を、秀丸マクロのシンプルさとC++のパワーを両立させながら開発できます。

## 主な機能

`HmCppInvoke` は、秀丸エディタの各機能をオブジェクト指向のクラスとして整理し、直感的なアクセスを提供します。

### `Hm.Edit` - テキスト操作
- ドキュメント全体のテキスト取得・設定
- 選択範囲のテキスト取得・設定
- 現在行のテキスト取得・設定
- カーソル位置やマウス位置の取得

### `Hm.Macro` - マクロ連携
- 文字列やファイルからのマクロ実行
- マクロ変数の取得・設定（ローカル変数・静的変数）
- 秀丸マクロの関数や命令の呼び出し（可変長引数対応）
- C++の関数を秀丸マクロとして実行

### `Hm.File` - ファイル操作
- 秀丸エディタの文字コード判別機能を利用したファイル読み込み
- `IStreamReader` 形式によるシンプルなファイルアクセス

### `Hm.OutputPane` - アウトプット枠操作
- アウトプット枠へのテキスト出力
- 内容の保存、復元、クリア
- ベースディレクトリの設定

### `Hm.ExplorerPane` - ファイルマネージャ枠操作
- プロジェクトの読み込み・保存
- 表示モードの取得・設定
- カレントディレクトリの取得

## 関連URL

https://秀丸マクロ.net/?page=nobu_tool_hm_cpp_invoke

https://www.nuget.org/packages/HmCppInvoke-vs2022/

https://www.nuget.org/packages/HmCppInvoke-vs2019/
