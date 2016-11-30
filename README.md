# 2016年度 応用プロジェクト制作（法政大学）
ハードウェアコンテスト[GUGEN](https://gugen.jp/)に応募しました。   
見ていっていただけると幸いです。  
https://gugen.jp/entry2016/2016-124

## プロダクト名 
ACTEE   
<img src="https://gugen.jp/uploads/20161129200853489.png" width="70%">

## プロダクトの詳細 
自らの投げる「動作」により、光の「反応」が返ってくるプロダクト。
公園などで遊ぶ際、もっと面白くさせるのがこのACTEE（アクティー）です。 
### モード説明
#### ◇ノーマルモード◇
投げた時の加速度を検知し、ACTEEが光ります。ブレがなく、上手い投げ方のときに青、普通の投げ方は黄、下手な投げ方をすると赤に光ります。
そして、とてもうまく投げることが出来ると、フラッシュが光り、あたり一面を照らします。  
#### ◇すごろくモード◇
投げた時の加速度を検知し、ACTEEの周りの４５個のLEDライトが一個づつ光ります。
うまさを判定し、最大で６コマ進めます。
ゴールすることが出来るとフラッシュが光ります。 
#### ◇爆弾ゲームモード◇
ACTEEの色が青から赤にだんだん変化していきます。そして、フラッシュの電撃を食らってしまった人が負けです。  
#### ◇アートモード◇
光のアートのような美しい写真が撮ることが出来ます。ACTEEを手に持ち、振り被る加速度により、色が変化します。
### ターゲットユーザー
大人から子供までわいわい楽しめます。

## 使用モジュール 
- Arduino pro mini
- [3軸加速度センサ(アナログ出力)](http://akizukidenshi.com/catalog/g/gM-05153/)
- [マイコン内臓RGBテープLED(NeoPixel)](http://www.akiba-led.jp/product/1474)
- 写ルンです フラッシュ部分
- [Arduino用 Relay Module V3](http://akizukidenshi.com/catalog/g/gM-07031/)
- [フルカラーRGBLED付ロータリーエンコーダ](http://akizukidenshi.com/catalog/g/gP-05773/)

## 参考資料 
- [Adafruit_NeoPixelライブラリ使いかた](http://www.jonki.net/entry/2014/12/17/130536)