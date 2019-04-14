= ある同人サークルにおける電子決済の導入と運用
@<author>{lrks}
//lead{
この物語は、ある同人サークルで電子決済を導入するまでの記録を淡々と述べるものです。過度な期待はしないでください。
あと、部屋は明るくして、本書から3mは離れて見やがってください。
//}


== はじめに
締め切りが厳しいです（4ヶ月ぶり）。
博士課程の学生や社会人で同人誌を書いている人、朝起きられる人、みんな人間辞めているのでは？

人間辞めている限界サークルといえば当サークルですが、2018年12月からPayPayと交通系電子マネー決済を導入しました@<emoji>{congratulations}。
以前から導入している手段と合わせ、「現金」「クレジットカード」「交通系電子マネー」「QRコード」による決済に対応したことになります。

本稿は、これらの支払い手段を導入するまでのいきさつと、即売会でのオペレーションについてまとめたものです。
それではご覧ください。


== 導入編
クレジットカード決済、交通系電子マネー決済、QRコード決済の導入までについて述べます。

=== クレジットカード決済
即売会でクレカ決済といえば？Squareですよ！Square！
ということで、当サークルではSquare株式会社@<fn>{square}が提供するクレジットカード決済を導入しています。
スマートフォンまたはタブレットと、@<img>{contactless_reader}や@<img>{chip_card_reader}などの決済端末を組み合わせることでクレジットカード決済を行えます。
Square株式会社による審査を受ければVisa・Mastercard・AMEXの取り扱いが可能となり、株式会社ジェーシービーによる2次審査次第でJCB、Diners、Discoverの取り扱いが可能です。
//subfig[Squareリーダー]{
//image[contactless_reader][Square Reader][scale=0.49]
//image[chip_card_reader][ICカードリーダー][scale=0.49]
//}
//footnote[square][米Square社の日本法人で、三井住友カード株式会社と提携している。]

下記の特徴があり、利用までのハードルが低いという利点があります。

: 端末価格が安い
  決済端末が約8000円で提供されています。
  これは、据え置き型端末利用の決済サービスはもちろん、他のモバイル決済で利用されているMiura Systemsの端末と比べても半額以下です。
: 申し込みがWebで完結する
  他社の場合だと、電話かメールで詳細問い合わせすると申込書が郵送で届くので署名捺印して返送、という手続きを踏む場合もあります。
  その点Squareは楽です。
: 加盟店審査が寛大…な気がする
  個人かつ催事利用であっても「総合的な判断により残念ながら…」と言われる率が少ない気がします。

決済時にPINコードが使えないためサイン必須という制約はあるものの、最近はEMV Contactless@<fn>{contactless}に対応した端末が提供され、非接触決済に限ればサイン不要になりました。
他のモバイル決済ではあまり対応しておらず、利点といえそうです。
//footnote[contactless][クレジットカードでもSuicaのように非接触で決済できる方式です。ブランドのサービス名は「Visa payWave」や「Master contactless」など。最近発行したクレジットカードやApple Payで利用できることがあります。]

まるでSquareの手先のようになってしまいましたが、即売会でのクレジットカード決済には非常に適していると思います。
サイン必須なのは勘弁してほしい、また三井住友カードと提携しているのでせめてiD決済を、と思ったこともありましたが、EMV Contactlessに対応したのでどうでも良くなってしまいました。

=== 交通系電子マネー編
交通系電子マネー編です。
ここで述べる交通系電子マネーとは、全国相互利用可能な交通系ICカードのうちPiTaPaを除く9種類、具体的にはKitaca・Suica・PASMO・TOICA・manaca・ICOCA・SUGOCA・nimoca・はやかけんのことを指します。

