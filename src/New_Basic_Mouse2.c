/****************************************************************************/
/*																			*/
/*			NewBasicMouse ver 0.1			2017/06/13						*/
/*																			*/
/****************************************************************************/


#include "iodefine.h"
#include "Clock.h"
#include "UI.h"
#include "Wait.h"
#include "SCI.h"
#include "xprintf.h"
#include "ADC.h"
#include "Interrupt.h"
#include "Motor.h"
#include "Wall_Sensor.h"
#include "Buzzer.h"
#include "Battery.h"
#include "My_Func.h"
#include "E2Flash_RX220.h"
#include "Map.h"
#include "Control.h"
#include "Run.h"
#include "Log.h"


void mode0(void);
void mode1(void);
void mode2(void);
void mode3(void);
void mode4(void);
void mode5(void);
void mode6(void);
void mode7(void);
void mode8(void);
void mode9(void);
void mode10(void);
void mode11(void);
void mode12(void);
void mode13(void);
void mode14(void);
void mode15(void);
void select_mode(void);
void init(void);
void pullup_unused_port(void);

void main(void)
{
	init();
	//check_battery();	//動作確認が終わったらコメントアウトを外してください
	reset_log();

	while(1){
		select_mode();
	}
}

//タクトスイッチを押したらLED点灯
void mode0(void)
{
	while(1){
		if(get_sw_state(DOWN)){
			LED_on(1);
		}
		else{
			LED_off(1);
		}
		if(get_sw_state(UP)){
			LED_on(2);
		}
		else{
			LED_off(2);
		}
		if(get_sw_state(EXEC)){
			LED_on(3);
		}
		else{
			LED_off(3);
		}
	}
}

//バッテリ電圧をPC画面に表示
void mode1(void)
{
	while(1){
		xprintf("battery:%5d[mV]\n", battery_vol);
		wait_ms(100);
	}
}

//ブザーを鳴らす
void mode2(void)
{
	while(1){
		sound_buzzer(scale[BUZ_LA0], 500);
		sound_buzzer(scale[BUZ_SI0], 500);
		sound_buzzer(scale[BUZ_DO1], 500);
		sound_buzzer(scale[BUZ_RE1], 500);
		sound_buzzer(scale[BUZ_MI1], 500);
		sound_buzzer(scale[BUZ_FA1], 500);
		sound_buzzer(scale[BUZ_SO1], 500);
		sound_buzzer(scale[BUZ_LA1], 500);
		sound_buzzer(scale[BUZ_SI1], 500);
		sound_buzzer(scale[BUZ_DO2], 500);
		sound_buzzer(scale[BUZ_RE2], 500);
		sound_buzzer(scale[BUZ_MI2], 500);
		sound_buzzer(scale[BUZ_FA2], 500);
		sound_buzzer(scale[BUZ_SO2], 500);
		sound_buzzer(scale[BUZ_LA2], 500);
		sound_buzzer(scale[BUZ_SI2], 500);
		sound_buzzer(scale[BUZ_DO3], 500);
	}
}

//壁センサーをPC画面に表示
void mode3(void)
{
	output_wall_sensor();
}

//タクトスイッチを押すとモーターが回る
void mode4(void)
{
	while(1){
		if(get_sw_state(UP) && get_sw_state(DOWN)){
			change_motor_speed(200, 200);
		}
		else if(get_sw_state(UP)){
			change_motor_speed(0, 200);
		}
		else if(get_sw_state(DOWN)){
			change_motor_speed(200, 0);
		}
		else{
			change_motor_speed(0, 0);
		}
		wait_ms(50);
		xprintf("left_step:%6d, right_step:%6d\n", motor_step_l, motor_step_r);
	}
}

//指定区画前進
void mode5(void)
{
	short n = get_num();
	set_wall_gain(5);		//壁制御ゲインをセット

	turn_on_motor();
	wait_sw_on();
	wait_ms(1000);
	update_center_ref();
	LED_on(4);
	straight(SECTION*n, 500, 0, 1, 1);
	LED_off(4);
}

//その場旋回
void mode6(void)
{
	turn_on_motor();
	wait_sw_on();
	wait_ms(1000);
	LED_on(4);
	for(short i = 0; i < 1; i++){
		cturn(360*5);
	}
	LED_off(4);
}

