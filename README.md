sujinikuRpgRuntimeCppWin32
====
# 注意 Attention
未完成のため、ほとんどの機能は未実装です。  
Because it is incomplete, most functions are not implemented yet.

名前は「ランタイム」ですが、しかし現時点では、このアプリ単独でしか動作しません。  
The name has "runtime", but at this time it works only with this application alone, because of incomplete.

現状では日本語しかサポートしていません。英語はサポートしていません。  
Currently only Japanese is supported.  English is not supported. 

# 使用プログラミング言語 Programming lnaguage
Win32 API を使用しています。Visual C++ 2017 を使用しています。  
This app is develppped Win32 API on Visual C++ (Visual Studio 2017).

Direct X は非使用です。  
This app is not using DirectX now.


# 動作環境 Operating environment
いまのところ、Windows 上でしか動作しません。Windows7 での動作を確認しています。  
I have checked the moving this app on Windows 7.  

Direct X は非使用です。よって、Direct X のインストールは不要です。  
Direct X is not used. Therefore, installation of Direct X is unnecessary.  

# 利用規約 Terms of service
コードのライセンスは MITライセンス で公開しています。  
The license of this app is released under MIT license.  

また、私 sujiniku が当ソフト内で公開した画像ファイルやマップデータなどコンテンツのライセンスは、特にことわりのないかぎり、原則的に パブリックドメイン または クリエイティブコモンズ0 (CC0) とします。  
Also, the licenses of contents such as image files and map data released by me sujiniku are, in principle, public domain or creative commons 0 unless otherwise stated.  

ただし将来的に、例外的にライセンスを明記したコンテンツが追加される可能性もありうるので、もし、その場合には、それぞれの個別の追加コンテンツについては、その個別のライセンスに従ってください。  
However, in the future, content with the exception of specifying the license may be added, so in that case, please follow the individual license for each individual additional content.  

# サンプルゲームの操作方法 How to operate sample game

動作       |キー  |
:---------|----:|
決定       |Z   |
キャンセル / メニュー開  |X    |
移動      |矢印  |

Action       |Key  |
:---------|----:|
Decide       |Z   |
Cancel / Menu  |X    |
Move      |Arrow  |

# ゲームの起動方法 How to launch this game
現時点では未完成です。

ダウンロードしたら、ダウンロード方法にもよりますが、通常は圧縮されてる状態のファイルなので、まず解凍してください。

Visual Studio では解凍せずともソリューションファイルをダブルクリックすると起動してしまいますが、読み込みエラーになります。

なので、まず解凍が必要です。


Visual Studioを使ってソリューション(sln)ファイルを起動して、ソースファイルをビルドしてください。  
Since this app is incomplete at the moment, please use Visual Studio to build the source code.  

ソリューションファイルの場所は

sujinikuRpgRuntimeCppWin32-master\sujinikuRpgRuntimeCppWin32

です。

バージョンは古いですが、
sujinikuRpgRuntimeCppWin32-master\sujinikuRpgRuntimeCppWin32-master\sujinikuRpgRuntimeCppWin32\x64\Release に、実行ファイルがあります。


# GitHubに上書き保存する場合の保存場所（自分用メモ）
解凍したファイル内の

sujinikuRpgRuntimeCppWin32-master\sujinikuRpgRuntimeCppWin32\sujinikuRpgRuntimeCppWin32

にソースコード sujinikuRpgRuntimeCppWin32.cpp　があるので、GitHUbの対応するフォルダを上書き。

ソリューションファイルがあるフォルダと同じフォルダに「sujinikuRpgRuntimeCppWin32」というフォルダがあるので、そこの中にあるCPPファイルの「sujinikuRpgRuntimeCppWin32」を、GitHubの当リポジトリの該当する同じ場所にアップロードする。


# その他の書類 Other documents
ソースコードの関数や変数などの説明といった、ゲームのコード解説書については `CodeCommentaryBook_jpn.md` に記載されています。  
Specifications of games such as explanations of functions and variables in source code are described in `CodeCommentaryBook_jpn.md` .  

# 更新履歴 update history
2018年9月2日 : 履歴の記録を開始。  
この時点で先月8月中にver0.01がリリースされている。
