= アイドルマスターとデータ分析＋α（総集編その1）
@<author>{itezaP;twitter:sugerest}

//profile{
今回のタイトルに「その1」とつけましたが、次が出るかはわかりません。
//}

//lead{
総集編ということで、万策尽きました。
//}

== 本記事について

こんにちは、はじめましての人ははじめまして、itezaPと申します。
主にアイドルマスターを主軸としたデータ分析についての記事を書いています。
2016年の冬コミC91でSIGCOWWが処女作「COMIC L0 vol.1」を発行し、約2年間でvol.6まで発行されました。
この2年間でx院（xは任意の文字列）をなんとか出所できた人、志願して懲役を延長した人など、環境が大きく変化しました。
ですが、新刊を一度も落とさなかったのは奇跡だと思います。

今回の記事は、私がvol.6までに書いた記事を再録・加筆修正したものをまとめた総集編となります。
具体的にはvol.1，4，5，6の内容のマージや加筆修正を行いました。
当時の記事をそのまま再録したものもあります。
vol.2，3については禁則事項です。
変更点や修正部分がある場合には、章のはじめに詳細を記述しています。

== 身体的特徴を使ったアイマスキャラ分析

本記事は、2016年12月に発行した「L0 Vol.1」に掲載した「メガ少女クラスタリングシュート」、
ならびに2018年12月に発行した「L0 Vol.6」に掲載した「アイマスキャラの回帰モデルつくってみた（試作）」の記事をマージ・加筆修正したものになります。
この章の内容は、回帰モデルによるデータ分析がVol.6に該当し、クラスタリングによるデータ分析がVol.1に該当します。

これらの記事の執筆から月日がたち、新しいアイドルたちが数多く増えました。
そのため、シャイニーカラーズや新たなシンデレラガールズのキャラたちをデータセットに加えて追加実験を行いました。

=== データセット

データは各キャラの年齢、身長、体重、3サイズを対象とします。
各キャラの情報は「im@sparql」のAPIを使用して取得しました@<fn>{imasparql}。
シンデレラガールズのキャラデータは取得できた188人中、185人のデータを使用します。
3サイズが不明の3人 （佐藤心、大沼くるみ、双葉杏）は、今回のデータから除外としました。
なお、7代目シンデレラガールズこと安部菜々は年齢が不明なのですが、本人の名誉のため17歳とします。
また夢見りあむの3サイズはプロフィール欄では不明なのですが、セリフにてバストが95との情報があるので、そちらを使用します。
ミリオンライブは取得できた全54人 （765組の13人を含む）を対象とします。
さらにシャイニーカラーズの16人も対象とします。
//footnote[imasparql][@<href>{https://sparql.crssnky.xyz/imas/}]

ここで、データ分析に使うために2つのサブデータセットを用意します。
1つはシンデレラガールズ（新キャラの5人を除く）の180人のデータを訓練データとし、それ以外のデータをテストデータとするデータセットです。
テストデータは、ミリオンライブとシャイニーカラーズのキャラ達を合わせたデータセットをテストデータ1とし、シンデレラガールズの新キャラ5人のデータをテストデータ2とします。
ここでのシンデレラガールズの新キャラ5人とは、辻野あかり、砂塚あきら、夢見りあむ、黒埼ちとせ、白雪千夜を指します。
回帰モデルによるデータ分析では、上記のデータセットを使用します。
もう1つは、各シリーズの12歳以下のデータを集合とするデータセットです。
クラスタリングによるデータ分析では、このデータセットを使用します。

=== 回帰モデルによる分析

2018年11月から12月にかけて行われた「THE IDOLM@STER CINDERELLA GIRLS 6thLIVE MERRY-GO-ROUNDOME!!!」にて、新たなキャラが追加されることが発表されました。
シンデレラガールズに新キャラが追加されるのは、実に約4年ぶりだそうです。
すでに濃すぎる面々が揃っているシンデレラガールズに新キャラ、いったいどんなぶっ飛んだキャラが来るのでしょうか。
二番煎じにならないよう、同シリーズはもちろん、他シリーズ （ミリオンライブやシャイニーカラーズ）のキャラとも被らないような設定になることが予想されます。
そこで新キャラがどんな身体的特徴になるのかを知る足掛かりとして、現在登場しているキャラの身体的特徴の分布や傾向をつかみたいと思います。
調査する方法は様々ありますが、今回は個人的に少し勉強した回帰分析を使って、いろいろやってみたいと思います。
あわよくば、作った回帰式で新キャラのプロフィール予測などができれば、と思っています （結果はお察しください）。

==== 回帰分析

回帰とは、予測したい変数 （目的変数）を説明したい変数 （説明変数）でどの程度説明・予測できるかを分析する手法です。
目的変数と説明変数を使った式を回帰式 （回帰モデル）といい、回帰式を導出することを回帰分析といいます。
1つ （1種類）の説明変数@<m>{x}で目的変数@<m>{y}を表す式を単回帰式、式の導出を単回帰分析と呼びます。
一方、2つ以上 （2種類以上）の説明変数を用いる場合は、重回帰分析によって重回帰式を求めます。
以下に最も単純な単回帰式を示します。
@<m>{\beta_{1}}は傾き、@<m>{\beta_{0}}は切片です。

//texequation{
y = \beta_{1}x + \beta_{0}
//}

回帰式の@<m>{\beta_{1}}と@<m>{\beta_{0}}は、実際のデータとの誤差が最小となるようにします。
誤差が最小となるような@<m>{\beta_{1}}と@<m>{\beta_{0}}を決定するために、最小二乗法と呼ばれる手法を使用します。
最小二乗法によって求めた@<m>{\beta_{1}}や@<m>{\beta_{0}}を偏回帰係数と呼びます。

求めた回帰式がデータに対してどれくらい説明できているかを表す指標を決定係数と呼びます。
決定係数は@<m>{R^{2}}で示され、0から1の範囲の値をとります。
1に近いほど、回帰式の説明変数が目的変数をよく説明できている （よく予測できている）、と言えます。

==== 単回帰式（線形）の導出

回帰式を導出する際、ただ闇雲に式を導出・評価するのは効率が悪いのは明白です。
取得したデータ分布 （例えば年齢と身長など）に相関がありそうかを散布図で眺めて狙いをつけます。
例えば、シンデレラガールズたちの年齢と身長・バスト・ヒップの散布図を@<img>{cinderella_hbh_age}に示します。

//imagew[cinderella_hbh_age][年齢と身長・バスト・ヒップの散布図]

例えば、年齢と身長やバストはやや正の相関がありそうですね。
正の相関があるということは、年齢が上がるにつれて身長やバストが増加する、という意味になります（私は年齢が低く、身長や胸が小さいのが好きです）。
それなりに相関があるものは精度の良い （学習データに対して、決定係数が1に近い）回帰式が構築できそうです。
一方、ヒップは身長・バストと比べるとデータが全体に広がっている （散らばっている）印象です。
つまり、年齢が上がるにつれてヒップも大きくなるとは言いにくい、ということです。

では、年齢を説明変数、身長を目的変数とした線形の単回帰式を求めたいと思います。
構築した回帰式のプロット図を@<img>{cinderella_age_height_model}に示します。
図中の丸点は各データ、直線は回帰式のプロットを示します。
さらに左上には回帰式と決定係数を示します。

