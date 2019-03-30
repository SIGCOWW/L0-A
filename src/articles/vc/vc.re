= 一般男性音声コーパス(OJaMa)を用いた深層学習に基づく統計的声質変換入門
@<author>{4月からD2の Sython;twitter:SythonUK}
//profile{
有名人と結婚できる一般男性になりたい人生でした．
//}

//lead{
ネコでもわかる！統計的声質変換
//}

== はじめに

Sythonです．某大学大学院の博士後期課程2年目に突入しました@<fn>{syukatu}．
大学で音声合成・声質変換技術を研究しているスキマ時間を用いて，これまでに音声分析や声質変換 (Voice Conversion: VC) に関する記事を執筆してきました．
本稿では，筆者がこれまでに執筆した次の記事をまとめます．
//footnote[syukatu][D2ですが，博士後期課程修了後を見据えて現在就活中です（無事に修了できるとは言っていない）．]

 * 深層学習とささやきフィルタによる音声変換 (L0: Vol.3)
 * バ美肉おじさんになるための裏声を用いた統計的声質変換技術の基礎的検討 (L0: Vol.5)
 * 風邪声判定および風邪声声質変換のための音声分析・基礎的検討 (L0: Vol.6)

== 深層学習に基づく統計的声質変換
声質変換 (VC) とは，入力された音声の言語情報（何を話しているか）を保持しながら，それ以外の情報（例えば，誰が話しているか，どんな風に話しているか，等）を変換する技術です（@<img>{vc}）．
特に，統計モデルに基づくVCモデルを学習させるものを「統計的VC」と呼びます．
本節では，深層学習に基づく統計的VCの枠組みを簡単に説明します@<fn>{vc}．
//footnote[vc][Vol.3とVol.5の内容に基づいたものを一部修正・加筆しています．]
//image[vc][声質変換 (VC) の概念図][scale=0.5]

=== 学習データ（音声コーパス）の用意
VCモデルを構築するために，変換元・変換先話者の音声を収録した学習データ（@<b>{音声コーパス}@<fn>{corpus}）を用意します．
音声コーパスを分類する主要因を次に示します．
//footnote[corpus][最低限，音声とテキストの対応が取れているものだと思っていただければよいです（参考: @<href>{http://www.asj.gr.jp/journal/pdf/7210onkochishin.pdf}）．]

==== パラレル or ノンパラレル
変換元・変換先の話者対で，同じ発話内容の音声を収録した音声コーパスを@<b>{パラレル音声コーパス}，そうでないものを@<b>{ノンパラレル音声コーパス}と呼びます．
パラレル音声コーパスを作るのは大変ですが，高品質なVCモデルを構築できます．
一方で，ノンパラレル音声コーパスは比較的作りやすいですが，構築されるVCモデルの品質は，パラレル音声コーパスを用いた場合と比較すると劣化しがちです．

==== 単一話者対 or 多数話者対
統計的VCのための最もベーシックな音声コーパスは，変換元・変換先話者がそれぞれ1人ずつである@<b>{単一話者対音声コーパス}です．
より多数の話者を用意した音声コーパスは@<b>{多数話者対音声コーパス}と呼ばれます．
当然ですが，多数話者対音声コーパスを用いることで，より多様な話者対でのVCを実現できます@<fn>{one_or_many}．
//footnote[one_or_many][本稿では，説明の都合上このように分類しましたが，実際は「話者対の数」を基準にして議論するのではなく，単純に「話者の数」で議論されることが多いと思います．]

音声コーパスの種類に基づく統計的VCの分類を@<img>{para_or_nonpara}及び@<img>{one_or_many}に示します．
筆者の所感ですが，「パラレル」は「一対一」のVCと，「ノンパラレル」はそれ以外のVCとそれぞれ対応づいている印象があります．
また，多数話者対を用いたVCでは，「多」に対応する話者が「学習データに含まれない未知のもの」を含まれているケースがほとんどです．
本稿では，最もシンプルな@<b>{「パラレル音声コーパスを用いた一対一VC」}について述べます．
//subfigw[音声コーパスの種類に基づく統計的VCの分類]{
//image[para_or_nonpara][パラレル or ノンパラレル][scale=0.45]
//image[one_or_many][単一話者対 or 多数話者対][scale=0.4]
//}

=== 音声特徴量の分析
人間の音声は、声帯振動によって生成された音源が、口、鼻、喉といった声道のフィルタを通ることによって作り出されています（@<img>{speech_process}）。
本稿では，音声特徴量の分析に，森勢先生によって開発されたWORLD@<fn>{world}（のPythonラッパーであるpyworld@<fn>{pyworld}）を利用し、
声帯振動の特性を基本周波数(F0)と非周期性指標(AP: APeriodicity)、
声道フィルタの特性をスペクトル包絡の特徴量（MCEP: Mel-CEPstral coefficients）として抽出します@<fn>{features}．
WORLDスペクトル包絡からMCEPへの変換には、音声信号処理ツールキットSPTK@<fn>{sptk}（のPythonラッパーであるpysptk@<fn>{pysptk}）を利用します．
@<list>{analysis}に音声特徴量抽出のPythonコードを示します。
//image[speech_process][音声の生成過程：ソース・フィルタモデル（東京大学 信号処理特論第7回講義資料より引用）][scale=0.8]
//list[analysis][pyworldとpysptkを用いた音声特徴量抽出]{
from scipy.io import wavfile
import pyworld
import pysptk
import numpy as np

# load wav file (fs: sampling frequency)
fs, wav = wavfile.read('gomi.wav')
wav = wav.astype(np.float64)

# analysis parameters
#  alpha: warping coefficient
#  mcep_order: order of mel-cepstral coefficients
alpha = pysptk.util.mcepalpha(fs)
mcep_order = 39

# speech analysis
f0, sp, ap = pyworld.wav2world(wav, fs)
mcep = pysptk.sp2mc(sp, order=mcep_order, alpha=alpha)
//}
//footnote[world][@<href>{http://www.kki.yamanashi.ac.jp/~mmorise/world/index.html}]
//footnote[pyworld][@<href>{https://github.com/JeremyCCHsu/Python-Wrapper-for-World-Vocoder}]
//footnote[features][ざっくり説明すると，F0は声の高さ，APは声のかすれ具合を表します．MCEPは，スペクトル包絡を低次元圧縮した特徴量です．]
//footnote[sptk][@<href>{http://sp-tk.sourceforge.net/}]
//footnote[pysptk][@<href>{https://github.com/r9y9/pysptk}]

=== VCモデルの学習
前節の音声特徴量分析により得られた音声特徴量のドメインで，入力音声の話者性を変換する統計モデル（VCモデル）を学習します。
本稿では、話者性の知覚において特に重要なMCEPの変換を考えます@<fn>{source_features}。
//footnote[source_features][F0は変換元・変換先話者の平均と分散を用いて線形変換されます。APは、そもそも変換されないことが多いです。]

