# 概要

このモジュールはApache2.2のフィルターとして動作します。  
任意のスクリプト言語からのレスポンス出力時、キー名称が `X_HDRNT_` で始まる全てのHTTPレスポンスヘッダのキーと値をApacheのnoteへ書き換えます（元のHTTPレスポンスヘッダは削除されます）。

※ Apacheの内部仕様により `X_HDRNT_` は `x_hdrnt_` のように小文字でもOKです。

アプリケーションのデバッグ情報や分析用の情報をApacheのログファイルへ出力したい場合にご利用ください。

本モジュールはApacheのnoteをサポートしていないスクリプト言語での利用を想定しています。例えばPHPの場合は apache_note() 関数でその機能はサポートされていますのでそちらをご利用ください。またクライアント端末に内容が渡ってしまうのを気にしなければ、本モジュールを使用しなくてもHTTPレスポンスヘッダの値をそのままApacheのログファイルへ出力することはできます。

# コンパイル&インストール例

    /usr/sbin/apxs -c mod_header_note.c
    sudo install .libs/mod_header_note.so /usr/lib64/httpd/modules/

# httpd.confへの記載例

    LoadModule header_note_module modules/mod_header_note.so
    SetOutputFilter header_note_output_filter

# Apacheカスタムログ設定の例

    LogFormat "%h %l %u %t \"%r\" %>s %b %{x_hdrnt_hoge}n" common