即売会で交通系電子マネー決済を導入するために申し込む業者といえば●●だ！ん、●●はダメになった？え、××は新規不可…？一体どうすれば…。
ということで、決済代行業者数社に手当たり次第申し込んでみました@<fn>{kessai}。
@<table>{suica}は、これまでに加盟店審査の申し込みを行った業者とその審査結果です。
//tabooular[suica][交通系電子マネー取り扱いのために申し込んだ記録]{
申し込み時期 | 社名・サービス名 | 審査結果
============>|<=================|<==============
2017年9月    | 楽天ペイ         | 即落ち
2018年7月    | 楽天ペイ（再挑戦）| 何度かやり取りがあるも否決
      8月    | Airペイ          | 激落ち
      8月    | R社              | 可決の見込みなしで強制終了
      8月    | A-REGI           | 常設店舗が必要で要件を満たせず
      9月    | A社              | 意味のある返事なし
      9月    | United Will      | 何往復かやりとりするも否決
      9月    | J社              | 難しい旨の回答をもらった
//}
//footnote[kessai][審査会社が同じと思われるところはどれか1社に限定して申し込んでいます。なお、法人格が必要だったり、飲食店限定、催事利用不可など条件を満たせないことが明記されているところには当然申し込んでいません。ここで申し込んだところは全て「可決の見込みがある」と思って申し込んだわけです。]

門前払いの波動を感じる。
心意気としては、手数料3.24%と言わず334%でも構わないので導入させてくれって感じだったのですが…。
この頃「加盟店審査　落ちる」でググりまくった結果、ターゲティング広告が「審査に通らない方へ」とかそんな内容ばかりだった気がします。
申込者（代表者）個人の信用情報に傷が付かないよう気を付けたり、審査を行う会社のカード@<fn>{rakuten}を発行したり、0AB~J番号@<fn>{0abj}を用意したり、居住地に配慮する、広告リンクから申し込むなどの努力（？）をしたのですが、空振りでしたね。
//footnote[rakuten][具体的には楽天カードです。2019年4月から楽天ペイ関連事業は楽天ペイメント株式会社が行うことになりますが、審査も行うのか…？審査基準は変わるのか…？が気になります。]
//footnote[0abj][固定電話に割り当てられる番号のことです。]

そんな中、コイニー株式会社@<fn>{coiney}が交通系電子マネー決済の取り扱いを始めるという噂を知りました。
前々から@<img>{terminal_plus}のCoineyターミナルPlusというリーダーで「対応予定」とはなっていましたが、すでに加盟している店舗のTwitterで「まもなく開始という案内が来た」という書き込みがあったのです。
いまのうちに加盟店となれば、今後訪れるであろう交通系電子マネー取り扱いの追加審査が甘くなることが期待できます。
そこで、加盟店審査に申し込んでみたところ、コイニー株式会社が行う1次審査に通過し、Visa・Master・セゾンカードの取り扱いができるようになりました。
これは嬉しい。思わずセゾンカードの発行申し込みをしてしまいました。
ただ残念ながら、2次審査は審査会社@<fn>{jcb}により否決でJCB・AMEX・Diners・Discoverの取り扱いは不可とのことです。
//image[terminal_plus][CoineyターミナルPlus][scale=0.45]
//footnote[coiney][モバイル決済サービス「Coiney」を提供する会社で、株式会社クレディセゾンと提携している。]

一方で、本命の交通系電子マネーは後日行われた審査に通過し、無事取り扱い可能となりました。
これは嬉しい。
審査前に神社へ参拝したことや、審査結果を受け取った時刻にJR線に乗っていたことが効いたのでしょう。
JCBの審査？肉を切らせて骨を断つですよ。
//footnote[jcb][Coineyの規約には「クレジットカード会社等による加盟店審査」とあり具体的な社名は記載されていません。しかし、JCBが入っているので株式会社ジェーシービーなのでは…。]

なお、Coineyで交通系電子マネーを取り扱うにあたっては、いくつか制約があります。
誰でも閲覧可能なサポートページ@<fn>{coiney-support}によると、いくつか気になる記述がありました。