//image[cinderella_age_height_model][年齢と身長の線形単回帰式プロット]

決定係数が0.46程度と思ったほどよい回帰式にはなりませんでした。
この結果から、シンデレラガールズのキャラは身長と年齢が比例して大きくなる傾向にあるが、そうではないキャラもそれなりにいるということになります。
それはシンデレラガールズのキャラ達の多様性が大きな要因と考えられます。
例えば、諸星きらりのように17歳で180cm以上あるキャラもいれば、片桐早苗のように28歳で150㎝程度のキャラもたくさんいます。
様々な特徴のキャラがいることによって担当のキャラが見つかる確率が上がるので、とてもよいことだと思います。

もう一例として、年齢とバストの線形単回帰式を求めます。
結果のプロット図を@<img>{cinderella_age_bust_model}に示します。

//image[cinderella_age_bust_model][年齢とバストの線形単回帰式プロット]

決定係数は0.4程度となり、身長の時よりも精度は落ちています。
この結果から、身長よりバストのほうが多様性がある、ということですね。

==== 単回帰式（非線形）の導出

先ほどは年齢と身長・バストを対象に線形の単回帰式を求めました。
ですが、それぞれの散布図を見た限り、直線より曲線のほうが良い回帰式になりそうです。
そこで、次は非線形の単回帰式を求めてみたいと思います。
年齢と身長の非線形回帰式のプロットを@<img>{cinderella_age_height_polymodel}に、年齢とバストの非線形回帰式のプロットを@<img>{cinderella_age_bust_polymodel}に示します。

//image[cinderella_age_height_polymodel][年齢と身長の非線形単回帰式プロット]

//image[cinderella_age_bust_polymodel][年齢とバストの非線形単回帰式プロット]

決定係数が0.6と0.53になり、どちらも0.1以上の向上が見られました。
なのでこれらのデータに関しては、直線よりも曲線のほうがよい回帰式になることがわかりました。
しかし決定係数がまだ0.6程度なので、これがベストな回帰式だ、とは言いにくいと考えられます。

回帰式をより複雑にすれば精度は向上しますが、その場合には過学習の問題が発生します。
ここでの過学習とは、学習データに合わせすぎた回帰式を求めてしまい、他のデータや未知のデータに対しての汎用性が悪くなってしまう、というものです。
回帰式の汎用性と予測精度の両立は難しい問題です。

==== 単回帰式の性能評価

テストデータ1を使って、求めた単回帰式の性能評価をしてみましょう。
線形単回帰式のプロットを@<img>{MilliShiny_age_hb_model_pred}に、非線形単回帰式のプロットを@<img>{MilliShiny_age_hb_polymodel_pred}に示します。
図中の三角はテストデータを表し、@<m>{R^{2}}はテストデータの説明・予測の精度を示しています。

//imagew[MilliShiny_age_hb_model_pred][線形単回帰式プロット]

//imagew[MilliShiny_age_hb_polymodel_pred][非線形単回帰式プロット]

…線形・非線形ともによい精度とは言えませんね。
年齢と身長の回帰式においては、非線形でも決定係数が0.16とかなり低くなりました。
一方、年齢とバストの回帰式においては決定係数が0．29と多少高くなりました。
この結果より、学習データからテストデータを説明・予測することは難しいといえます。
言い換えると、シンデレラガールズとミリオンライブ＋シャイニーカラーズにおいては、同年齢層の身長やバストのデータ分布は異なると考えられます。
なるべく他シリーズのキャラと特徴が被らないようデザインされていることがわかりますね。

==== 重回帰式の導出と評価

今までは説明変数が一つの場合のみを考えましたが、それほど高い精度は得られませんでした。
そこで説明変数を増やした場合にどれくらい精度が上がるのか調査します。
今回は年齢・体重・3サイズから身長を予測する重回帰式を求めます。
さらに年齢・身長・体重・ウエスト・ヒップからバストを予測する重回帰式も求めます。

重回帰式は説明変数が複数あり図示できないので、導出した回帰式の標準化偏回帰係数を@<table>{multi_reg_coeff}に示します。
ここでの標準化偏回帰係数とは、目的変数にどれくらい影響があるかを表す偏回帰係数です。
通常、偏回帰係数は大小を直接比較することはできません （各データの単位などが異なることがあるため）。
ですが、説明変数と目的変数をそれぞれ標準化 （平均0、分散1となるようにデータを変換すること）することで同じ尺度に揃えることができ、比較が可能になります。
標準化偏回帰変数は大きいほど、目的変数への影響力が高いです。
身長を予測する重回帰式をモデル1、バストを予測する重回帰式をモデル2とします。
各標準化偏回帰係数は、小数点第3位で四捨五入しています。

//tabooular[multi_reg_coeff][各モデルの標準化偏回帰係数一覧]{
説明変数	|	モデル1	&	モデル2
==========>|====&====
年齢	|	0.22	|	0.08	
身長	|	*	|	-0.09	
体重	|	0.76	|	0.40	
バスト	|	-0.19	|	*	
ウエスト	|	0.26	|	0.51	
ヒップ	|	-0.21	|	-0.06
//}

決定係数は、モデル1が0.76、モデル2が0.85とそこそこ良い回帰式ができました。
各係数の数値を見ると、身長を予測するには体重が最も影響力が高く、バストを予測するにはウエストが最も影響力が高いことがわかりました。
身長に一番影響を与えるのは体重というのは直感的にわかりますが、バストに一番影響を与えるのがウエストなのは少し意外でした （体重だと思っていたので）。

テストデータ1を使って、導出した重回帰式の性能評価を行いましょう。
その結果、モデル1の決定係数は0.6、モデル2が0.74と、単回帰の場合と比較してかなり良くなりました。
この予測精度なら、アイマスキャラの回帰モデルができた、といってもいいかもしれません。

==== 新たなキャラ達とその予測結果

2018年12月から2019年2月にかけて、新たなキャラがシンデレラガールズに追加されました。
辻野あかりちゃん、砂塚あきらちゃん、夢見りあむさんのお三方です。
彼女らの共通点として、SNS・ゲーム実況・アイドルといった、なんとも現代的な特徴があることが挙げられます。
また2019年2月下旬には黒埼ちとせさん、白雪千夜ちゃんがスターライトステージに実装されました。
どのキャラも既存キャラに負けない強烈なキャラ付けがなされています。

回帰モデルを作った目的の1つとして、新キャラの身体的特徴の予測があります。
テストデータ2を使用して、構築した回帰モデルに当てはめ、どの程度予測できているのかを調査します。
回帰モデルは、比較的精度の高い非線形の単回帰モデルを使用します。
単回帰モデルを@<img>{new_dere_age_hb}に示します。
図中の各点は、彼女たちのデータをプロットしたものになります。
また、各キャラの実測値と予測値の誤差を@<table>{new_dere_pred}に示します。
ここでの予測誤差は小数点第3位で四捨五入した絶対値としました。

//imagew[new_dere_age_hb][非線形単回帰モデルと新キャラデータのプロット]