//左手法
void mode7(void)
{
	wait_sw_on();
	wait_ms(1000);

	straight(70, -100, 0, 2, 0);	//ケツタッチ
	set_wall_gain(5);
	straight(34, 500, 0, 2, 0);
	turn(90);
	straight(70, -100, 0, 2, 0);	//ケツタッチ
	straight(34, 500, 0, 2, 0);
	turn(-90);

	wait_ms(500);
	update_center_ref();
	set_wall_gain(5);
	short acc=4;
	straight(H_SECTION , LEFT_SPEED, LEFT_SPEED, acc, 1);

	while(1){
		if(wall_ls == 0){							//左壁なし
			straight(H_SECTION, LEFT_SPEED, 0, acc, 1);
			turn(90);
			straight(H_SECTION, LEFT_SPEED, LEFT_SPEED, acc, 1);
		}
		else if((wall_lf == 0) && (wall_rf == 0)){	//前壁なし
			straight(SECTION, LEFT_SPEED, LEFT_SPEED, acc, 1);
		}
		else if(wall_rs == 0){						//右壁なし
			straight(H_SECTION, LEFT_SPEED, 0, acc, 1);
			turn(-90);
			straight(H_SECTION, LEFT_SPEED, LEFT_SPEED, acc, 1);
		}
		else{										//袋小路
			straight(H_SECTION, LEFT_SPEED, 0, acc, 1);
			turn(180);
			straight(H_SECTION, LEFT_SPEED, LEFT_SPEED, acc, 1);
		}
	}
}

//mapデータの保存
void mode8(void)
{
	reset_map();

	//各区画の東側の壁だけセットする
	for(short x = 0; x < MAZE_SIZE; x++){
		for(short y = 0; y < MAZE_SIZE; y++){
			set_map(x, y, 1, 1);
		}
	}

	output_map();	//map配列をPCに出力
	save_map();		//map配列をデータフラッシュに保存
	reset_map();	//map配列をリセット
	load_map();		//データフラッシュからmap配列にデータをロード
	output_map();	//map配列をPCに出力
}

//ログ取り
void mode9(void)
{
	set_log_var(sensor_ls, short, 0);
	set_log_var(sensor_rs, short, 1);
	set_log_var(now_speed, short, 2);

	turn_on_motor();
	wait_sw_on();
	wait_ms(1000);
	update_center_ref();
	set_wall_gain(5);

	LED_on(4);
	//start_ring_log();
	start_log();
	straight(SECTION*2, LEFT_SPEED, 0, 4, 1);
	LED_off(4);

	output_log();
}

//
void mode10(void)
{
	wait_sw_on();
	wait_ms(1000);

	straight(70, -100, 0, 2, 0);	//ケツタッチ
	set_wall_gain(5);
	straight(34, 500, 0, 2, 0);
	turn(90);
	straight(70, -100, 0, 2, 0);	//ケツタッチ
	straight(34, 500, 0, 2, 0);
	turn(-90);

	wait_ms(500);

	cturn(720);

}

//
void mode11(void)
{

}

//
void mode12(void)
{

}

//
void mode13(void)
{

}

//
void mode14(void)
{

}

//
void mode15(void)
{

}

//モードを選択する
void select_mode(void)
{
	switch(get_num()){
	case  0:mode0();break;
	case  1:mode1();break;
	case  2:mode2();break;
	case  3:mode3();break;
	case  4:mode4();break;
	case  5:mode5();break;
	case  6:mode6();break;
	case  7:mode7();break;
	case  8:mode8();break;
	case  9:mode9();break;
	case 10:mode10();break;
	case 11:mode11();break;
	case 12:mode12();break;
	case 13:mode13();break;
	case 14:mode14();break;
	case 15:mode15();break;
	}
}

//初期設定
void init(void)
{
	init_clock();
	pullup_unused_port();
	init_ui();
	init_wait();
	init_SCI();
	xdev_out(print_char_sci);	//xprintf関数に1文字出力する関数を設定する
	init_ADC();
	init_buzzer();
	init_wall_sensor();
	init_motor();
	init_Data_Flash();
	init_interrupt_1ms();
	init_interrupt_50us();
}

//使ってないポートをプルアップに設定する
void pullup_unused_port(void)
{
	PORT4.PCR.BIT.B4 = 1;
	PORTE.PCR.BIT.B0 = 1;
	PORTE.PCR.BIT.B1 = 1;
	PORTE.PCR.BIT.B2 = 1;
	PORTE.PCR.BIT.B3 = 1;
	PORTE.PCR.BIT.B4 = 1;
	PORTE.PCR.BIT.B5 = 1;
	PORTA.PCR.BIT.B0 = 1;
	PORTA.PCR.BIT.B1 = 1;
	PORTA.PCR.BIT.B3 = 1;
	PORTA.PCR.BIT.B4 = 1;
	PORTA.PCR.BIT.B6 = 1;
	PORT0.PCR.BIT.B3 = 1;
	PORT0.PCR.BIT.B5 = 1;
	PORTB.PCR.BIT.B7 = 1;
	PORTC.PCR.BIT.B3 = 1;
	PORTC.PCR.BIT.B4 = 1;
	PORTC.PCR.BIT.B5 = 1;
	PORTH.PCR.BIT.B1 = 1;
	PORT1.PCR.BIT.B6 = 1;
	PORT2.PCR.BIT.B7 = 1;
}