一般に，パラレル音声コーパスを用いたとしても，変換話者対のMCEPの系列長は異なります．
そこで，VCモデル学習の前に，変換話者対のMCEPの系列長をDynamic Time Warping (DTW)によって揃えます（@<img>{dtw}）．
以降，DTWによって系列長が揃えられた変換元話者のMCEPを@<m>{\bm{x\}}，
変換先話者のMCEPを@<m>{\bm{y\}}と表記します．
また，DTW後の系列長を@<m>{T}とします．
//image[dtw][DTWの概念図．変換元・変換先音声の音素境界は既知とします．][scale=0.5]

本稿では，VCモデルとしてDeep Neural Network (DNN)を用います．
VCモデル学習の目的は，@<m>{\bm{x\}}から@<m>{\bm{y\}}を予測するDNNのモデルパラメータ（結合重みとバイアス）を推定することです．
これは変換先MCEP @<m>{\bm{y\}}と生成MCEP @<m>{\bm{\hat y\}}から計算される損失関数を最小化することに対応します．
最も単純な損失関数は，@<m>{\bm{y\}}と@<m>{\bm{\hat y\}}の間の二乗誤差(MSE: Mean Squared Error)です．
//texequation{
L_{\rm MSE}(\bm{y}, \bm{\hat y}) = \frac{1}{T}(\bm{\hat y} - \bm{y})^\top (\bm{\hat y} - \bm{y})
//}
MSE最小化基準によるVCモデル学習の概念図を@<img>{dnnvc}に示します@<fn>{frame}．
//image[dnnvc][MSE最小化基準によるVCモデル学習][scale=0.5]
//footnote[frame][図中の@<m>{t}はフレームインデックスを表します．同じDNNをズラーッと並べて時系列のベクトルをまとめて生成しているイメージです．]

=== 変換音声波形の生成
実際に入力音声を変換するための波形生成方式として，本稿では、
音声特徴量変換とWORLDボコーダの利用@<fn>{fcvc}
と
差分スペクトル法の利用@<fn>{sdvc}
の2つを紹介します。
//footnote[fcvc][@<href>{https://ieeexplore.ieee.org/document/4317579/}]
//footnote[sdvc][@<href>{https://www.isca-speech.org/archive/interspeech_2014/i14_2514.html}]

==== WORLDボコーダによる波形生成
実際に変換された音声特徴量を用いて、WORLDボコーダにより音声波形を生成します（@<img>{fcvc}）@<fn>{saito17sp}。
声道の特徴量(MCEP)だけでなく音源の特徴量（F0やAP）も変換できますが、
ボコーダによる分析・合成時に音声の品質が劣化します。
//image[fcvc][特徴量変換とWORLDボコーダを用いた波形生成][scale=0.9]
//footnote[saito17sp][@<href>{http://sython.org/papers/SP/saito201701sp.pdf} から引用]

==== 差分スペクトル法による音声波形変換
変換元の音声波形に対し、変換先と変換元の話者対での差分スペクトルフィルタを畳みこむことで音声を変換します（@<img>{sdvc}）@<fn>{saito17sp}。
波形生成時のボコーダ処理に起因する品質劣化を回避できますが、
音源の特徴量は変換できません。
//image[sdvc][差分スペクトルフィルタによる音声波形変換][scale=0.9]

ボコーダ処理による品質劣化の影響は大きく、音源特徴量の変換を必要としない場合（例えば、同性間での声質変換）では、差分スペクトル法で高品質な声質変換を実現できます。

=== 本節のまとめ
まとめると，統計的VCは，
1. 音声コーパスの用意，
2. 音声特徴量の分析，
3. VCモデルの学習，
4. 変換音声波形の生成
の4つで構成されます．
@<img>{dnnvc_overview}にDNNに基づく統計的VCの流れ図を示します．
//image[dnnvc_overview][DNNに基づく統計的VCの流れ図][scale=0.9]

ついでに，最近のVC研究のトピックを簡単に紹介します．

==== 変換音声特徴量の過剰な平滑化への対策
MSE最小化基準に基づくVCモデル学習は，変換音声特徴量@<m>{\bm{\hat y\}}が従う確率分布として等分散のガウス分布を仮定した，DNNのモデルパラメータの最尤推定として解釈できます．
このモデル化では，ランダム性の高いMCEPの高次成分の分布が縮小（過剰な平滑化）し，生成される音声の品質が著しく劣化するという問題がありました（@<img>{MCEPs_plot}の``MGE''）@<fn>{saito18ganspss}．
後処理的に音声特徴量の分散を無理やり広げることで音質は改善します@<fn>{toda07mlvc}が，それでも人間の音声と同程度の品質は達成できていませんでした．
近年提案された敵対的VCは，過剰な平滑化への対策として，生成MCEPの分布を変換先MCEPの分布に近づける制約を加えてVCモデルを学習させる手法です．
本稿の2.5節における``敵対的学習に基づくVCモデル学習(敵対的VC)の導入''で，敵対的VCの枠組みを説明します@<fn>{gans}．
//image[MCEPs_plot][VCモデルから生成されたMCEPの例][scale=0.75]
//footnote[saito18ganspss][@<href>{https://ieeexplore.ieee.org/document/8063435/}より引用（厳密に言うと2列目の``MGE''はMSE最小化と異なりますが，ほとんど同じです）]
//footnote[toda07mlvc][@<href>{https://drive.google.com/file/d/0B8UaDFgTTWodYUhDU28xR2cwdzA/view}]
//footnote[gans][敵対的学習に基づくVCは，近年盛んに研究されている印象です（例：ノンパラレル多対多VC @<href>{http://www.kecl.ntt.co.jp/people/kameoka.hirokazu/Demos/stargan-vc/}）]

==== WaveNetの導入
従来の統計的VCでは，変換音声波形の生成時に音質が劣化します@<fn>{rekka}．
近年は，音声特徴量から波形を生成する部分（@<img>{fcvc}と@<img>{sdvc}の``Synthesis filtering''）をDNNで置き換える研究が盛んです．
特に，WaveNet@<fn>{WN}に基づくDNNボコーダであるWaveNet Vocoder@<fn>{WNVoco}をVCモデルとして用いた研究が流行っている気がします@<fn>{WNVocoVC}．
さらに，音声特徴量の分析を行う部分もWaveNetで置き換える研究もあります@<fn>{vqvae}．
//footnote[rekka][WORLDボコーダでは，機械音っぽいbuzzyな音になります．差分スペクトル法では，たまにキンキンする音がなったりします．]
//footnote[WN][@<href>{https://arxiv.org/abs/1609.03499}]
//footnote[WNVoco][@<href>{https://researchmap.jp/?action=cv_download_main&upload_id=128513}]
//footnote[WNVocoVC][@<href>{https://pdfs.semanticscholar.org/ac7a/d4a80e5b0aac54ede084f93d3c3252d1b988.pdf}]
//footnote[vqvae][@<href>{https://avdnoord.github.io/homepage/vqvae/}]