//tabooular[new_dere_pred][各キャラの予測誤差]{
予測変数	|	身長	&	バスト
==========>|====&====
辻野あかり	|	6.88	|	1.21	
砂塚あきら	|	0.88	|	0.79	
夢見りあむ	|	11.99	|	10.62
黒埼ちとせ	|	3.02	|	1.62	
白雪千夜	|	0.57	|	9.96
//}

この結果から最も予測に近い子はあきらになりました。
身長・バストともに1cmの誤差なのでかなり良い精度だと思います。
次点でちとせが近く、身長の誤差が多少大きいですが、それなりの精度です。
あかりはバストの予測結果は良いですが、身長の誤差は多少大きくなってしまいました。
一方、りあむはどちらも10cm以上の誤差があり、千夜のバストも10cm近い誤差があります。
たしかにりあむはイラストを見る限りでも、他の子とは大きく特徴が異なることがわかります。
千夜も17歳で72ということで（お察し）です。
ですが、貧乳はステータスという格言があるように、小さいのも魅力の1つです。

以上のことから、あかりやあきら、ちとせは今回の回帰モデルから予測可能なキャラといえると思います。
今回追加されたキャラたちの範囲では、そこそこ良い回帰モデルができたかと思います。
一方、りあむや千夜のプロフィールは他の子と大きく異なることから、今回のモデルからの予測は困難です。
他のデータと大きく異なる傾向にあるデータを予測することは難しい問題です。

=== クラスタリングによる年少組の分類

シンデレラガールズやミリオンライブの年少組の発育具合ってどのくらいなのでしょうか。
誰しもが一度は考えたことのある当たり前の疑問だと思います。
例えば「ありすって12才グループでもスレンダーな方だよな」とか「育ちゃん、これで10才か」など様々だと思います。
また同時に、クラスタリングでいい感じに年齢別（発育別）に分類できないか、と考えました。
そこで年少組を対象にクラスタリングを使って年齢別に分類し、発育の度合いを測れるか調査しました。
今回は主成分分析とクラスタリングを組み合わせて分類を行い、その分類結果を可視化したいと思います。

==== 主成分分析

主成分分析を一言で表すと、次元を圧縮する手法です。
多くの次元（特徴量）を持つデータの情報量をなるべく損なわずに、低次元の特徴量に縮約します。
この手法の大きなメリットとして、多次元データを2次元や3次元に圧縮することでデータを可視化することができる点です。

==== クラスタリング

クラスタリングは、複数のデータをいくつかのグループに分割する、教師なし学習の1つです。
ここでの教師なし学習とは、入力データの情報のみから、もっともらしい法則や構造を見つけ出す学習のことです。
そのため、正解はわからないけど何らかの共通の特徴を持つグループを発見したい、といった場合にこの手法を使用します。

今回使用するクラスタリングは簡単かつ定番のK-means法を使用しました。
K-means法の大まかな流れは以下のような流れになります。

 1. 各点（各データ）にランダムにクラスタを割り振る
 2. 各クラスタの重心を計算する
 3. 各点のクラスタをもっとも重心に近いクラスタに更新する
 4. 更新しなくなるまで2. ~ 4.を繰り返す

各クラスタの重心は、各クラスタ内の全ての点の平均座標としました。
また、距離の尺度はユークリッド距離を使用しました。

==== クラスタリング結果

シンデレラガールズの幼少組（9~12才）の身体データを使って発育をチェックしてみましょう。
ご存知とは思いますが、ここで幼少組のイラスト一覧を@<img>{charlist_dere}に示します。

//imagew[charlist_dere][シンデレラガールズ幼少組の一覧]

今回は主成分分析によって6次元データを2次元に圧縮し、クラスタリングを行います。
2次元キャラの6次元データを2次元に圧縮します（字面だけみると不思議な文章です）。
身体データは各キャラの年齢、身長、体重、バスト、ウェスト、ヒップを使います。
クラスタ数は年齢の幅と同じ4とします。

主成分分析、クラスタリングを行った結果を@<table>{lori_cluster1}に示します。
クラスタリングの可視化結果を@<img>{lori_dere_kmeans4}に示します。

//tabooular[lori_cluster1][幼少組クラスタリングの結果1]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
赤城みりあ	|	12	|	11	
市原仁奈	|	9	|	9	
櫻井桃華	|	11	|	12	
佐々木千枝	|	12	|	11	
橘ありす	|	10	|	12	
遊佐こずえ	|	9	|	11
横山千佳	|	9	|	9	
龍崎薫	|	9	|	9	
佐城雪美	|	10	|	10	
福山舞	|	9	|	10	
メアリー・コクラン	|	11	|	11	
古賀小春	|	12	|	12
城ヶ崎莉嘉	|	11	|	12	
的場梨沙	|	12	|	12	
結城晴	|	12	|	12
//}

//image[lori_dere_kmeans4][幼少組クラスタリングの可視化結果1]

ちゃまや莉嘉が11才のクラスタに所属してしまいました。
他の子たちも若干の年齢のズレがあります。
随分発育のよい11才ですね。

実際の年齢と年齢クラスタが異なる原因はなんでしょうか。
考えられる原因として、単位の違うデータ(今回は年齢と身長とか)をひとくくりにしてしまっているので、数値の大きいデータに引っ張られている可能性があります。
なので、主成分分析をする前のデータを標準化します。
標準化することによってデータの分散を統一することができます。
標準化は全ての値に対してその変数の平均を引いて、その変数の標準偏差で割ることで得られます。
各データを標準化して、もう一度クラスタリングした結果を@<table>{lori_cluster2}に示します。
クラスタリングの可視化結果を@<img>{lori_dere_nkmeans4}に示します。

//tabooular[lori_cluster2][幼少組クラスタリングの結果2]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
赤城みりあ	|	12	|	11	
市原仁奈	|	9	|	9	
櫻井桃華	|	12	|	12	
佐々木千枝	|	11	|	11	
橘ありす	|	11	|	12	
遊佐こずえ	|	10	|	11
横山千佳	|	9	|	9	
龍崎薫	|	10	|	9	
佐城雪美	|	10	|	10	
福山舞	|	9	|	10	
メアリー・コクラン	|	12	|	11	
古賀小春	|	12	|	12
城ヶ崎莉嘉	|	12	|	12	
的場梨沙	|	12	|	12	
結城晴	|	12	|	12
//}

//image[lori_dere_nkmeans4][幼少組クラスタリングの可視化結果2]

異なるクラスタに属する人数が8人から6人に減少しました。
実年齢より上のクラスタに属する子（発育の良い子）、下のクラスタに属する子（これからに期待の子）がいますね。
例えばありすは12才ですが、11才のクラスタに属しています。
12才クラスタの面々のプロフィールと比較すると、多少数値が見劣りするかもしれません。
ですが、そこもまた魅力の一つだと思います。
僕は小さいのが好きです。

次にミリオンライブの年少組のデータを加えてクラスタリングしてみます。
ミリオンライブ年少組の一覧を@<img>{charlist_milli}に示します。
クラスタリング結果を@<table>{lori_cluster3}に、可視化結果を@<img>{lori_millidere_nkmeans4}に示します。
シンデレラの子達は結果が変化しなかったので、ここでは結果を省略します。

//image[charlist_milli][ミリオンライブ幼少組の一覧]

