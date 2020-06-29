# マラソン用スクリプトメモ
- やることメモ
    1. ディレクトリの作成
        - setup.pyをproblemディレクトリにコピーして実行
        ```
        + problem
        L+ src
        L+ input
        L+ tester
        L+ mgr
        L+ test
        L+ exe
        ```
    1. ジェネレーターをダウンロード or 作成し`tester`に配置
    1. `make_input.py`を`mgr`に配置し実行
        - TEST_NUM, COMMANDを適宜変更
    1. testerの出力をスコアのみに書き換える
    1. run_test.pyのcmdを適宜書き換え、実行
        - 引数
            1. 実行ファイル名
            1. テスト名（リザルトのファイル名）
            1. テスト数を指定（省略可）...TEST_NUMのidxを入力
                - -1で１ケースのみ実行
