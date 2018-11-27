#include "Buzzer.h"
#include "iodefine.h"
#include "Clock.h"
#include "Wait.h"

#define BUZ_CLOCK	(CLOCK * 1000000 / 64)	//ブザー用タイマの周波数[Hz]
#define BUZ_TGR		MTU0.TGRB
#define BUZ_START	MTU.TSTR.BIT.CST0
#define BUZ_TCNT	MTU0.TCNT
#define BUZ_TIOR	MTU0.TIORH.BIT.IOB

volatile short buzzer_count = -1;	//ブザーを鳴らす時間のカウント用
//音階周波数
const short scale[17] = {444, 494, 523, 587, 659, 698, 784, 880, 988, 1047, 1174, 1318, 1396, 1568, 1760, 1976, 2094};

//ブザー初期化
void init_buzzer(void)
{
	MPC.PWPR.BIT.B0WI = 0;			//PFSWEビット書き込み許可
	MPC.PWPR.BIT.PFSWE = 1;			//PFSレジスタ書き込み許可
	MPC.P15PFS.BIT.PSEL = 1;		//MTIOC0Bとして使用する
	MPC.PWPR.BIT.PFSWE = 0;			//PFSレジスタへの書き込みを禁止
	MPC.PWPR.BIT.B0WI = 1;			//PFSWEビットへの書き込みを禁止
	PORT1.PMR.BIT.B5 = 1;			//周辺機能入出力として使用

	MSTP_MTU0 = 0;					//MTU0スタンバイ解除
	MTU.TSTR.BIT.CST0 = 0;			//タイマー停止
	MTU0.TCR.BIT.TPSC = 3;			//動作周波数設定(CLOCK/64)
	MTU0.TCR.BIT.CCLR = 2;			//TGRBのコンペアマッチでTCNTクリア
	MTU0.TMDR.BIT.MD = 0;			//通常動作に設定
	MTU0.TIORH.BIT.IOB = 3;			//初期出力Lコンペアマッチでトグル
	MTU0.TGRB = 0;
}

//ブザーを鳴らし始める関数
//frequency		周波数[Hz]
//time			鳴らす時間[ms]
//時間が過ぎたら自動的に音が止む
//timeに0を指定すると鳴り続ける
void start_buzzer(short frequency, short time)
{
	buzzer_count = time - 1;
	BUZ_TGR = BUZ_CLOCK / 2 / frequency;
	BUZ_START = 0;
	BUZ_TIOR = 3;
	BUZ_START = 1;
}

//ブザーが鳴っている間，wait関数で待つ
//frequency		周波数[Hz]
//time			待つ時間[ms]
void sound_buzzer(short frequency, short time)
{
	start_buzzer(frequency, 0);
	wait_ms(time);
	stop_buzzer();
}

//ブザー割り込み関数
void buzzer_interrupt(void)
{
	if(buzzer_count < 0){return;}
	else if(buzzer_count > 0){
		buzzer_count--;
	}
	else{
		stop_buzzer();
	}
}

void stop_buzzer(void)
{
	BUZ_START = 0;
	BUZ_TCNT = 0;
	BUZ_TIOR = 0;	//端子出力を禁止
	buzzer_count = -1;
}

void level_up_buzzer(void)
{
	//ふぁふぁふぁふぁみｂそふぁ
	wait_ms(200);
	sound_buzzer(scale[BUZ_FA2], 100);
	wait_ms(5);
	sound_buzzer(scale[BUZ_FA2], 100);
	wait_ms(5);
	sound_buzzer(scale[BUZ_FA2], 100);
	wait_ms(5);
	sound_buzzer(scale[BUZ_FA2], 100);
	wait_ms(100);
	sound_buzzer(1224, 100);
	wait_ms(100);
	sound_buzzer(scale[BUZ_SO2], 100);
	wait_ms(100);
	sound_buzzer(scale[BUZ_FA2], 700);
	wait_ms(200);
}

void hotel_buzzer(void){
	//レード♯ードーシーラ レ(低い方） レー（高い方）♪ 宿屋ＢＧＭです。

	wait_ms(200);
	sound_buzzer(scale[BUZ_RE2], 100);
	wait_ms(50);
	sound_buzzer(1108, 100);
	wait_ms(50);
	sound_buzzer(scale[BUZ_DO2], 100);
	wait_ms(50);
	sound_buzzer(scale[BUZ_SI2], 100);
	wait_ms(50);
	sound_buzzer(scale[BUZ_LA2], 150);
	wait_ms(50);
	sound_buzzer(scale[BUZ_RE1], 150);
	wait_ms(50);
	sound_buzzer(scale[BUZ_RE2], 400);
	wait_ms(200);

}

void church_save_buzzer(void){
	sound_buzzer(scale[BUZ_MI2], 400);
	wait_ms(40);
	sound_buzzer(scale[BUZ_RE2], 200);
	wait_ms(40);
	sound_buzzer(scale[BUZ_DO2], 200);
	wait_ms(40);
	sound_buzzer(scale[BUZ_SI2], 400);
	wait_ms(140);
	sound_buzzer(scale[BUZ_DO2], 150);
	wait_ms(40);
	sound_buzzer(scale[BUZ_DO2], 600);
}

void coin_buzzer(void){
    sound_buzzer(scale[BUZ_SI1]*4, 100);
    wait_ms(5);
    sound_buzzer(scale[BUZ_MI2]*4, 200);
    //wait_ms(5);
    //sound_buzzer(scale[BUZ_MI2]*2, 200);
}

void pipo_buzzer(void){
    sound_buzzer(2000, 150);
    wait_ms(20);
    sound_buzzer(1000, 150);
    wait_ms(20);
}