//tabooular[lori_cluster3][幼少組クラスタリングの結果3]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
中谷育	|	12	|	10	
周防桃子	|	11	|	11	
大神環	|	12	|	12	
//}

//image[lori_millidere_nkmeans4][幼少組クラスタリングの可視化結果3]

なんと、育ちゃんが12才クラスタに属しています。
さすが育さんです。
育ちゃんのプロフィールと10才クラスタの面々のプロフィールを比較すると、育さんの発育の良さがわかります。
これは将来に期待ですね。

次にシャイニーカラーズの小宮果穂ちゃんを考えます。
果穂ちゃんは12才にも関わらず、高校生組と考えてもいいくらいのスタイルの持ち主です。
このことから、果穂ちゃんを入れた場合、クラスタリング結果が大きく変わることが予想されます。
その影響がどれくらい出るのか、クラスタリングしてみたいと思います。
クラスタリング結果を@<table>{lori_cluster4}に、可視化結果を@<img>{lori_all_nkmeans4}に示します。

//tabooular[lori_cluster4][幼少組クラスタリングの結果4]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
赤城みりあ	|	11	|	11	
市原仁奈	|	9	|	9	
櫻井桃華	|	11	|	12	
佐々木千枝	|	11	|	11	
橘ありす	|	11	|	12	
遊佐こずえ	|	10	|	11
横山千佳	|	9	|	9	
龍崎薫	|	9	|	9	
佐城雪美	|	10	|	10	
福山舞	|	9	|	10	
メアリー・コクラン	|	11	|	11	
古賀小春	|	11	|	12
城ヶ崎莉嘉	|	11	|	12	
的場梨沙	|	11	|	12	
結城晴	|	11	|	12
中谷育	|	11	|	10	
周防桃子	|	11	|	11	
大神環	|	11	|	12	
小宮果穂	|	12	|	12
//}

//image[lori_all_nkmeans4][幼少組クラスタリングの可視化結果4]

予想通りでした。
果穂ちゃん一人だけが12才クラスタに属してしまい、それによって他の子達が大きくその影響を受けています。
それだけ果穂ちゃんは規格外ということですね。

=== 特徴量の変更とその結果

前節のクラスタリングでは、年齢や身長・体重・3サイズの6つの特徴量を使用しました。
ですが本当に全ての特徴量が必要なのでしょうか。
発育度合いによって年齢別クラスタリングしたい場合、年齢を考慮せずに分類できた方が理想的です。
つまり、身体的特徴のみからいい感じのクラスタリングができるかが問題です。
次に年齢を除いた5次元の特徴量を使ってクラスタリングを行います。

特徴量から年齢を除外しクラスタリングを行います。
クラスタリング結果を@<table>{lori_cluster5}に示します。
使用するデータは、果穂ちゃんを除いたデレとミリの幼少組を対象とします。

//tabooular[lori_cluster5][幼少組クラスタリングの結果5]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
赤城みりあ	|	12	|	11	
市原仁奈	|	9	|	9	
櫻井桃華	|	12	|	12	
佐々木千枝	|	11	|	11	
橘ありす	|	11	|	12	
遊佐こずえ	|	10	|	11
横山千佳	|	9	|	9	
龍崎薫	|	10	|	9	
佐城雪美	|	10	|	10	
福山舞	|	9	|	10	
メアリー・コクラン	|	12	|	11	
古賀小春	|	12	|	12
城ヶ崎莉嘉	|	12	|	12	
的場梨沙	|	12	|	12	
結城晴	|	12	|	12
中谷育	|	12	|	10	
周防桃子	|	11	|	11	
大神環	|	12	|	12	
//}

年齢を除外した場合においても、年齢を考慮した場合と同様のクラスタリング結果になりました。
身長・年齢・3サイズの特徴量だけでも、発育度別にクラスタリングが可能であることがわかりました。

さらに特徴量を減らし、3サイズだけ使ってクラスタリングを行ってみましょう。
クラスタリング結果を@<table>{lori_cluster6}に、可視化結果を@<img>{lori_millidere_nkmeans4_bwh}に示します。

//tabooular[lori_cluster6][幼少組クラスタリングの結果6]{
名前	|	年齢（クラスタ）	&	本来の年齢
==========>|====&====
赤城みりあ	|	12	|	11	
市原仁奈	|	9	|	9	
櫻井桃華	|	12	|	12	
佐々木千枝	|	12	|	11	
橘ありす	|	10	|	12	
遊佐こずえ	|	10	|	11
横山千佳	|	9	|	9	
龍崎薫	|	10	|	9	
佐城雪美	|	10	|	10	
福山舞	|	9	|	10	
メアリー・コクラン	|	11	|	11	
古賀小春	|	12	|	12
城ヶ崎莉嘉	|	12	|	12	
的場梨沙	|	11	|	12	
結城晴	|	12	|	12
中谷育	|	11	|	10	
周防桃子	|	12	|	11	
大神環	|	12	|	12		
//}

//image[lori_millidere_nkmeans4_bwh][幼少組クラスタリングの可視化結果6]

身長と体重を除いたクラスタリングを行うことによって、少し異なる結果が得られました。
例えば、梨沙とメアリーは年齢が1つ下がって11才のクラスタに属しました。
これは、バストが他の12才組と比較して少し控えめであることが1つの主因として考えられます。

さらに育ちゃんが11才のクラスタに属しています。
バストの数値は申し分ないのですが、ウェストやヒップの数値が影響しているかもしれません。
また今回使用したK-means法は、各クラスタの重心に近いデータを同じクラスタとします。
各クラスタの重心にもっとも近いキャラを調べたところ、11才クラスタは梨沙、12才クラスタは桃華でした。
@<img>{lori_millidere_nkmeans4_bwh}より、育ちゃんは桃華より梨沙に近いことがわかります。
そのため、育ちゃんが11才クラスタに属したと考えられます。

=== まとめ

この章ではアイマスキャラの身体的特徴を使って、回帰モデルやクラスタリングといったデータ分析を行ってみました。
回帰モデルを構築した結果として、導出した重回帰式がそれなりの精度でアイマスキャラを説明できる回帰式になりました。
しかし、今回は有意差検定や交差検証などを行っていないので、統計屋さんに怒られ事案かもしれません。
幼少組を対象とした年齢別クラスタリングでは、それなりによく分類できていたのではないでしょうか。
ただしこの結果はアイマス幼少組だけによる分類結果なので、データが増えることによってより詳しく発育度合いがわかると思います。

今回使った分析手法はメジャーかつ割と簡単な手法のため、より複雑な手法を使うとより高精度にモデル構築や分類ができると思います。

== アイマスとケモインフォマティクス

本記事は、2017年10月に発行した「L0 Vol.4」に掲載した「バブみの高そうな香りがする化合物の部分構造解析」の再録となります。
ですが、いくつかの画像については差し替えを行なっています。

=== はじめに

バブm、母性を感じる要素って何でしょうか。
言葉遣い、振る舞い、雰囲気などの様々な要素が考えられると思います。
その様々な要因の中でも、香りにも母性の感じる要因があるのではと考えました。
そこで母性の塊である「ちゃま」こと櫻井桃華を象徴する花である「バラ」の香りに注目したいと思います。
ここで、櫻井桃華を簡単に紹介します。
櫻井桃華は「アイドルマスターシンデレラガールズ」に登場する神戸出身の12歳の女の子です。
いくつかの画像を@<img>{momoka}に示します。

