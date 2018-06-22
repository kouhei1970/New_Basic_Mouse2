#include "Interrupt.h"
#include "iodefine.h"
#include "Clock.h"
#include "Buzzer.h"
#include "Wall_Sensor.h"
#include "Battery.h"
#include "Motor.h"
#include "Control.h"
#include "Log.h"

//1ms割り込み初期設定
void init_interrupt_1ms(void)
{
	MSTP_MTU1 = 0;					//モジュールストップ解除
	MTU.TSTR.BIT.CST1 = 0;			//タイマー停止
	MTU1.TCR.BIT.TPSC = 2;			//動作周波数設定(CLOCK/16)
	MTU1.TCR.BIT.CCLR = 1;			//TGRAのコンペアマッチでTCNTクリア
	MTU1.TGRA = CLOCK*1000/16 - 1;	//1msでコンペアマッチ
	MTU1.TIER.BIT.TGIEA = 1;		//割り込み要求許可
	IR(MTU1,TGIA1) = 0;				//割り込み要求フラグクリア
	IEN(MTU1,TGIA1) = 1;			//割り込み要求許可
	IPR(MTU1,TGIA1) = 12;			//割り込み優先度設定
	MTU.TSTR.BIT.CST1 = 1;			//タイマースタート
}

//50us割り込み初期設定
void init_interrupt_50us(void)
{
	MSTP_MTU2 = 0;					//モジュールストップ解除
	MTU.TSTR.BIT.CST2 = 0;			//タイマー停止
	MTU2.TCR.BIT.TPSC = 1;			//動作周波数設定(CLOCK/4)
	MTU2.TCR.BIT.CCLR = 1;			//TGRAのコンペアマッチでTCNTクリア
	MTU2.TGRA = CLOCK*50/4 - 1;		//50usでコンペアマッチ
	MTU2.TIER.BIT.TGIEA = 1;		//割り込み要求許可
	IR(MTU2,TGIA2) = 0;				//割り込み要求フラグクリア
	IEN(MTU2,TGIA2) = 1;			//割り込み要求許可
	IPR(MTU2,TGIA2) = 13;			//割り込み優先度設定
	MTU.TSTR.BIT.CST2 = 1;			//タイマースタート
}

//1ms毎に実行される関数
void interrupt_1ms(void)
{
	control_speed();			//速度制御割り込み
	turn_off_motor_timer(0);	//モータードライバ電源管理割り込み
	log_interrupt();			//ログ取得割り込み
	buzzer_interrupt();			//ブザー消音割り込み
	check_battery_interrupt();	//バッテリ電圧監視割り込み
}

//50us毎に実行される関数
//AD変換中に別の割り込みからAD変換が始まらないように，AD変換を行う関数はこの関数から呼んだほうがよい
void interrupt_50us(void)
{
	static char pointer = 0;

	switch(pointer){
	case  0:update_wall_sensor(SEN_LF, 0);break;	//LF壁センサー値を消灯した状態で取得
	case  1:update_wall_sensor(SEN_LF, 1);break;	//LF壁センサー値を点灯した状態で取得
	case  2:break;
	case  3:break;
	case  4:break;
	case  5:update_wall_sensor(SEN_LS, 0);break;
	case  6:update_wall_sensor(SEN_LS, 1);break;
	case  7:break;
	case  8:break;
	case  9:break;
	case 10:update_wall_sensor(SEN_RS, 0);break;
	case 11:update_wall_sensor(SEN_RS, 1);break;
	case 12:break;
	case 13:break;
	case 14:break;
	case 15:update_wall_sensor(SEN_RF, 0);break;
	case 16:update_wall_sensor(SEN_RF, 1);break;
	case 17:break;
	case 18:break;
	case 19:update_battry();break;					//バッテリ電圧取得
	}

	if(++pointer >= 20){pointer = 0;}
}