==== Sequence-to-Sequence学習の導入
従来の統計的VCでは，VCモデル学習前に変換元・変換先話者のMCEPの系列長をDTWで揃える必要があります．
このDTW処理ですが，たまにコケて明らかに対応が取れていないMCEPの対ができてしまうことがあります．
また，そもそも論として，実際にVCをする際には変換元話者の話す速さが変換できないという問題もあります．
近年では，Sequence-to-Sequence学習@<fn>{seq2seq}をVCモデル学習に導入し，変換話者対の系列長を揃えるところまで学習させる研究が盛んです@<fn>{seq2seqVC}．
//footnote[seq2seq][@<href>{https://papers.nips.cc/paper/5346-sequence-to-sequence-learning-with-neural-networks}]
//footnote[seq2seqVC][例: @<href>{http://www.kecl.ntt.co.jp/people/kameoka.hirokazu/Demos/convs2s-vc/index.html} や @<href>{https://arxiv.org/abs/1810.06865}]

== 統計的VCの実例：ささやき声へのVC (L0 Vol.3)
深層学習を用いた統計的VCの一例として，
一般男性の声をかわいい女性のささやき声に変換するVCをつくってみました．

健全な男性諸君であれば，「かわいい声の女性に耳元でささやかれたい」と思うのは当然だと思います．
しかし，彼女のいない㋔㋟㋗各位には，そのような体験をすることは難しいかもしれません．
本節では，近年提案されたDNNアーキテクチャの一種であるinput-to-output highway networkと，音声をささやき声に変換する「ささやきフィルタ」を用いた，ささやき声へのVCを提案します．

=== 音声コーパス
変換元話者を大学院生のSさん（男性）@<fn>{Ssan}，変換先話者を有名女性声優のHさんとします．
今回は，著者らが独自に構築したHさんの音声データ241文@<fn>{speechdata}に基づき，SさんとHさんのパラレル音声コーパスを作成しました．
Sさんには非常に酷なのですが，無響室に3時間ほど篭っていただき，音声をひたすら収録していただきました．
お疲れ様です．
//footnote[Ssan][Sさんはこの後も出てきます．]
//footnote[speechdata][朗読CDから抽出した42文と，とあるゲームの音声を録音した199文を用いています．]

=== Input-to-output highway networkに基づくVCモデル
ここでは，
近年提案されたinput-to-output highway network@<fn>{highway}を利用し，1次から59次のMCEP@<fn>{mcep}をフレーム毎に変換することを考えます．
@<img>{hwnet}にinput-to-output highway networkのアーキテクチャを示します．
//footnote[mcep][入力音声の音量に関連する0次のMCEPは，変換元話者のものをコピーします．]
//footnote[highway][@<href>{https://www.jstage.jst.go.jp/article/transinf/E100.D/8/E100.D_2017EDL8034/_pdf}]
//image[hwnet][Input-to-output highway networks][scale=0.9]

変換後のMCEP @<m>{\bm{\hat y\}}は，次式で推定されます．
//texequation{
\bm{\hat y} = \bm{x} + \bm{T} (\bm{x}) \circ \bm{G} (\bm{x})
//}
ここで，@<m>{\circ}は要素ごとの積を意味します．
@<m>{\bm{G\}(\bm{\cdot\})}は差分スペクトル特徴量推定器@<fn>{g}であり，
入出力特徴量の差分@<m>{\bm{y\} - \bm{x\}}を推定する役割を持っています．
@<m>{\bm{T\}(\bm{\cdot)\}}はhighway networkのtransform gate@<fn>{tgate}であり，
入力特徴量@<m>{\bm{x\}}に応じて@<m>{\bm{G\}(\bm{\cdot\})}の推定結果に重み付けをする役割を持っています．
ざっくり説明すると，違いが大きい部分はちゃんと変換して，違いが小さい部分は入力された特徴量をそのまま出力すればいい，という考え方です．
@<m>{\bm{G\}(\bm{\cdot\})}と@<m>{\bm{T\}(\bm{\cdot\})}は，@<m>{L_{\rm MSE}(\bm{y}, \bm{\hat y})}を最小化するように学習されます．
予備実験により，単純なFeed-Forwardネットワークを用いた場合と比較して変換音声の品質が改善することを確認しました．
論文についての詳しい内容は著者のSaitoさんに聞いてください．
//footnote[g][3層のFeed-Forwardネットワークを用います．隠れ素子数は512，隠れ層の活性化関数はReLU，出力層の活性化関数はLinearです．]
//footnote[tgate][1層のFeed-Forwardネットワークとして表現されます．出力層の活性化関数はsigmoidです．]

=== 差分スペクトル法による変換音声波形生成
本節では，ささやき声への変換を考えるので，（詳細は後述しますが）音源特徴量（F0とAP）の変換は必要ありません．
故に，差分スペクトル法を用いて変換音声波形を生成@<fn>{diffFilter}することで，変換音声の品質改善が期待できます．
//footnote[diffFilter][MCEPのドメインでは，単純に変換先話者の特徴量から変換元話者の特徴量を引いたものをフィルタとします．]

=== ささやきフィルタの設計
統計的VCにより，Sさんの音声をHさんの音声っぽく変換することができました．
しかし，本節の目的である「ささやかれている感」を出すためにはもう少し処理が必要です．
ここでは，「Hさんにささやかれているっぽい音声」を生成するための「ささやきフィルタ」を設計する方法を説明します．

==== ささやき声の特徴
文献@<fn>{Farner}によると，ささやき声には次の特徴があるそうです．
//footnote[Farner][@<href>{http://articles.ircam.fr/textes/Farner09a/index.pdf}]

 * 声帯振動はほとんど観測されません．
 * 低域において，スペクトル包絡の傾きが平坦（または逆）になります．

ささやき声の声帯振動は，基本周波数の値をすべて0にすることで再現できます．
すなわち，重要なのはスペクトル包絡の変形です．
実際にささやき声の声道特性を見てみましょう．
@<img>{spec_a}に「あ」の対数振幅スペクトルを示します．
確かに，1kHzまでの低域では，
包絡の傾きが逆になっています．
また，広域では有声音とささやき声の違いがほとんどないことも観測できます．
これらの特性を信号処理で再現できれば，ささやき声への変換ができそうです．
//image[spec_a][「あ」の対数振幅スペクトル（有声音，ささやき）][scale=0.75]

==== ささやきフィルタ
それでは，有声音をささやき声へ変換するための「ささやきフィルタ」を実際に設計します．
結論から述べると，有声音のスペクトル包絡に対し，対数領域で@<img>{ssykfilter}の特性を持つようなフィルタを足し合わせることでささやき声への変換が可能となります．
ざっくりと説明すると，あるしきい値@<m>{f_{\rm th\}}まで単調増加し，それ以降が一定の値になる特性のフィルタです．
今回は@<img>{ssykfilter}のように設計しましたが，しきい値などの設定は話者に依存するはずなので，本当はちゃんとやる必要があります@<fn>{conf}．
実際にささやきフィルタを適用させると@<img>{filtered_a}になります．
特に低域がささやき声っぽくなっている気がします．
//image[ssykfilter][ささやきフィルタの特性][scale=0.75]
//image[filtered_a][ささやきフィルタ適用後の「あ」][scale=0.75]
//footnote[conf][このへんの設定も統計モデルで出来ればいいですよね．出来るかどうかわからないけど．]