//imagew[momoka][ママの写真]

人間が匂いを感じるメカニズムは、特定の化学物質や化合物が体内の特定の受容体に結合し、電気信号が発生して匂いを感じるという流れです。
化合物の特性として、類似する構造は類似する機能を持つことが知られています。
例えば、毒性、沸点、溶解度など様々です。
化合物の香りも同様であり、類似する構造は類似する香りとなります（例外はあります）。
しかし、化合物の全体構造が似ていなくても、類似する香りや機能であることが多々あります。
それは全体構造ではなく、特定の部分構造が重要である、といったパターンです。
全体での比較が似ていなくても、部分的な要素に注目すると共通の要素があった、という事例はたくさんあると思います。
例えば以下のキャラたち（@<img>{tanedarisa}参照）は「とある共通の特徴」を持っています。
（勘のいい人はすぐにわかると思います。）

//image[tanedarisa][あややが好きです]

化合物だとこんな感じです（@<img>{common-fragment}参照）。
丸で囲った場所が共通の部分構造です（共通の部分構造は他にもあります）。

//image[common-fragment][2つの化合物の共通構造の一例]

部分構造の比較によって、構造類似性の比較や機能の推定を行う手法がケモインフォマティクス（バイオインフォマティクス）
にてよく行われています。
今回はバラの香りに関連する化合物を用いて、部分構造の抽出を行い、彼女の母性の秘密を探ってみようと思います。

=== 化合物の表現方法と部分構造について

化合物とは、２種類以上の元素が化学結合によって結びついた物質です。
例えば、水は水素２つと酸素１つからできている化合物です。
化合物は元素を頂点、化学結合を辺とした無向グラフとして表すことができます。
そして無向グラフは行列式（隣接行列）に変換することができます。
以下に有名な化合物であり、6つの炭素で構成されるベンゼンの構造と隣接行列に変換した例を@<img>{benzen}示します。
ここで、行列式の対角要素には各元素の元素番号を入れています。
元素番号とは元素周期表の順番であり、炭素は元素周期表の６番目に該当するため、6とします。
また化学結合を区別するため、単結合（1本の辺）と二重結合（2本の辺）をそれぞれ1, 2とします。

次に化合物を部分構造に分割することを考えます。
部分構造の定義方法は様々なものが存在します。
例えば、注目している頂点から距離xまでの部分構造や、化学的な特性に基づいた部分構造などがあります。
今回は単純な分割方法である、注目頂点から距離x以内までの部分構造とします。
例として、距離２以内とした場合のベンゼンの部分構造抽出結果を@<img>{benzen-fragment}に示します。
部分構造は文字列で表しています。
例えば、炭素と炭素の単結合の場合は「C-C」、二重結合の場合は「C=C」と表します。
@<img>{benzen-fragment}において、距離2の場合には部分構造「C-C=C」と「C=C-C」が考えられますが、今回は同一のものと見なすことにします。

//image[benzen][ベンゼン構造式と隣接行列][scale=0.7]
//image[benzen-fragment][ベンゼンの部分構造抽出結果]

=== データセット

今回使用するデータはバラの香りがするものの中から、桃華と関係があると考えられるダマスク・スウィート系とティー・フェノリック系に分類される計6つの化合物を使用します。
この化合物を選んだ理由は後ほど説明します。
@<img>{rose-compound}に使用した6つの化合物を示します。
甘いバラの香りがするダマスク・スウィート系は①、ティーローズ特有の香りがするティー・フェノリック系は②でグループ分けしています。

データは主に「日本化学物質辞書（日化辞）」というところから調達しました。
いくつか無いデータもあるかもですが、化合物名でググったら何かしらのページがヒットします。
化合物中の結合情報や元素情報は「molファイル（XXX.mol）」と呼ばれるファイルに記述されています。
今回の実験では、上記のデータから水素を全て除外したものを使用します。

//imagew[rose-compound][今回の使用する化合物一覧]

=== 部分構造の抽出結果

データセット全件に対して距離2以内の部分構造を抽出した結果を@<table>{fragment-anal}に示します。

//tabooularw[fragment-anal][部分構造の一覧表]{
  化合物名                   | C   & O   & C-C & C=C & C-O &C-C-C&C-C=C&O-C-C&C-O-C&C=C-O
  ==========================>|=====&=====&=====&=====&=====&=====&=====&=====&=====&======
  2-フェネチルアルコール     | 8   & 1   & 5   & 3   & 1   & 2   & 7   & 1   & 0   & 0
  ゲラニオール               | 10  & 1   & 7   & 2   & 1   & 4   & 6   & 1   & 0   & 0
  ネロール                   | 10  & 1   & 7   & 2   & 1   & 4   & 6   & 1   & 0   & 0
  シトロネロール             | 10  & 1   & 8   & 1   & 1   & 7   & 3   & 1   & 0   & 0
  1,3-ジメトキシ-5-          | 9 \ & 2 \ & 4 \ & 3 \ & 4 \ & 1 \ & 7 \ & 2 \ & 2 \ & 2 \
  メチルベンゼン             |     &     &     &     &     &     &     &     &     &
  1,3,5-トリメトキシベンゼン | 9   & 3   & 3   & 3   & 6   & 0   & 6   & 3   & 3   & 3
//}

ティー・フェノリック系の化合物にはダマスク・スウィート系には現れない部分構造C-O-C, C=C-Oが現れています。
このことから、この部分構造はティー・フェノリック系特有の香りを出すために重要であると言えます。
また、ダマスク・スウィート系の化合物は、炭素が枝分かれせずに連続している（直鎖構造である）部分構造が重要であることがC-C-Cの数からわかります。
このような単純な部分構造解析でも、ある程度香りの特徴をつかむことが可能なことが判明しました。

=== 今回のデータと櫻井桃華との関連性について

櫻井桃華のソロ曲「ラヴィアンローズ」の歌詞の一番サビには「甘い香りにはしゃぐミツバチみたく踊りましょう」とあります。
つまり、甘い香りのバラがあることがわかります。そこでダマスク・スウィート系のバラであると推測しました。
また、彼女の趣味であるティータイム。そこから連想されるのは紅茶の香り、ティー・フェノリック系と推測しました。
つまり、この2種類の香りの共通点がわかれば、彼女の母性（バブみの高い化合物部分構造）について何かわかるのではと予想しました。

残念ながら、今回の手法では、バブみが高いといえる部分構造は見つけられませんでした。
しかし、彼女の母性を構成する一要素としてC-O-CやC=C-O、炭素の直鎖構造が関連するかもしれないということを見出すことができました。

=== まとめ

今回は櫻井桃華を象徴する花である「バラ」の香りから、彼女の母性の秘密について考察してみました。
ケモインフォマティクスの分野では、いろんな部分構造の定義での解析が進められています。
他の手法で解析することで、より彼女の母性の秘密に迫れるかもしれません。
またデータセットに関しても、化学に疎いのでよくわかりませんでした。
専門の人がデータを集めてくれると、違った結果やもっとバブみの高い部分構造が見えてくるのではないかと期待しています。

== アイマスとバイオインフォマティクス