: 対応端末が限られる
  決済に利用できるスマートフォン・タブレットは、一部のiOS端末に限られます。
  それ以外の端末ではサポートが無いどころか、インストールしても交通系電子マネー決済の画面が出ません。
  当サークルで使っているiPad Air2は対象外なため、急遽第6世代のiPadを買いました。
  もう4年以上前の端末なので仕方ないといえばそうですが、予期していなかったですね…。
: 決済アプリのインストールに制限がある
  決済アプリのインストール回数は1アカウント10回までだそうです。
  おそらく超えないとは思いますが、気をつける必要があります。

//footnote[coiney-support][@<href>{https://support.coiney.com/hc/ja/categories/360001182011}]

こうした制約があっても、やはり交通系電子マネーの取り扱いができるのは非常に嬉しく、即売会での交通系電子マネー決済には大変適していると思います。
冒頭で「いまのうちに加盟店となっておけば…」という旨を述べましたが、そこはあまり関係ないようなので、新規申し込みでも取り扱い審査に通るようです。嬉しい！

=== QRコード決済編
即売会でQRコード決済といえば？pixiv PAYですよ！pixiv PAY！
ということで、当サークルではpixiv PAYによる決済を受け入れられるようにしていました。
ただ、前述の交通系電子マネーを導入したせいか利用者がおらず、あまり活用できていないのが現状です。
具体的な言及は控えますが、他の決済サービスでは扱えない商材が取り扱い可能なため、今後当サークルがそうした商材を扱う際には活用するかも知れません。

この他に手頃なQRコード決済は？
ということで、そして当サークルではPayPay@<fn>{paypay}を導入しています。
個人間送金機能を利用したわけではなく、審査を受けてユーザスキャン@<fn>{userscan}（@<img>{yomitoru}）方式の加盟店として登録しています。
ただ、個人間送金との比較を@<table>{paypay}にまとめてみると、それほど大きな違いはないことが分かり、実は個人間送金機能で十分だったのでは？という感が否めません。
//subfig[PayPayの支払い方法（公式サイトより引用）]{
//image[yomitoru][ユーザスキャン][scale=0.49]
//image[miseru][ストアスキャン][scale=0.49]
//}
//tabooular[paypay][加盟店と個人間送金の比較]{
           | 加盟店                   & 個人間送金
==========>|==========================&============
加盟店審査 | 必要                     & 不要
手数料   \ | 2021年9月まで無料        & 無料 \
           | （ユーザスキャンに限る） &
支払側還元 | あり（通常0.5%）         & なし
支払元     | PayPay残高               & PayPay残高
           | Yahoo!マネー             & （特典付与分は除く）
           | クレジットカード         &
振込先     | 銀行口座                 & PayPay残高
//}
//footnote[paypay][PayPayを選んだのは半ば消去法です。LINEは就活で遺恨が…それと審査が厳しいようなので見送りました。メルペイは検討時点でサービスインしていなかったこと、そして催事利用について問い合わせるとあまり乗り気ではないような返答があったため…Origamiは鳶一折紙さんより五河琴里さんのほうがタイプなのでちょっと…。]
//footnote[userscan][客側がQRコードを読む方式のことです。「よみとる決済」「店舗提示型」「カスタマースキャン」などと呼ばれることもありますが、ここでは「ユーザスキャン」で統一します。]

せっかく加盟店になったなら、ストアスキャン@<fn>{storescan}（@<img>{miseru}）方式で決済したいところです。
でも、まず個人では相手にしてもらえないでしょう、その上催事利用？無理では？一体どうすれば…。
ということで、当サークルでは株式会社ユニヴァ・ペイキャストが提供する「UnivaPay StoreApp」経由でストアスキャンを実現しています。
実は法人向けサービスのような気がしていて、個人利用ではもう1社くらい@<fn>{upc}挟む必要がありそうという印象を受けていますが、少なくとも申込に関しては法人・個人を問わないようです。
//footnote[storescan][店側がバーコードを読む方式のことです。「みせる決済」「利用者提示型」「コード支払い」「マーチャントスキャン」などと呼ばれることもありますが、ここでは「ストアスキャン」で統一します。]
//footnote[upc][実際に「pCAT（ルミーズ株式会社）」や「あっと決済（株式会社ビジコム）」が挟まっています。]

