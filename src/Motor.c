#include "Motor.h"
#include "iodefine.h"
#include "Clock.h"
#include "My_Func.h"
#include "Mouse_Data.h"
#include "Control.h"

#define MOTOR_DIR_L		PORTH.PODR.BIT.B3
#define MOTOR_GO_L		0
#define MOTOR_BACK_L	1
#define MOTOR_DIR_R		PORTB.PODR.BIT.B5
#define MOTOR_GO_R		1
#define MOTOR_BACK_R	0

#define MOTOR_START_L	MTU.TSTR.BIT.CST3
#define MOTOR_TGR_L		MTU3.TGRC
#define MOTOR_START_R	MTU.TSTR.BIT.CST4
#define MOTOR_TGR_R		MTU4.TGRC

#define MOTOR_EN		PORTH.PODR.BIT.B2

#define MOTOR_CLOCK		(CLOCK * 1000000 / 16)

#define TURN_OFF_MOTOR_TIME	1000//モーターを無効にするまでの待ち時間[ms]

volatile unsigned long motor_step_l;	//左モータステップ数
volatile unsigned long motor_step_r;	//右モータステップ数
volatile unsigned long motor_step_sum;	//合計モータステップ数

//モーター初期設定
void init_motor(void)
{
	//IO設定
	PORTH.PDR.BIT.B3 = 1;		//PH3(L_MOTOR_DIR)を出力に設定
	PORTB.PDR.BIT.B5 = 1;		//PB5(R_MOTOR_DIR)を出力に設定
	PORTH.PDR.BIT.B2 = 1;		//PH2(MOTOR_EN)を出力に設定

	MOTOR_EN = MOTOR_OFF;

	MPC.PWPR.BIT.B0WI = 0;		//PFSWEビットへの書き込みを許可
	MPC.PWPR.BIT.PFSWE = 1;		//PFSレジスタへの書き込みを許可
	MPC.P14PFS.BIT.PSEL = 1;	//P14(L_MOTOR_STEP)をMTIOC0Aに設定
	MPC.PB3PFS.BIT.PSEL = 2;	//PB3(R_MOTOR_STEP)をMTIOC0Cに設定
	MPC.PWPR.BIT.PFSWE = 0;		//PFSレジスタへの書き込みを禁止
	MPC.PWPR.BIT.B0WI = 1;		//PFSWEビットへの書き込みを禁止
	PORT1.PMR.BIT.B4 = 1;		//周辺モジュールとして使用
	PORTB.PMR.BIT.B3 = 1;		//周辺モジュールとして使用

	//MTU3設定
	MSTP_MTU3 = 0;				//MTU3スタンバイ解除
	MTU.TSTR.BIT.CST3 = 0;		//MTU3タイマー停止
	MTU3.TCR.BIT.TPSC = 2;		//動作周波数設定:CLOCK/16
	MTU3.TCR.BIT.CCLR = 1;		//TGRAのコンペアマッチでTCNTクリア
	MTU3.TMDR.BIT.MD = 2;		//PWMモード1に設定
	MTU3.TMDR.BIT.BFA = 1;		//TGRAとTGRCレジスタはバッファ動作
	MTU3.TIORH.BIT.IOA = 2;		//初期出力L,コンペアマッチでH
	MTU3.TIORH.BIT.IOB = 1;		//初期出力L,コンペアマッチでL
	MTU3.TGRA = 0;
	MTU3.TGRB = 40 - 1;
	MTU3.TGRC = 0;
	MTU3.TIER.BIT.TGIEA = 1;	//割り込み要求許可
	IR(MTU3,TGIA3) = 0;			//割り込み要求フラグクリア
	IEN(MTU3,TGIA3) = 1;		//割り込み要求許可
	IPR(MTU3,TGIA3) = 14;		//割り込み優先度設定

	//MTU4設定
	MSTP_MTU4 = 0;				//MTU4スタンバイ解除
	MTU.TSTR.BIT.CST4 = 0;		//MTU4タイマー停止
	MTU4.TCR.BIT.TPSC = 2;		//動作周波数設定:CLOCK/16
	MTU4.TCR.BIT.CCLR = 1;		//TGRAのコンペアマッチでTCNTクリア
	MTU4.TMDR.BIT.MD = 2;		//PWMモード1に設定
	MTU4.TMDR.BIT.BFA = 1;		//TGRAとTGRCレジスタはバッファ動作
	MTU.TOER.BIT.OE4A = 1;		//出力許可 MTU4では必要
	MTU4.TIORH.BIT.IOA = 2;		//初期出力L,コンペアマッチでH
	MTU4.TIORH.BIT.IOB = 1;		//初期出力L,コンペアマッチでL
	MTU4.TGRA = 0;
	MTU4.TGRB = 40 - 1;
	MTU4.TGRC = 0;
	MTU4.TIER.BIT.TGIEA = 1;	//割り込み要求許可
	IR(MTU4,TGIA4) = 0;			//割り込み要求フラグクリア
	IEN(MTU4,TGIA4) = 1;		//割り込み要求許可
	IPR(MTU4,TGIA4) = 14;		//割り込み優先度設定

	turn_off_motor();
}