本記事は、2018年8月に発行した「L0 Vol.5」に掲載した「タンパク質で見るデレマス次世代ママ候補初級編」を加筆修正した記事になります。
具体的には、データの追加とそれに伴う追加実験を行いました。

=== はじめに

論文誌Natureにて「The Rosa genome provides new insights into the domestication of modern roses」
という論文が掲載されました。
バラの遺伝子を解析し、他の遺伝子 （いちご、りんご）などと比較した結果、進化的にバラといちごは「思っていた以上に近い」ことが
わかったそうです@<fn>{rose}。
//footnote[rose][@<href>{https://www.nature.com/articles/s41588-018-0110-3}]

遺伝子単位で近いなら、そこから翻訳されるタンパク質単位ではどのくらい近いのか、と考えました。
そこで今回は、タンパク質のアミノ酸配列データ （以下、タンパク質配列）を用いて、バラといちごがどれくらい類似しているのか調査します。
さらに追加で、いくつかの生物種とバラの類似度を計算して比較を行いたいと思います。
これは学術的に興味があっただけです。
やましい心はありません。
本当です。

上記の理由は建前で、この論文を読んだときにあることを考えました。
バラといえば、我らが母、櫻井桃華です。
いちごといえば、橘こと、橘ありすです。
両名は「アイドルマスターシンデレラガールズ （以下、デレマス）」に登場する、大変素敵で魅力的な女性たちです。
さらにデレマスのカップリングには「ももあり （桃華×ありす）」という素晴らしい組み合わせがあります。
いちごはバラに近い、ありすは桃華に近い、なるほどありすにバブみを感じるのか、なんて妄想を思いついたのがきっかけです。

さらにデレマスには、花や果物から連想できる女性たちが数人います。
ひまわりといえば龍崎薫、ライラックといえば相葉夕美、といった感じです。
例えばひまわりがバラに近ければ、「ももかお （桃華×薫）」という意外な可能性や、薫は母になってくれる女性である可能性が微レ存、
といった妄想も膨らみます。

=== 類似度の決め方

物と物がどれくらい類似しているかを定量的に計算・解析することは、非常に重要ですが難しい問題です。
@<img>{alstroemeria}は「アイドルマスターシャイニーカラーズ」に登場する双子アイドル「大崎甜花、大崎甘奈」です。
双子なので容姿などが似ていることが見て取れますが、どれくらい類似しているかを定量的に表現するのは難しいです。
類似度を計算しようと思うと、様々な特徴をなんとか数値化して比較する方法が考えられます。
例えば色合いや全体の輪郭、喋る言葉や単語の傾向など様々あります。

//image[alstroemeria][甜花ちゃんが好きです][scale=0.95]

タンパク質の場合、同じ生物学的機能や立体構造を持っているかなどを予測するための重要な指標になります。
タンパク質配列同士の類似度を計算する方法として、生物学的な意味も考慮した配列アライメントによるアライメントスコアを求めるのが基本です。
ですが、今回は文書の類似度計算でよく用いられているn-gramを使って類似度を調査したいと思います。
n-gramは文字列や文書をn個の文字 （または単語）で分割する方法です。
例えば「櫻井桃華」を「2文字 (n=2)かつ1文字ずつスライド」で分割すると「櫻井、井桃、桃華」となります。
非常にシンプルな方法のため、様々な場面で応用することができます。
今回はn-gramをタンパク質配列の類似度に応用してみようと思います。
ある生物種のタンパク質配列すべてをn-gram （アミノ酸残基n文字単位）で分割し、その分割文字列群を各集合・ベクトルとします。
例えば「ARISU」というタンパク質配列があり「2文字 (n=2)かつ1文字ずつスライド」の場合、「AR、RI、IS、SU」となります。
ベクトルの大きさはその分割文字列の出現回数としました。

2つの集合・ベクトルを比較して類似度を求める指標は数多くあります。
今回、集合の類似度の指標としてJaccard係数、Dice係数、Simpson係数、Tanimoto係数を使用します。
各係数の導出方法は以下の通りです。
各式の@<m>{A,B}は任意の集合であり、@<m>{|A|,|B|}は各集合@<m>{A,B}の要素数を表します。

//texequation{
Jaccard(A,B) = \frac{|A \cap B|}{|A \cup B|}
//}

//texequation{
Dice(A,B) = \frac{2|A \cap B|}{|A| \cup |B|}
//}

//texequation{
Simpson(A,B) = \frac{|A \cap B|}{\mathrm{min}\{|A|,|B|\}}
//}

//texequation{
Tanimoto(A,B) = \frac{|A \cap B|}{|A|+|B|-|A \cap B|}
//}

ベクトルの類似度の指標としてはcos （コサイン）類似度を使用します。
cos類似度の計算式は以下の通りです。
@<m>{|V|}はベクトルの次元数を表します。

//texequation{
cos(\vec{A},\vec{B}) = \frac{\sum_{i=1}^{|V|}A_iB_i}{\sqrt{\sum_{i=1}^{|V|}A_i^2}\sqrt{\sum_{i=1}^{|V|}B_i^2}}
//}

=== データセット

バラやいちごなどのタンパク質配列を集めます。
今回はアミノ酸配列データベースUniProt@<fn>{uniprot}から、学名で一致するデータをすべてダウンロードします。
//footnote[uniprot][@<href>{https://www.uniprot.org/}]
バラの場合「Rosa」で検索してヒットした配列をすべて対象とします。
登録されているバラはさらに細分化されて分類されているのですが、よくわからないので全て対象にしてしまいます。
今回集めた生物種と学名、データ数を@<table>{dataset}に示します。
さらにその花または果物から連想できるデレマスのアイドル名とその理由を示します。
加えて@<table>{dataset}に載っているアイドルのイラストを@<img>{charalist}に示します。

//tabooularw[dataset][使用した生物種データ一覧]{
生物種名	|	学名	&	データ数	&	アイドル名 （理由）
==========>|====&====&====
バラ	|	Rosa	|	46,862	|	櫻井桃華 （ソロ曲、イラストより）
いちご	|	Fragaria Ananassa	|	769	|	橘ありす (好物より)
ひまわり	|	Helianthus Annuus	|	54,266	|	龍崎薫 （イラストより）
りんご	|	Malus Domestica	|	2,472	|	十時愛梨 （ソロ曲より）
クローバー	|	Trifolium Repens	|	372	|	緒方智絵里 （趣味より）
さくら	|	Prunus Yedoensis	|	142	|	小早川紗枝 （イラストより）
ライラック	|	Syringa Vulgaris	|	151	|	相葉夕美 （ソロ曲より）
ゆず	|	Citrus Junos	|	19	|	喜多見柚 （名前より）
//}

//imagew[charalist][アイドルのイラスト一覧]

=== 誰がもっとも桃華に近いのか

n-gram (n=2,3,4,5)によって生成された各生物種の集合を対象として、各指標でバラとの類似度を計算しました。
類似度が1に近い順で順位付けした結果を@<table>{set-result}に示します。
なお、学名・生物種名は長いので以後はアイドルの名前で表記します。
名前の下には類似度を示します。
類似度は小数点第3位で四捨五入しました。

