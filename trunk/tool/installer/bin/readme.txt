インストーラーパッケージ

主な機能
・かなり簡略化されたインストーラー作成ツール
・ファイルの転送（ＭＤ５による破損チェック機能あり）
・レジストリの書き込み
・インストール終了後、任意の実行ファイルの起動可能
・アンインストール時に任意のフォルダの削除対応
・ＵＡＣを考慮した設計
・対応ＯＳ：windosXP/Vista


使い方
・maidsetup.exeがあるフォルダにある config.xml が設定ファイルとなっています
・設定ファイルに書いてあるコメントを見ればわかると思います
・設定ファイル内にある[XXX] の文字は環境変数みたいなもので
　インストーラーが起動時に環境をしらべて自動的に変換してくれます
　[ProgramFiles]	"C:\Program Files" 等
　[Windows]			"C:\Windows" 等
　[SourceFolder]	config.xml があるフォルダ。普通はディスクとかになります。
　[TargetFolder]	インストーラーを起動後、「インストールするフォルダ」で設定してあるとこ
　[UserProgramMenu]	<user name>\Start Menu\Programs
　[UserDeskTop]		<user name>\Desktop
　[UserAppData]		<user name>\Application Data
　[CommonAppData]		<All Users>\Start Menu\Programs
　[CommonProgramMenu]	<All Users>\Desktop
　[CommonDeskTop]		<All Users>\Application Data
　[Uninstaller]			アンインストーラーが置かれるパス（[ProgramFiles]\maid_install_app\maiduninstaller.exe）
　[Uninstallercommand]	アンインストーラーに送り込む設定ファイルのパス


・簡単な挙動
　maidsetup.exe(asInvoker)
　　[UserAppData]\maidsetup ディレクトリを作成、そこにmaidconfig.exe を送り込む
　　maidconfig.exe "config.xml" として起動

　maidconfig.exe(asInvoker)
　　config.xml内にあるインストールの設定を選択してもらう画面
　　選択後、config.xml から選択された部分を取り出して、installprogram.xml　を作成
　　maidinstaller.exe も用意して
　　maidinstaller.exe "installprogram.xml" として起動

　maidinstaller.exe(requireAdministrator)
　　installprogram.xmlに指定されたとおりにコピー、レジストリの作成を行う
　　最後に[ProgramFiles]\maid_install_app ディレクトリを作成して
　　maiduninstaller.exe を作成する
　　インストールログは [TargetFolder]\uninstall.xml に書き込まれます

　maiduninstaller.exe(requireAdministrator)
　　uninstall.xml　にしたがって削除する