//左モータ割り込み
void motor_interrpt_l(void)
{
	motor_step_l++;
	motor_step_sum++;
}

//右モータ割り込み
void motor_interrpt_r(void)
{
	motor_step_r++;
	motor_step_sum++;
}

//ステップ数の変数を0に初期化
void reset_motor_step(void)
{
	motor_step_l = 0;
	motor_step_r = 0;
	motor_step_sum = 0;
}

//モータの速度を変える関数
//speed_l	左モータ速度[mm/s]
//speed_r	右モータ速度[mm/s]
//正の場合は正転，負の場合は逆転
void change_motor_speed(short speed_l, short speed_r)
{
	unsigned short tgr_l, tgr_r;
	char start_l, start_r;

	turn_off_motor_timer(TURN_OFF_MOTOR_TIME);

	if(speed_l != 0){
		if(speed_l > 0){
			MOTOR_DIR_L = MOTOR_GO_L;
		}
		else{
			MOTOR_DIR_L = MOTOR_BACK_L;
			speed_l = -speed_l;
		}
		if(speed_l < MIN_SPEED){speed_l = MIN_SPEED;}
		if(speed_l > MAX_SPEED){speed_l = MAX_SPEED;}
		tgr_l = (unsigned short)((MOTOR_CLOCK / MOTOR_STEP_NUM * (long)(PI*TIRE_DIAMETER) / 1000) / speed_l);
		start_l = 1;
	}
	else{
		tgr_l = 0;
		start_l = 0;
	}

	if(speed_r != 0){
		if(speed_r > 0){
			MOTOR_DIR_R = MOTOR_GO_R;
		}
		else{
			MOTOR_DIR_R = MOTOR_BACK_R;
			speed_r = -speed_r;
		}
		if(speed_r < MIN_SPEED){speed_r = MIN_SPEED;}
		if(speed_r > MAX_SPEED){speed_r = MAX_SPEED;}
		tgr_r = (unsigned short)((MOTOR_CLOCK / MOTOR_STEP_NUM * (long)(PI*TIRE_DIAMETER) / 1000) / speed_r);
		start_r = 1;
	}
	else{
		tgr_r = 0;
		start_r = 0;
	}

	MOTOR_TGR_L = tgr_l;
	MOTOR_TGR_R = tgr_r;
	MOTOR_START_L = start_l;
	MOTOR_START_R = start_r;
}

//モータドライバ電源管理タイマー
//現在の速度が0の場合，指定時間後にモータドライバを無効にする
//time	指定時間 0を渡すとcountをデクリメントする割り込み処理を行う
void turn_off_motor_timer(short time)
{
	static short count;

	if(time > 0){
		count = time;
		turn_on_motor();
	}
	else{
		if(count > 0){
			count--;
		}
		else if(count == 0){
			turn_off_motor();
			count = -1;
		}
	}
}

//モータードライバ電源ON
void turn_on_motor(void)
{
	MOTOR_EN = MOTOR_ON;
}

//モータードライバ電源OFF
void turn_off_motor(void)
{
	MOTOR_EN = MOTOR_OFF;
	MOTOR_START_L = 0;
	MOTOR_START_R = 0;
}

//現在のモータードライバ電源の状態を返す
char get_motor_en_status(void)
{
	return MOTOR_EN;
}

