= ワイ「ち、チノちゃん！バ○スタのHackしないで！」
@<author>{街の国際ウィザード・バリスター, lrks;twitter}
//lead{
数週間前、念願の同人誌即売会に当選したのだが、『技術力が注目されないなら奇行に走るしかないのでは』という懸念の声があり、結果、あのコピペを改変してネスカフェバリスタの解析記事を書くことになった。
この記事では、バリスタ内部で利用されるI2CバスとBluetoothそれぞれの通信内容を調べ、外部からの操作やHTCPCPの実装を行なっているのだが、なかなか思い通りにいかない場面もあって心イタイイタイなのだった。
//}


== チノ「はじめにですね……」
//noindent
―― ある日の喫茶・ラビットハウスにて ――
チノ「働き方改革の一環で、低価格帯のメニューはインスタントコーヒーを使うことになりました。生産性向上が目的なので、インスタントコーヒーをいれるための専用マシンを使いましょう。」
ワイ「あらかじめ用意しておいたのが、@<img>{pm9631}に示す『ネスカフェバリスタPM9631』と@<img>{barista-i}の『ネスカフェバリスタアイ』になります。」
//subfig[ラビットハウスに導入されたバリスタ]{
//image[pm9631][PM9631][scale=0.3]
//image[barista-i][バリスタアイ][scale=0.3]
//}
チノ「お客様に気付かれないよう、コーヒーを淹れているふりをしてバリスタを使わなければなりません。」
ワイ「難しそうだね…。」
チノ「そこでOpenPoseか何かで動作を検知し、それをトリガーにバリスタを人の手を介さずに操作することにします。」
ワイ「え、そんなことできるの！？」
チノ「OpenPoseはともかく、バリスタを遠隔操作することはできます。次章ではPM9631の操作、またその次ではバリスタアイの操作、そして最後に応用編としてHTCPCPの実装を行います。」


== ワイ「PM9631の操作？」
ワイ「チノちゃん、バリスタの遠隔操作ってどういうこと？」
チノ「ではまず、PM9631を例にして説明します。」



== おわりに
くぅ〜疲れましたw