//tabooularw[set-result][集合の類似度]{
  順位 (n=2)	|	1	&	2	&	3	&	4	&	5	&	6	&	7
  ==========>|====&====&====&====&====&====&====
 Jaccard \ |	ありす	&	愛梨	&	智絵里	&	薫	&	紗枝	&	夕美	&	柚
      |	0.98	&	0.97	&	0.96	&	0.95	&	0.94	&	0.92	&	0.91
 Dice \ |	ありす	&	愛梨	&	智絵里	&	薫	&	紗枝	&	夕美	&	柚
      |	0.99	&	0.99	&	0.98	&	0.98	&	0.97	&	0.96	&	0.95
 Simpson \ |	柚	&	薫	&	愛梨	&	紗枝	&	夕美	&	ありす	&	智絵里
      |	1.0	&	1.0	&	1.0	&	0.99	&	0.99	&	0.99	&	0.99
 Tanimoto \ |	ありす	&	愛梨	&	智絵里	&	薫	&	紗枝	&	夕美	&	柚
      |	0.98	&	0.97	&	0.96	&	0.95	&	0.94	&	0.92	&	0.91
  --------------------------------------------------------------------
  > 順位 (n=3) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
Jaccard \ |	ありす	&	愛梨	&	智絵里	&	薫	&	夕美	&	紗枝	&	柚
     |	0.96	&	0.95	&	0.94	&	0.92	&	0.84	&	0.79	&	0.43
Dice \ |	ありす	&	愛梨	&	智絵里	&	薫	&	夕美	&	紗枝	&	柚
     |	0.98	&	0.97	&	0.97	&	0.96	&	0.91	&	0.88	&	0.60
Simpson \ |	柚	&	夕美	&	紗枝	&	薫	&	愛梨	&	智絵里	&	ありす
     |	1.0	&	1.0	&	1.0	&	1.0	&	0.99	&	0.99	&	0.98
Tanimoto \ |	ありす	&	愛梨	&	智絵里	&	薫	&	夕美	&	紗枝	&	柚
     |	0.96	&	0.95	&	0.94	&	0.92	&	0.84	&	0.79	&	0.43
  --------------------------------------------------------------------
  > 順位 (n=4) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
 Jaccard \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.99	&	0.75	&	0.49	&	0.31	&	0.17	&	0.13	&	0.03
 Dice \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.99	&	0.86	&	0.66	&	0.47	&	0.28	&	0.23	&	0.07
 Simpson \ |	柚	&	夕美	&	紗枝	&	薫	&	智絵里	&	ありす	&	愛梨
      |	1.0	&	1.0	&	1.0	&	1.0	&	1.0	&	1.0	&	0.99
 Tanimoto \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.99	&	0.75	&	0.49	&	0.31	&	0.16	&	0.13	&	0.03
  --------------------------------------------------------------------
  > 順位 (n=5) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
 Jaccard \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.76	&	0.15	&	0.06	&	0.03	&	0.01	&	0.01	&	0.00
 Dice \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.86	&	0.26	&	0.11	&	0.05	&	0.03	&	0.02	&	0.00
 Simpson \ |	紗枝	&	ありす	&	夕美	&	柚	&	智絵里	&	愛梨	&	薫
      |	0.98	&	0.98	&	0.98	&	0.97	&	0.96	&	0.96	&	0.88
 Tanimoto \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
      |	0.76	&	0.15	&	0.06	&	0.03	&	0.01	&	0.01	&	0.00
//}

n=3までのJaccard,Dice,Tanimotoにおいては、ありすが最も桃華に近い結果になりました。
全体の順位もほとんど変化がありません。
しかし、各類似度の4位くらいまでは類似度が90%以上と非常に高いため、n=3まではどの生物種も類似しているといえます。
n=4からは順位や類似度も変化しました。
なんと薫が最も桃華に似ているという結果になりました。
桃華と薫の類似度はn=5においてもJaccardとTanimotoで76%、Diceで86%と高いです。
アップルパイプリンセスこと愛梨はnが変化しても高い順位 (2位)を保っています。
ですがn=5の場合の類似度においては、JaccardとTanimotoで14%、Diceで25%となり、かなり低くなっています。
Simpsonにおいては、順位が他の類似度と比べて大きく異なっています。
類似度を確認すると、n=5の最下位においても類似度が88%と高いので、あまり順位に意味はないかもしれません。

次に、n-gram (n=2,3,4,5)によるコサイン類似度を計算しました。
以下の@<table>{vector-result}に示します。
集合の時と同様、名前の下には小数点第3位で四捨五入した類似度を示します。

//tabooular[vector-result][ベクトルの類似度]{
  順位 (n=2)	|	1	&	2	&	3	&	4	&	5	&	6	&	7
  ==========>|====&====&====&====&====&====&====
  cos \ |	薫	&	愛梨	&	ありす	&	智絵里	&	柚	&	夕美	&	紗枝
       |	1.0	&	1.0	&	0.99	&	0.98	&	0.96	&	0.96	&	0.95
  --------------------------------------------------------------------
  > 順位 (n=3) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
  cos \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
     |	0.99	&	0.96	&	0.96	&	0.90	&	0.87	&	0.79	&	0.72
  --------------------------------------------------------------------
  > 順位 (n=4) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
  cos \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
     |	0.97	&	0.72	&	0.72	&	0.51	&	0.47	&	0.34	&	0.25
  --------------------------------------------------------------------
  > 順位 (n=5) <|	1	&	2	&	3	&	4	&	5	&	6	&	7
  --------------------------------------------------------------------
  cos \ |	薫	&	愛梨	&	ありす	&	智絵里	&	夕美	&	紗枝	&	柚
       |	0.80	&	0.31	&	0.30	&	0.17	&	0.16	&	0.13	&	0.09
//}

cosにおいては、nに関係なく順位に大きな変化はありませんでした。
常に類似度1位の薫に注目すると、nが大きくなっても類似度は80%と高い数値でした。
そのため薫と桃華は、類似した集合かつベクトルを持っていることがわかります。
次いで高い愛梨やありすに注目してみます。
順位は高いですが、類似度はn=5の場合で両者ともに30%程度と、薫と大きく差が開きました。
しかし、集合の場合と比較すると15~20%程度の類似度の向上が見られました。
これは集合の要素だけではなく、出現回数を考慮しているためと考えられます。

各類似度を調べた結果、ありすより薫のほうが桃華に近かったとわかりました。
花同士なので類似しているのは直感的だと思います。
桃華に近いということは、立派なママになる可能性や素質が非常に高いと考えられます。
愛梨ママはなんとなくわかりますが、薫ママは新たな可能性ですね。
当初の目的であるありすと桃華の類似度も、それなりの結果でした。
智絵里 （クローバー）や紗枝 （さくら）よりも桃華 （バラ）に近いのは意外な結果でした。
やはり「思っている以上にいちごはバラに近い」という感じでしょうか。

=== 他シリーズキャラとの比較

これまでは、シンデレラガールズのキャラを対象としました。
しかし、花とゆかりが深いキャラはミリオンライブやシャイニーカラーズにも存在します。
次に、それらのキャラとの比較を行う追加実験をしたいと思います。
もしかしたら薫を超えるママ候補が出現するかもしれません。
@<table>{dataset2}にアイドルから連想できる生物種を示し、そのアイドルのイラストを@<img>{charalist2}に示します。
アルストロメリアはユニットなので、今回は代表してリーダーの甘奈ちゃんとします。
集合の類似度を計算した結果を@<table>{set-result2}に示し、ベクトルの類似度を計算した結果を@<table>{vector-result2}に示します。