==== 耳元でささやかれている感の再現
最後に，耳元でささやかれている感を出します．
ここでは，SoX@<fn>{sox}というツールを使います．
ささやき声の音声をssyk.wavとすると，次のコマンドで「右からささやかれている感」を出すことができます．
//footnote[sox][@<href>{http://sox.sourceforge.net/}]
//cmd{
$ sox ssyk.wav ssyk_migi.wav remix 1v0.0 1v0.5
//}
後半の1v0.0 1v0.5とかが音量を調整する部分です．
左からささやかれている感も同じように出せます．

==== 音声サンプル
本節で作成した音声サンプルは@<fn>{samples_ssyk}にあります．
内容は下記の通りです．

 * Source: 変換元話者Sさんの音声
 * Source (W): Sさんのささやき声っぽい音声
 * Source (WR): Sさんのささやき声っぽいものが右から聞こえる音声
 * Converted: Sさんの声をHさんに変換した音声@<fn>{conv}
 * Converted (W): Hさんのささやき声っぽい音声
 * Converted (WR): Hさんのささやき声っぽいものが右から聞こえる音声

Sourceは気持ち悪いので聞かなくてもいいですが，Converted (W)やConverted (WR)はささやかれてるっぽさが出せているのではないかと思います．
//footnote[samples_ssyk][@<href>{http://sython.org/demo/sasayakiVC/demo.html}]
//footnote[conv][基本周波数をスケーリングした波形を一度Vocoderで生成し，それに対して差分スペクトル法を適用しています．なので，品質が若干悪くなります．]

=== 本節のまとめ
本節では，一般男性の通常発話を有名女性声優のささやき声っぽくするためのVCについて述べました．
今後は，これを用いたVRコンテンツとかができればいいなあと思っています．
誰か作ってください．

== 一般男性音声コーパス(OJaMa)の紹介
VCモデルを学習させるための音声コーパスがないと，そもそも統計的VCは実現できません．
本節では，これまでの記事で用いた音声コーパスを@<b>{「一般男性音声コーパス(OJaMa)」}@<fn>{OJaMa}と命名し，その内容について紹介します@<fn>{hsan_no_onsei}．
//footnote[OJaMa][``O''rdinary ``Ja''panese ``Ma''le speaker]
//footnote[hsan_no_onsei][残念ながら，ささやき声VCで用いたHさんのパラレル音声コーパスは公開できません…]

=== 声優統計コーパス
OJaMaは，日本声優統計学会のWebページで公開されている「声優統計コーパス」@<fn>{VAC}のパラレル音声コーパスとなっています．
声優統計コーパスには，独自に作成された音素バランス文（100文）@<fn>{VAC_txt}を，3名の女性声優（土谷麻貴さん@<fn>{tsuchiya}，上村彩子さん@<fn>{uemura}，藤東知夏さん@<fn>{fujitou}さん）が様々なスタイル（通常，喜び，怒り）で発話したものが含まれています．
同コーパスのパラレル音声コーパスとして，
Japanese speech corpus of Saruwatari-lab., University of Tokyo (JSUT)@<fn>{JSUT}のサブセット``voiceactress100''と，
nico-opendata音声読み上げデータセット@<fn>{hiho}が存在します@<fn>{VAC_para}．
筆者の所感ですが，非常に使いやすくで有益なオープンソースデータであるのは間違いないという反面，音素バランス文のテキスト読み上げの難易度が高く，素人が発話するのには向いていないかもしれません．
同コーパスの中で，特に読み上げが難しそうと感じた3文を次に示します．
//footnote[VAC][@<href>{https://voice-statistics.github.io/}]
//footnote[VAC_txt][@<href>{https://github.com/voice-statistics/voice-statistics.github.com/blob/master/assets/doc/balance_sentences.txt}]
//footnote[tsuchiya][@<href>{http://act-oz.jp/talent/tsuchiyamaki.html}]
//footnote[uemura][@<href>{http://quatrestella.co.jp/tarent/ayako_uemura/index.html}]
//footnote[fujitou][@<href>{https://twitter.com/natsuiro_usagi?lang=ja}]
//footnote[JSUT][@<href>{https://sites.google.com/site/shinnosuketakamichi/publication/jsut}]
//footnote[hiho][@<href>{https://nico-opendata.jp/ja/casestudy/2stack_voice_conversion/report.html}]
//footnote[VAC_para][実際は，声優統計コーパスの話者3名でポーズ（休止）の挿入位置が異なります．JSUTは，藤東さんのポーズ挿入位置に合わせています．nico-opendata音声読み上げデータセットはわかりません．]

1. 少年時代はロシア帝国チェルニーヒウ県プルィルークィ郡トロスチャヌィーツャ村で過ごした@<fn>{muzui}
2. このときペリメーデーはアムピクトリュオーンに同行してテーバイに来ていたリキュムニオスに妻としてあたえられた
3. モーイーデーン岩壁地域の上部はプラーサートプラウィハーン寺院遺跡へ繋がるタイ側参道に続いている
//footnote[muzui][日本人には正しく発音できなさそう]

=== OJaMaの内容
OJaMaは，20代の男性大学院生のSさんが，声優統計コーパスのパラレル音声データを収録したものです@<fn>{pause}．
音声データはすべて48kHz/16bitのwavファイルです．
収録内容を次に示します．
//footnote[pause][ポーズの挿入位置は，藤東さんとJSUTの話者に合わせています．アクセントは，藤東さんのものに似せています．]

: 通常発話 (Normal)
 単純にテキスト100文を読み上げただけのものです．

: 裏声発話 (Falset)
 テキスト100文を裏声で読み上げたものです．

: 風邪声発話 (Cold)
 風邪を引いている状態でテキスト100文を読み上げたものです．

``通常発話''と``裏声発話''は無響室で，``風邪声発話''は比較的静音な屋内環境下で収録しました．