申し込みからしばらくすると無事加盟店審査に通過し、ストアスキャン方式でのPayPayがUnivaPayの提供するiOS/Androidアプリ、またはAPIから利用できるようになりました。
UnivaPayではPayPay以外の決済方式@<fn>{docomo}も対応予定とのことで、そちらの審査も通ると良いですね。
//footnote[docomo][2019年4月時点ではd払い・WeChat Pay・Alipayに対応しています。後ろ2つに関してはすでに審査通過となっており、さらにWeChat PayはCoiney側でも利用できるのですが、中国国内で本人確認の済んだアカウントでなければ決済できないためここでは触れません。]


== 運用編
運用編です。
当サークルが即売会で利用しているハードウェア構成と、アプリケーション周りについて述べます。

=== ハードウェア構成
ハードウェア構成を@<img>{hardware}に示します。
iPadを中心として、決済に必要な周辺機器がぶら下がる感じです。
2.4GHz/5GHz帯の無線はなるべく使わない、iPadの充電が切れたら終わりなので常時給電することにしています。
//image[hardware][ハードウェア構成図][scale=1]

このiPadには、第6世代(2018)のWiFi+Cellular（au版）モデルを使っています。
ここで「Cellular」にした理由は、費用を抑えつつも安定した通信環境を実現するためにApple SIMが必要だったからです。
Apple SIMを利用したことで、auの「LTEデータプリペイド」という30日間有効、初期費用・手数料なし、1GB1500円（税抜）な回線が使えるようになりました。
GB単価は高めな気がしますが、大手キャリアの回線ということで安心できます。
さらに最低利用期間もないため即売会のときだけ契約することも可能、ということでもしかすると格安SIMよりも安くなるかも知れません。
とはいえau回線に死なれると厳しいですが、そのときは諦めます。

さらに、このiPadには「Lightning - USB 3カメラアダプタ」が接続されており、そこに「DENSO AT20Q-SM」というバーコードリーダーが繋がっています。
1次元バーコードに加え、QRコードや画面に表示されたコードの読み取りにも対応しており、頒布物のバーコードやQR決済でのストアスキャンに利用するつもりです。
なお、@<img>{hardware}を見るとiPadには「外部機器」も点線で繋がっているように見えませんか？
これは将来の拡張用で、例えばカスタマディスプレイ@<fn>{cd}を接続したくなったときに利用します。
実際に繋ぐ場合はEthernetで接続し@<fn>{ethernet}、その間に外部機器の操作をHTTPか何かで叩けるようにした装置を挟み込むとiPad側のアプリケーションから操作しやすくなるんじゃないですかね。
//footnote[cd][昔は接続していました。この辺の詳細が載った「COSMIC L0 Vol.2」という本があるらしいんですけど…。@<href>{https://booth.pm/ja/items/489108}@<qrcode>{https://booth.pm/ja/items/489108}]
//footnote[ethernet][USB-Ethernetアダプタが必要になるでしょう。そして「Lightning - USB 3カメラアダプタ」のUSBポートは1つしかないため、バーコードリーダーと共存させるためにUSBハブも必要となるでしょう。]