//tabooularw[dataset2][追加した生物種データ一覧]{
生物種名	|	学名	&	データ数	&	アイドル名 （理由）
==========>|====&====&====
ゆり	|	Lilium	|	4,616	|	七尾百合子 （名前より）
あやめ（菖蒲）	|	Iris sanguinea	|	101	|	白石紬 (ソロ曲より)
アルストロメリア	|	Alstroemeria	|	467	|	大崎甘奈 （ユニット名より）
//}

//image[charalist2][追加したアイドルのイラスト一覧]

//tabooularw[set-result2][集合の類似度2]{
  順位 (n=2)	|	1	&	2	&	3
  ==========>|====&====&====
  Jaccard \ |	百合子	&	甘奈	&	紬
      |	0.97	&	0.97	&	0.96
  Dice \ |	百合子	&	甘奈	&	紬	
      |	0.99	&	0.98	&	0.98
  Simpson \ |	百合子	&	紬	&	甘奈
      |	0.99	&	0.98	&	0.98
  Tanimoto \ |	百合子	&	甘奈	&	紬
      |	0.97	&	0.97	&	0.96
  --------------------------------------------------------------------
  > 順位 (n=3) <|	1	&	2	&	3
  --------------------------------------------------------------------
  Jaccard \ |	百合子	&	甘奈	&	紬
      |	0.96	&	0.84	&	0.78
  Dice \ |	百合子	&	甘奈	&	紬	
      |	0.98	&	0.91	&	0.87
  Simpson \ |	百合子	&	紬	&	甘奈
      |	0.99	&	0.99	&	0.99
  Tanimoto \ |	百合子	&	甘奈	&	紬
      |	0.96	&	0.84	&	0.78
  --------------------------------------------------------------------
  > 順位 (n=4) <|	1	&	2	&	3
  --------------------------------------------------------------------
  Jaccard \ |	百合子	&	甘奈	&	紬
      |	0.59	&	0.16	&	0.12
  Dice \ |	百合子	&	甘奈	&	紬	
      |	0.74	&	0.28	&	0.22
  Simpson \ |	紬	&	百合子	&	甘奈
      |	0.99	&	0.99	&	0.99
  Tanimoto \ |	百合子	&	甘奈	&	紬
      |	0.59	&	0.16	&	0.12
  --------------------------------------------------------------------
  > 順位 (n=5) <|	1	&	2	&	3
  --------------------------------------------------------------------
  Jaccard \ |	百合子	&	甘奈	&	紬
      |	0.08	&	0.01	&	0.01
  Dice \ |	百合子	&	甘奈	&	紬	
      |	0.15	&	0.02	&	0.02
  Simpson \ |	紬	&	甘奈	&	百合子
      |	0.97	&	0.96	&	0.95
  Tanimoto \ |	百合子	&	甘奈	&	紬
      |	0.08	&	0.01	&	0.01
//}

//tabooular[vector-result2][ベクトルの類似度2]{
  順位 (n=2)	|	1	&	2	&	3	
  ==========>|====&====&====
  cos \ |	百合子	&	紬	&	甘奈
       |	0.97	&	0.94	&	0.93
  --------------------------------------------------------------------
  > 順位 (n=3) <|	1	&	2	&	3
  --------------------------------------------------------------------
  cos \ |	百合子	&	紬	&	甘奈
       |	0.87	&	0.81	&	0.63
  --------------------------------------------------------------------
  > 順位 (n=4) <|	1	&	2	&	3
  --------------------------------------------------------------------
  cos \ |	百合子	&	紬	&	甘奈
       |	0.46	&	0.37	&	0.21
  --------------------------------------------------------------------
  > 順位 (n=5) <|	1	&	2	&	3
  --------------------------------------------------------------------
  cos \ |	百合子	&	紬	&	甘奈
       |	0.16	&	0.13	&	0.08
//}

新たに追加した3名の中では、いずれの類似度も百合子がもっとも類似度が高い結果となりました。
ですが、類似度の数値はそれほど高い数値ではありません。
n=5の場合のJaccardやTanimotoに注目すると、百合子は智絵里以上ありす以下の類似度です。
次にn=5の場合のcosineに注目します。
集合の類似度の場合と大きな順位の変化はなく、おおよそ同等の結果（順位）となりました。

結果として、追加キャラたちはそこまで桃華に類似していませんでした。
たしかに百合子、紬、甘奈に母性を求めるのは、まだ少々厳しいでしょう。
今後に期待です。
個人的には、甘奈ちゃんはいいママになると考えています。

=== おわりに

今回は数種類の花や果物のタンパク質配列を使用して、バラとの類似度を計算してみました。
結果としてバラとひまわり （桃華と薫）は、n=5までのn-gramにおいては類似した集合・ベクトルを持つことがわかりました。
驚くべきことに、百合やライラックといった花よりも、いちごがバラに近いことが示唆されました。
つまり論文の趣旨である「思ったよりいちごがバラに近い」という考えが間違いではない、と言えます。
今回タンパク質配列の観点からもそのことが示唆されたのは、面白い結果だと思います。

一方、今回選んだ生物種はまだまだタンパク質配列が決定されていない生物種が多く、データ数のばらつきが大きいです。
今後データが充実していくと、より詳細なことがわかるかもしれません。
データ数の問題は、ウェットな実験屋さんが頑張ってくれるのを待つしかありません。
さらに今回はタンパク質のアミノ酸残基の化学的な意味を考慮していないので、それを考慮した解析を行うと違う結果になると考えられます。

私はみりあPなので、みりあを花に例えるなら何になるかを考えました。
例えば、フリージアやルピナス （白）あたりがよいのではないかと思います。
それぞれの花言葉は「天真爛漫」や「母性愛」などが挙げられます。
普段は子供っぽくあり、時にはお姉ちゃんのような優しさを見せる。
赤城みりあはそんな素敵な少女です。

== まとめ

今回はアイマスマスターのキャラの身体的特徴を使って、簡単なデータ分析を行いました。
また、私の研究分野であるバイオ・ケモインフォマティクスとアイドルマスターを多少強引に結びつけた記事を書きました。
バイオ・ケモインフォマティクスは、まだ知らない人が多い分野だと思うので、なかなかスパイスの効いた記事になったのではと思います。

今回までの記事に使用した手法は、比較的オーソドックスな手法が使われています。
今後の記事はもう少し複雑な手法を使った実験が考えられます。
流行りの「しんそーがくしゅー」なんかいいんじゃないでしょうか。
アイドルマスターのキャラは増え続けていますが、幼少組（今回は12才以下）のキャラはなかなか増えません。
これは何故なのでしょうか。
みなさん小さい子はお好きではないのでしょうか。
このような状況もあるため、データ分析の際はデータの比率や分布なども、いままで以上に気をつけていかないといけません。
また、引き続きバイオ・ケモインフォとアイドルマスターを（無理やり）結びつけた記事を書いていきたいと思っています。