=== ここまでのまとめ
本節では，筆者がこれまでの実験で用いた音声コーパスである「一般男性音声コーパス(OJaMa)」の内容を紹介しました．
本コーパスは @<href>{http://sython.org/Corpus/OJaMa} で公開されています．
主に個人での研究・分析目的であれば無償で利用可能です@<fn>{shoyo}．
再配布・公序良俗に反する利用などの行為はご遠慮ください．
気が向いたら追加で音声を収録するかもしれないので，ご期待ください（？）
//footnote[shoyo][（いるかどうかわかりませんが）仮に商用での利用をご希望の場合は，筆者のTwitterアカウント @SythonUK まで（DMか何かで）ご一報頂ければ幸いです]

== OJaMaを用いた事例1：裏声を用いたバ美肉VC (L0 Vol.5)
オタク界隈における近年のバーチャルYouTuber（以降、VTuberと表記）の流行は目覚ましいものがあり、
2019年3月26日の時点でのVTuberの総数は6,000人を突破したと言われています@<fn>{ascii}。
//footnote[ascii][@<href>{https://ascii.jp/elem/000/001/798/1798509/}]
最近では、VTuberを媒体として@<b>{バ}ーチャルな@<b>{美}少女に受@<b>{肉}する@<b>{「バ美肉」}がブームとなっており、
実際にバ美肉に成功したおじさん@<fn>{ojisan}は@<b>{「バ美肉おじさん」}と呼ばれています。
本節では、外見（2Dまたは3Dアバター）だけでなく、声までも美少女化したいという欲張りなバ美肉おじさんの願望を叶えるための技術を提案します。
//footnote[ojisan][年齢的に実際におじさんかどうかは不明であることが多いですが、本稿では一貫して「おじさん」と称することにします。]

=== バ美肉おじさんの種別
VTuberがここまで流行している背景には、人々の「自己表現の拡張」及び「自己承認欲求の解消」という2つの欲求があると考えられます。
「現実の世界に疲れたおじさんが美少女のアバターの皮を被り、インターネットの世界でチヤホヤされる」という行為は、
これら2つの欲求を同時に満たす非常に合理的な手段であると言えます。
本節では、バ美肉おじさんの種別を、代表的なVTuberを例に挙げながら解説します。

==== 外見の美少女化を用いたバ美肉おじさん
バ美肉には、何よりもまず外見を美少女化する必要があります。
VTuberブームの先駆けとなった代表的なVTuberの一人であるねこます氏（通称：バーチャルのじゃロリ狐娘YouTuberおじさん）@<fn>{nekomasu}は、
ケモミミの3D美少女アバター（@<img>{nekomasu}）を被ったおじさんが地声@<fn>{jigoe}で配信をするというインパクトのあるコンテンツが注目され、
まさに上記のバ美肉おじさんの先駆けともなった存在と言えるでしょう。
//footnote[nekomasu][@<href>{https://www.youtube.com/channel/UCt8tmsv8kL9Nc1sxvCo9j4Q}]
//footnote[jigoe][同氏がボイスチェンジャー等で声の美少女化をしない理由として、（1）ツールや機材のセットアップ等に時間をとられたくない、 （2）ボイスチェンジャーで変換された音声の音質が一定水準に達しないとそもそも動画を見てもらえない、という2点が挙げられています。（参考: @<href>{https://www.youtube.com/watch?v=tTmZAtfHPVU&feature=youtu.be&t=701} 11:45より）]
//image[nekomasu][ねこます氏のVRアバター][scale=0.75]

==== 外見と声の美少女化を用いたバ美肉おじさん
完全なバ美肉には、外見だけでなく、声も美少女化する必要があります。
ここでは、声の美少女化のための
1.音声認識と音声合成を用いた手法、
2.ボイスチェンジャーを用いた手法、
及び
3.統計的VCを用いた手法の3つを紹介します。

: 1.音声認識と音声合成の利用
  「生声での発声→音声認識→VOICEROIDなどの音声合成ソフトでの発声」という方法で声の美少女化を実現します。
  仕組みが非常にシンプルであるという利点がありますが、音声認識・合成に伴う発話遅延や、認識誤りによる意図しない発話の生成などが生じるという欠点があります。
  代表的なVTuberとしては、のらきゃっと氏（通称：バーチャルサキュバスおじさん、@<img>{noracat}）@<fn>{noracat}が挙げられます。
//footnote[noracat][@<href>{https://www.youtube.com/channel/UC3iwL9Yz8LcKkJsnLPevOTQ}]
//image[noracat][のらきゃっと氏のVRアバター][scale=0.5]

: 2.ボイスチェンジャーの利用
  リアチェンvoice@<fn>{rcvoice}や恋声@<fn>{koigoe}といったボイスチェンジャーソフトを利用して声の美少女化を実現します。
  リアルタイムでの声質変換が可能ですが、十分な品質の音声を生成するためにはある程度の訓練が必要、と言われています@<fn>{koigoeusing}。
  代表的なVTuberとしては、魔王マグロナ氏（通称：最強のバ美肉おじさん、@<img>{magurona}）@<fn>{magurona}や、
  竹花ノート氏（通称：癒やし系聖母バ美肉おじさん、@<img>{takebana}）@<fn>{takebana}が挙げられます。
//footnote[rcvoice][@<href>{https://crimsontech.jp/works/rcvoice/}]
//footnote[koigoe][@<href>{http://www.geocities.jp/moe_koigoe/koigoe/koigoe.html}]
//footnote[koigoeusing][筆者も実際に恋声を使ってみたのですが、ある程度の音質で変換できるようになるまでに話者自身の訓練とソフトのパラメータチューニング等が必要な気がしました。]
//footnote[magurona][@<href>{https://www.youtube.com/channel/UCPf-EnX70UM7jqjKwhDmS8g}]
//footnote[takebana][@<href>{https://www.youtube.com/channel/UCuPrcSmgo6twlAQzwYPlSkQ}]
//image[magurona][魔王マグロナ氏のVRアバター][scale=0.75]
//image[takebana][竹花ノート氏のVRアバター][scale=0.5]

: 3.統計的VCを用いた手法
  頑張ってデータを収集してVCモデルを学習させれば、自分の声を好きな声優などの声に変換する技術が実現できます。
  VCがまだまだ世の中に知られていない@<fn>{vc_recent}ということもあり、
  残念ながら声質変換技術を用いたバ美肉おじさんVTuberはいまだに存在しない@<fn>{vc-babiniku}のですが、
  ニコニコ動画では、ディープラーニング声質変換を用いたVTuber（？）の動画が投稿されており@<fn>{hiroshiba}、
  男性話者から結月ゆかりへの声質変換技術を実現しています。
//footnote[vc_recent][最近流行りつつある気がします．]
//footnote[vc-babiniku][筆者の調査不足かもしれない]
//footnote[hiroshiba][@<href>{http://www.nicovideo.jp/watch/sm33527293}]

上記の3つが代表的な手法ですが、やはり1.や2.と比較すると、まだまだ3.の統計的VCを用いた手法はそこまで浸透していない印象があります。
本節では、バ美肉おじさんになりたいという皆様のための統計的VCの可能性を探りたいと思います。

=== 音声コーパス
本節では，変換元話者を某大学の男子大学院生Sさん（OJaMaコーパスの``通常発話''）、変換先話者を声優統計コーパスの藤東知夏さん（通常発話音声）としました。
サンプリングレートは，48kHzから16kHzにダウンサンプリングしました．

=== 敵対的学習に基づくVCモデル学習（敵対的VC）の導入
VCモデルのDNNアーキテクチャは、input-to-output highway networkを用いました。
本節では、変換音声をさらに高品質化するために、近年提案されたGenerative Adversarial Network (GAN)@<fn>{gan}の枠組みに基づくVCモデルの学習（敵対的VC）@<fn>{ganvc}を導入します。
敵対的VCは、@<img>{ganvc}に示すように、変換された音声と人間の自然音声を識別する識別モデルと、
VCモデルを交互に学習させる手法です。
識別モデル@<fn>{d}は、次式で定義される識別損失を最小化するように学習されます。
//texequation{
L_{\rm D}(\bm{y}, \bm{\hat y}) = L_{\rm D,1}(\bm{y}) + L_{\rm D,0}(\bm{\hat y})
//}
//texequation{
L_{\rm D,1}(\bm{y}) = -\sum_{t} \log D(\bm{y}_t), \quad L_{\rm D,0}(\bm{\hat y}) = -\sum_{t} \log ( 1 - D(\bm{\hat y}_t) )
//}
ここで、@<m>{L_{\rm D,1\}(\bm{\cdot\})}と@<m>{L_{\rm D,0\}(\bm{\cdot\})}はそれぞれ自然音声と合成音声に対する損失です。
@<m>{L_{\rm D\}(\bm{\cdot\})}の最小化により、識別モデルは自然音声に対して1を、変換音声に対して0を出力するように学習されます。
一方で、VCモデルは、次式で定義される損失関数を最小化するように学習されます。
//texequation{
L_{\rm G}(\bm{y}, \bm{\hat y}) = L_{\rm MSE}(\bm{y}, \bm{\hat y}) + \omega_{\rm D} \mathbb{E}_{\bm{\hat y}} [ \frac{L_{\mathrm{MSE}}}{L_{\mathrm{ADV}}} ] L_{\rm ADV}(\bm{\hat y})
//}
この式の第2項@<m>{L_{\rm ADV\}(\bm{\hat y\}) = L_{\rm D,1\}(\bm{\hat y\})}は、識別モデルを騙す（すなわち、変換音声に対して1を出力させる）損失であり、
自然音声と変換音声の特徴量の分布の違いを最小化する効果があります。
故に、敵対的VCの学習は、
従来の二乗誤差最小化に、GANに由来する分布間距離最小化の正則化を加えたものとして解釈できます。
@<m>{\omega_{\rm D\}}は第2項に対する重みパラメータで、本節では1.0とします。
@<m>{\mathbb{E\}_{\bm{\hat y\}\} [L_{\mathrm{MSE\}\}/L_{\mathrm{ADV\}\}]}
は、2つの損失関数のスケールを調整する役割を持っています。
敵対的VCの枠組みをわかりやすくまとめたものを@<img>{ganvc_wakaru}@<fn>{ganvc_wakaru}に示します。
//image[ganvc][Input-to-output highway networkを用いた敵対的VC][scale=0.8]
//image[ganvc_wakaru][敵対的VCの概略図][scale=0.9]
//footnote[gan][@<href>{https://arxiv.org/abs/1406.2661}]
//footnote[ganvc][@<href>{https://ieeexplore.ieee.org/document/8063435/}]
//footnote[d][本稿では3層のFeed-Forwardネットワークを用います。隠れ素子数は128、隠れ層の活性化関数はReLU、出力層の活性化関数はsigmoidです。]
//footnote[ganvc_wakaru][@<href>{https://logmi.jp/294756} より引用]

=== 裏声を用いた差分スペクトル法に基づく統計的VC
@<img>{f0hist_normalS}及び@<img>{f0hist_fujitou}に示すように、
変換元話者と変換先話者の性別が異なる場合、F0の分布（ヒストグラム）は大きく異なるため、
差分スペクトル法によるVCを直接適用することはできません。
変換元音声のF0を線形変換して生成した波形に対して差分スペクトル法を適用する
という手法@<fn>{diffvc_f0}も考えられますが、
音声の分析・合成というプロセスを挟むことによって品質が劣化するという問題があります。
//subfigw[F0のヒストグラム]{
//image[f0hist_normalS][変換元話者（Sさん、地声）][scale=0.33]
//image[f0hist_fujitou][変換先話者（藤東さん）][scale=0.33]
//image[f0hist_uragoeS][変換元話者（Sさん、裏声）][scale=0.33]
//}
//footnote[diffvc_f0][@<href>{http://www.ieice.org/ken/paper/20160328Ib6Y/}]

本節では、
@<b>{「変換元話者が裏声で発話することにより、変換先話者が女性であってもそれなりに女性らしい高さの声を出せる」}
という仮定に基づき、裏声の音声波形に対して差分スペクトル法を適用する手法を提案します。
具体的な手順は非常にシンプルで、
1.変換元話者の男性が裏声でパラレル音声データを収録、
2.変換元話者の裏声から変換先話者への変換を（MCEPのドメインで）行うVCモデルを学習、
3.裏声音声に対し、学習済みのモデルから推定された差分スペクトルフィルタを畳み込んでVC、
となります。
Sさんの裏声音声（OJaMaコーパスの``裏声発話''）から抽出されたF0のヒストグラムを@<img>{f0hist_uragoeS}に示します。
やはり変換先話者の藤東さん（@<img>{f0hist_fujitou}）と比較すると、分布の形状は異なっていますが、
F0の範囲はそれっぽく近づいており、F0の変換をしなくても差分スペクトル法がそれなりに上手く動いてくれることが期待できます。

=== 音声サンプル
今回の実験で生成した音声サンプルを@<fn>{samples_uragoe}で公開しています。
波形生成の方法として
1.WORLDボコーダの利用と
2.差分スペクトル法の2通りを試しました。
公開しているサンプルは以下のとおりです。

 * Source (Normal): 変換元話者（Sさん）の音声（地声）
 * Source (Falsetto): 変換元話者（Sさん）の音声（裏声）
 * Target: 変換元話者（藤東さん）の音声
 * Converted (Normal-MSE): 地声の変換音声（MSE最小化で学習）
 * Converted (Normal-GAN): 地声の変換音声（敵対的声質変換で学習）
 * Converted (Falsetto-MSE): 裏声の変換音声（MSE最小化で学習）
 * Converted (Falsetto-GAN): 裏声の変換音声（敵対的声質変換で学習）

筆者が聴いた感想として、
まず、波形生成方式によらず、地声・裏声の両方で、敵対的VCを採用することで品質が改善しています。
学習に90文しか使っていないことも考慮すると、なかなか良いものができたのではないかと思います。
また、1.の中での比較では、裏声を用いた場合のほうが変換先の話者に近い音声を生成できているような気がします。
さらに、2.の中での比較では、裏声を用いた場合のほうがより自然な（機械音っぽくない）音声を生成できているような気がします。
厳密な評価をやったわけではないのですが、まとめとして、
@<b>{「裏声の男性話者から女性話者への変換を行うVCは、それなりにうまく動作する」}
と言えそうです。
//footnote[samples_uragoe][@<href>{http://sython.org/demo/uragoeVC/demo.html}]

=== 本節のまとめ
本稿では、バ美肉（@<b>{バ}ーチャルな@<b>{美}少女に受@<b>{肉}）を目的としたVTuberおじさんのためのVCとして、
裏声を用いた差分スペクトル法の枠組みを提案し、それなりに上手く動いてくれることを確認しました。
今後の課題としては、以下が考えられます。

==== 裏声収録時のコスト削減などの工夫
裏声音声のサンプルを聴くとわかるのですが、地声の音声と比べるとやはり話者が辛そうな気がします。
地声での変換モデルを裏声に適用するなどの方法で、より少ない裏声データから変換モデルを構築する枠組みが必要かもしれません。
また、パラレル音声データを収録する際に、リファレンス（変換先）の音声と録音音声のF0を可視化することで、
より高品質なデータが収集できそうです。

==== リアルタイムVCの実現
今回の実験で用いたスクリプトは全てPythonで作成しており、また、変換モデルに深層ニューラルネットを用いていることも考えると、
リアルタイムでの声質変換は厳しそうな気がします。
そう思っていた時期が私にもあったのですが，最近だとDNNベースのリアルタイムVCが提案されました@<fn>{rtvc}．
この技術を使うと，統計的VCに基づくバ美肉が本当に実現するかもしれません．
//footnote[rtvc][@<href>{https://drive.google.com/file/d/1_rU2VFDxI5f6Svnx-M5BO1chCxMXUIN_/view}]

==== 多対多VCへの拡張
現状だと、変換元と変換先の話者対が固定されており、VTuberへの応用を考えると正直あまり面白みがないという印象です。
多数話者対音声コーパスを用いた学習によって多対多VC（任意の話者 to 任意の話者）が実現できれば、
より面白く、かつ実用的なVTuber向けVCが実現できる気がします。

== OJaMaを用いた事例2：風邪声判定および風邪声VC (L0 Vol.6)
季節の移り変わりの時期，やはり気をつけたいのは風邪です．
年度始めの忙しい時期に風邪をひくと色々とつらいことでしょう．
一方で，年度始めの忙しい時期に風邪をひいたということにして，
つらい現実@<fn>{tsurai}から逃避したいという気持ちもあるかもしれません．
本節では，@<b>{(1)風邪をひいたかもしれないという人が，声だけで風邪かどうかを判定できるかどうか，}
そして@<b>{(2)通常の状態で録音された音声を風邪声っぽく変換することが可能かどうか}の基礎的検討を行います．
//footnote[tsurai][迫る原稿締切，国際会議論文やジャーナル執筆，etc...]

=== 音声コーパス
今回の実験では，Sさんが風邪を引いていないときに収録した音声（OJaMaコーパスの``通常発話''）と，Sさんが風邪をひいていたときに頑張って収録した音声（OJaMaコーパスの``風邪声発話''）を分析・比較します．
収録当時の自覚症状を以下に示します．

 * 軽度な鼻水
 * 中度な頭痛
 * 軽度な身体のだるさ
 * 学会発表のための原稿執筆や，ジャーナル執筆作業による重度なつらみ

=== 風邪声の音声特徴量分析
ここでは，風邪をひくことで音声特徴量がどう変わるのかを調査します．

==== 音声発話長
風邪をひくと，そもそも声を出すのがつらくなり，発話の速度が通常よりも遅くなることが考えられます．
これを検証するために，音声の発話長のヒストグラムを作成した結果を@<img>{dur_hist}に示します@<fn>{dur}．
全体的に，風邪声の発話長（図中の灰色バー）は，
そうでないときに収録されたもの（図中の青色バー）と比較して長くなっているのが確認できます．
//image[dur_hist][音声の発話長のヒストグラム．この図は，声優統計コーパスの91〜100番目の文章を読み上げた音声から作成しました．][scale=0.8]
//footnote[dur][発話前後の無音区間は削除しました．]

==== F0
風邪をひくと，喉が痛かったりで普段よりも高い声を出しにくくなると考えられます．
また，普段のしゃべり方よりも抑揚がなくなり，平坦な声となることも推測されます．
これらを検証するために，まず，
音声から実際に抽出されたF0のヒストグラムを作成しました．
Sさん（通常）の結果を@<img>{f0hist_kenkouS}に，
Sさん（風邪声）の結果を@<img>{f0hist_kazegoeS}にそれぞれ示します．
図より，風邪をひいた場合のF0の分布は，通常の場合と比較して鋭くなっていることが確認できます．
また，その平均値も，通常の場合と比較して小さくなっていそうです．
実際に計算した対数F0の平均と分散の値を@<table>{lf0_stats}に示します．
確かに，風邪をひくと対数F0の平均・分散ともに小さくなっていることが確認できます．実際の対数F0系列(@<img>{logF0})@<fn>{DTW}を見ても，抑揚のなさがわかります．
//subfig[F0のヒストグラム．この図は，声優統計コーパスの91〜100番目の文章を読み上げた音声から作成した．]{
//image[f0hist_kenkouS][Sさん（通常）][scale=0.5]
//image[f0hist_kazegoeS][Sさん（風邪声）][scale=0.5]
//}

//tabooular[lf0_stats][対数F0の統計量]{
          | 平均    &     分散
 =========|<======>&=========
 Sさん（通常）| 4.8034 & 0.036487
 Sさん（風邪声）| 4.6920 & 0.017122
//}

//image[logF0][音声の対数F0の時系列プロット．この図は，声優統計コーパスの91番目の文章を読み上げた音声から作成した．][scale=0.8]
//footnote[DTW][2つの系列の長さは，DTWによって揃えました．]

==== MCEP
重度な風邪をひいた場合，呂律が回らなくなって発音が曖昧になることがあると思います．
が，今回収録してもらったSさんは，そこまで深刻な状態ではなかったため，発音に関しては通常の状態とそこまで差が生じていないように感じました@<fn>{recording}．
では，MCEPの領域では，通常の状態と風邪をひいている状態でどのような差が生じるのでしょうか？
とりあえず，F0の分析と同様に，MCEPの平均と分散を計算してみました．
平均のプロットを@<img>{mcep_mean}に，
分散のプロットを@<img>{mcep_var}にそれぞれ示します．
@<img>{mcep_mean}より，MCEPの平均に関しては，通常の状態と風邪をひいた状態とでそこまで差がないことがわかります．
一方で，@<img>{mcep_var}を見ると，
音声のパワーに関連する0次のMCEPの分散は，風邪をひくと小さくなり，
高次のMCEPの分散は，風邪をひくと大きくなることが確認できます．
パワーの分散が小さくなるのは，F0の分析結果からある程度類推できるのですが，
高次のMCEPの分散が大きくなるのは面白い結果だと思います@<fn>{why_mcep}．

//subfig[MCEPの統計量．この図は，声優統計コーパスの91〜100番目の文章を読み上げた音声から作成しました．]{
//image[mcep_mean][平均][scale=0.5]
//image[mcep_var][分散（縦軸は対数スケール）][scale=0.5]
//}

//footnote[recording][収録の際に噛むこともありましたが，噛まないテイクのみを採用しました．噛んでしまったテイクも含めて分析してみると，さらにおもしろい結果が出るかもしれません（やるとは言っていない）．]
//footnote[why_mcep][なぜこのような結果になったかは，正直わかりません．実際，MCEPの各次元が音声知覚とどのように対応付いているかはまだ謎が多いです．]

==== AP
APは，声のかすれ具合を表す特徴量であり，
風邪をひいたときに最も大きな差が生じることが予想されます．
F0，MCEPと同様に，
APの平均と分散の値を計算してみました．
@<table>{ap_stats}に結果を示します．
表より，APの平均は，風邪をひくと大きくなり，
分散は小さくなることが確認できます．

//tabooular[ap_stats][APの統計量]{
          | 平均    &     分散
 =========|<======>&=========
 Sさん（通常）| 0.4583 & 0.043387
 Sさん（風邪声）| 0.5412 & 0.029487
//}

==== 風邪声の音声特徴量分析結果まとめ
風邪を引いたときの音声特徴量の変化についてまとめます．

: 音声発話長が長くなる
: F0の平均・分散がともに小さくなる
: MCEPの0次成分の分散が小さくなり，高次成分の分散が大きくなる
: APの平均が大きくなり，分散が小さくなる


=== DNNに基づく音声からの風邪判定
入力された音声から，その話者が風邪をひいているかどうかを判定する風邪判定DNN（@<img>{kaze_disc}）を構築します．
//image[kaze_disc][風邪判定器のコンセプト][scale=0.8]

==== 風邪判定DNNの学習
風邪判定DNN @<m>{D(\bm{\cdot\})}は，入力された音声特徴量から，その話者が風邪を引いているかどうかを判定します．
風邪判定DNN@<fn>{d_kaze}の学習時には，次式で定義される損失を最小化します．
//texequation{
L_{\rm D}(\bm{y}^{\rm ill}, \bm{y}^{\rm normal}) = L_{\rm D,1}(\bm{y}^{\rm ill}) + L_{\rm D,0}(\bm{y}^{\rm normal})
//}
//texequation{
L_{\rm D,1}(\bm{y}^{\rm ill}) = -\sum_{t} \log D(\bm{y}^{\rm ill}_{t}), \quad L_{\rm D,0}(\bm{y}^{\rm normal}) = -\sum_{t} \log ( 1 - D(\bm{y}^{\rm normal}_t) )
//}
ここで，@<m>{L_{\rm D,1\}(\bm{\cdot\})}と@<m>{L_{\rm D,0\}(\bm{\cdot\})}はそれぞれ風邪音声と通常音声に対する損失です．
@<m>{L_{\rm D\}(\bm{\cdot\})}の最小化により，風邪判定DNNは風邪音声に対して1を，通常音声に対して0を出力するように学習されます．
//footnote[d_kaze][本稿では3層のFeed-Forwardネットワークを用います．隠れ素子数は128，隠れ層の活性化関数はReLU、出力層の活性化関数はsigmoidです．]

==== 風邪判定DNNの学習結果
本稿では，MCEP（とその動的特徴量@<fn>{delta}）から風邪判定を行うDNNを学習させました@<fn>{excuse}．
声優統計コーパスの1〜90番目の文章を学習データ，91〜100番目の文章を評価データとし，判定の正解率を計算しました．
@<img>{accuracy}に結果を示します．
図より，
学習データに対する正解率は，ほぼ100％に近いものになっています．
評価データに対しては正解率97％程度でサチっていますが，
もう少しリッチな特徴量やDNNを使えば改善すると思います．
//footnote[delta][MCEPの時間変化（1次差分）の情報を持った特徴量です．]
//footnote[excuse][本当は特徴量の組み合わせとかも検討したり，support vector machineとかも使いたかったのですが，時間の都合上厳しかったのでダメでした．]
//image[accuracy][風邪判定DNNの正解率][scale=0.8]

=== DNNに基づく風邪声VC
DNNを用いて，通常音声の音声を風邪声っぽくする風邪声VC（@<img>{kaze_vc}）を実現します．
//image[kaze_vc][風邪声声質変換のコンセプト][scale=0.8]

=== 風邪声VC-DNN学習

風邪声VC-DNN @<m>{G(\bm{\cdot\})}は，
入力された通常音声の音声特徴量を，
風邪声音声の音声特徴量に変換します．
DNNアーキテクチャは，input-to-output highway networkを用いました．

=== 風邪声VC-DNNの学習結果
本稿では，通常音声のMCEPを風邪声っぽく変換するDNNを学習させました@<fn>{excuse_vc}．
学習・評価データについては，風邪判定DNNのものと同じです．
まず，学習後のDNNのtransform gateの例を@<img>{tgate}に示します．
この図において，gateの値が1に近い（つまり，赤に近い）ほど，
DNNはその入力特徴量を強く変換しようとしていることを意味します．
結果として，
風邪声VC-DNNは，通常音声のMCEPの低域から中域にかけて強く変換しようとしていることが確認できます．

//image[tgate][Transform gateの値の例．この図は，声優統計コーパスの91番目の文章を読み上げた音声から作成しました．][scale=0.8]
//footnote[excuse_vc][F0は入出力話者で線形変換し，APは入力話者のものをコピーします．本当はAPも変換したかった...]

今回の実験で生成した音声サンプルを@<fn>{samples_kaze}で公開しています．
波形生成には，WORLDボコーダを利用しました．
公開しているサンプルは以下のとおりです．

 * Source (N): Sさんの音声（通常）
 * Target (I): Sさんの音声（風邪声）
 * Converted: 変換音声

筆者が聴いた感想として，変換元の音声の話速は変わっていないため，風邪声っぽさが微妙に再現できていない気がします．
ただ，話速やイントネーションに関しては，話者が頑張って合わせにいけばいいので，そこまで大きな問題ではないと思います．
変換音声では，風邪をひいてテンションが下がった感は上手く再現出来ている気がします．
厳密な評価をやったわけではないのですが，まとめとして，
@<b>{「通常音声を風邪声っぽく変換する風邪声VCは，それなりにうまく動作する」}
と言えそうです．
//footnote[samples_kaze][@<href>{http://sython.org/demo/kazeVC/demo.html}]

=== 本節のまとめ
本節では，
(1)入力音声の話者が風邪をひいているかどうかを判定するDNNの学習と，
(2)通常音声を風邪声っぽく変換する風邪声VC-DNNの学習を行いました．
結果として，判定・変換のどちらもそれなりに上手く動いてくれることを確認しました．
今後の課題としては，以下が考えられます．

==== 異常検知問題としての定式化
本稿では，通常音声と風邪声音声のパラレルデータが手に入るという理想的な条件で実験を行いました．
しかし，風邪をひいているときに音声データを大量に収録するのは現実的ではないし，つらいです．
今後は，
@<b>{通常音声は大量に入手可能だが，風邪をひいている（異常な）音声は少量しか入手できない}という異常検知の枠組みで風邪声判定やVCを行えるようにする必要があると考えられます．

==== 用いる特徴量に関する調査
本稿では，MCEPを用いて風邪声判定やVCを行うという非常にシンプルな実験を行いました．
今後は，MCEP以外にもAPを用いて実験して，
どれだけ違う結果が得られるかどうか検証してみたいです．

==== 風邪の症状の深刻さを用いたモデル化
本稿では，「風邪をひいている or NOT」という2値分類の枠組みで風邪声判定・変換を行いました．
実際には，風邪の症状には段階があるはずで，単純な2値分類として解いてしまうのは面白くないです．
今後は，風邪の深刻さなども含めて段階的に判定したり，連続的に風邪声っぽさをいじれるようなVCが実現できると面白くなりそうです．

== おわりに
本稿では，筆者がこれまでに執筆した次の記事をまとめました．

 * 深層学習とささやきフィルタによる音声変換 (L0: Vol.3)
 * バ美肉おじさんになるための裏声を用いた統計的声質変換技術の基礎的検討 (L0: Vol.5)
 * 風邪声判定および風邪声声質変換のための音声分析・基礎的検討 (L0: Vol.6)

また，これまでの記事の実験で用いたOJaMaコーパスの内容も説明し，Webで公開しました．
今後も，時間と体力の許すかぎり，VC技術の面白さを伝えられる記事を執筆していきたいです．