そして、決済端末として「Square Reader」と「CoineyターミナルPlus」が接続されています。
特筆すべきはSquare Readerで、海外で先行販売されていた機器をそのまま持ってきたわけではなく、日本での提供開始に合わせてモデルチェンジ@<fn>{felica}したようです。
これまでに存在しているSquare Readerとその比較をFCC@<fn>{fcc}や技適を基に探すと@<table>{square-reader}のとおりになりました。
//tabooular[square-reader][Square Reader]{
FCC ID      | 技術基準適合証明等 | FCC申請日（初回）
<===========|====================|===================
2AF3K-SPC1  | あり（007-AG0225） | 2019-01-02
2AF3K-SHR1  | なし               | 2017-01-11
2AF3K-1SQHW | なし               | 2015-12-02
//}
//footnote[felica][プレスリリースの際に、海外モデルでは非対応だったFelicaへ対応した旨も述べられていました。]
//footnote[fcc][米国連邦通信委員会のことです。電波を放射する機器について、日本でいう「技適」の審査を行なっています。この審査の際、メーカーは機器のマニュアルや電波放射試験データを提出し、それが後に一般公開されるため、メーカーがあまり宣伝していないような情報を知ることができます。]

FCCに掲載されている内部の写真を見ると、SPC1ではSHR1および1SQHWに比べて基板レイアウトに変更があり、Squareマークの入ったICも変更されているように見えます。
また、測定誤差かも知れませんが、Bluetoothの出力が上がっているようです。

とはいえ、混雑した会場でBluetoothを使うのは非常に不安です。
Square Standという専用スタンドと、それに同梱される専用のUSBハブを使えばUSBで接続できるため、幸せになれるかも知れません@<fn>{unhappy}。
価格は32,980円とためらう値段ですが、米国では$199で売られています。
それでためらいが減ったので買ってしまったところ、確かにSquare Readerが@<img>{square-usb}のようにUSBで接続され、幸せになれました@<fn>{inapp}。
//image[square-usb][Square ReaderがUSBで接続される様子]
Square Standのハードウェアは各国で違いはないと思われますが、この「Square Stand（米国版）」+「Square Reader（国内版）」+「Squareアプリ（国内版）」という構成でうまくいって良かったです。
//footnote[unhappy][これを使わなくても幸せになりたくて、「Lightning - USB 3カメラアダプタ」と「Square Reader 専用ドック」を経由して繋いでみたものの、給電が行われるだけでリンク確立はBluetoothのみでした。Square Standに同梱される専用USBハブに秘密がありそうです。]
//footnote[inapp][Squareアプリ内では、本稿でいうところのSquare Readerが「Square Reader（Bluetooth 接続）」という表記になっているため、「Square Reader（Bluetooth 接続）がUSB接続されている」という旨のメッセージが出ることもあり、少し違和感があります…。]

なお、米国内にて@<fn>{legal}「Square Reader（米国版）」と「Squareアプリ（国内版）」のBluetooth接続も試したところ、こちらも一応は@<img>{square-bluetooth}のように接続できました。
ただ、ファームウェアアップデートの通知が出たにも関わらず、一向にアップデートが開始される気配がないなど、少々不穏です。
決済も行っていない@<fn>{legal-merchant}ため、本当に正しく接続できているかは不明ですが、国内で接続することはまずないので問題ありません。
Square Stand経由で繋げば有線での利用となるため、技適にも加盟店規約にも触れなさそうですが、あまり変なことはしないほうが良いですね。
//image[square-bluetooth][Square Reader（海外版）とSquareアプリ（国内版）を接続した様子][scale=0.45]
//footnote[legal][Square Reader（米国版）は技適を通っていないため、国内で利用すると電波法に違反する恐れがあります。当サークルでは法令に違反する、または違反の恐れがある行為はしません。]
//footnote[legal-merchant][加盟店規約により国外での決済は禁止されています。当サークルでは加盟店規約に違反する、または違反の恐れがある行為はしません。]

何はともあれ、これでSquare StandとSquare ReaderがUSBで接続できたのでヨシ！と思ったのですが、そうもいきません。
Square Standに同梱される専用USBハブが曲者で、どうやらSquareアプリを使っているときにしか有効にならないようです。
ここにバーコードリーダーを接続しても、Squareアプリからしか使えないじゃないですか……しかもSquareアプリは先ほど用意した「DENSO AT20Q-SM」に対応しておらず、それでは意味がなくなります。

ということで、Square Standの利用は保留です。
今後、Square Reader（USB接続）とバーコードリーダーを同時に利用できるような方法を探っていきたいと思います。


=== アプリケーション周り
iPadの中で動くアプリについてです。
SquareやCoineyで決済を行うには、各サービスが用意したアプリを起動して金額を入力する必要があり、手間がかかります。
そこで当サークルでは、各アプリと連携して起動と金額入力を自動で行う仕組みをネイティブアプリとして実装し@<fn>{implements}利用しています。
その画面構成図は@<img>{software}のとおりです。
//image[software][アプリケーション画面構成図]
//footnote[implements][機能および画面は開発中のものです。]

ここでSquareとの連携には、「Point of Sale API」を用いています。
誰でも見られる仕様書@<fn>{pos-api}によると内部ではCustom URL Schemeが利用されており、アプリ起動と決済金額の通知、さらに決済後は元のアプリに戻ってくることが可能です。
ただ、Square Readerとのリンク確立はアプリ起動後に行われるため、運が悪いとワンテンポ遅れてしまいます。
自分のアプリからSquare Readerを扱える「Reader SDK」が使えれば良いのですが、あいにく日本のアカウントでは有効にできません。
あとは結構な力技ですが、iPadのマルチタスク機能であるSplit Viewを用いて、Squareのアプリと自分のアプリを同時実行させれば、なんとかなることが期待できます。
さらに相当な力技で、Square Reader専用のiPhone/Android端末を用意しておき、その端末をスイッチコントロール@<fn>{switch-control}やadb@<fn>{adb}から操作ということも…いやこれは厳しさしか感じないですね。
//footnote[pos-api][@<href>{https://docs.connect.squareup.com/payments/pos/overview}]
//footnote[switch-control][iOSにおけるアクセシビリティ機能のひとつで、うまくやるとキーボード入力をタップ操作に変換できます。]
//footnote[adb][Androidのデバック機能で、うまくやらなくても外部からタップ操作のイベントを発生させられます。]

Coineyとの連携には、「CoineyKit」というSDKを用いています。
仕様は問い合わせにより開示されますが、内容は規約により保護されているため何も述べられません。
開発者向けの規約ではありますが、不適切と判断されて加盟店契約まで切られたら大変です。
もし加盟店情報交換センターに強制解除の情報が載った場合、他の決済サービスからも契約解除に至ることでしょう。
Coineyが使っているMiura Systemsの端末は、たびたびセキュリティの問題が指摘されていたり@<fn>{blackhat}@<fn>{square-miura}、FCCにはLCDテストや画面書き換えの手順も載っていたりして少し試したい気もありますが、攻めたことはやめます。
決済サービスの規約と公理教会が定めた禁忌目録を逸脱する行為はしません。
//footnote[blackhat][@<href>{https://www.blackhat.com/us-18/briefings/schedule/index.html#for-the-love-of-money-finding-and-exploiting-vulnerabilities-in-mobile-point-of-sales-systems-10992}]
//footnote[square-miura][米国内の話ですが「この件を受けてSquareはMiura Systems製リーダーを廃止した」と報じているニュースサイトもあり、昔は使えた…？どうしたらそんなことに…？という驚きでいっぱいです。]

アプリケーション周りで述べたいところは以上で、あとは特にありません。
おわり。


== おわりに
本稿では、当サークルにおける電子決済の導入と運用について述べました。

まだアプリケーションの話になりますが、今後は取り置きや事前決済@<fn>{jizen}にも対応したいです。
iOSのWalletアプリに登録できるPassを発行する、そのPassにはQRコードか何かを書いておいてバーコードリーダーで読んでユーザを識別ってだけならさほど手間はかからないのでは？
でもどうせなら、Miura SystemsのSDKやら何やらを使って非接触で読み取れるようにしたいところです。
//footnote[jizen][ただし、規約に触れる可能性もあるので、確認のうえ慎重に対応します。]

実現できるかは分かりませんでした！
いかがでしたか？
おわり。
