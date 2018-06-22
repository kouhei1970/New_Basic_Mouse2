#include "Battery.h"
#include "Mouse_Data.h"
#include "ADC.h"
#include "UI.h"
#include "Wait.h"
#include "Buzzer.h"
#include "Wall_Sensor.h"
#include "Motor.h"

short battery_vol;		//バッテリ電圧[mV]

//バッテリ電圧を更新する
//この関数は50us割り込みから呼ばれる
void update_battry(void)
{
	battery_vol = ((long)ad_convert(AN_BAT)*3300*11)/4095;	//3.3V:4095=分圧した電圧:AD変換値
}

//バッテリ電圧をLEDに表示する
//電圧が低すぎる場合は無限ループに入る
void check_battery(void)
{
	//バッテリ電圧の平均値を取得
	wait_ms(5);
	long battery_ave = 0;
	const short ave_num = 100;
	for(short i = 0; i < ave_num; i++){
		battery_ave += battery_vol;
		wait_ms(1);
	}
	battery_ave /= ave_num;

	//電圧が低すぎる場合は無限ループ
	if(battery_ave <= 3400*BAT_CELL){
		warning_battery();
	}

	//LEDで表示
	LED_clear();
	if(battery_ave > 3400*BAT_CELL){LED_on(1);}
	wait_ms(50);
	if(battery_ave > 3600*BAT_CELL){LED_on(2);}
	wait_ms(50);
	if(battery_ave > 3800*BAT_CELL){LED_on(3);}
	wait_ms(50);
	if(battery_ave > 4000*BAT_CELL){LED_on(4);}
	wait_ms(1000);
	LED_clear();
}

//1ms割り込みでバッテリ電圧を監視する
//1秒連続で電圧が低すぎる場合は無限ループに入る
void check_battery_interrupt(void)
{
	static short count;
	const short time = 1000;

	if(battery_vol <= 3400*BAT_CELL){
		if(++count > time){
			wall_sensor_en = 0;
			turn_off_motor();
			warning_battery();
		}
	}
	else{
		count = 0;
	}
}

//バッテリ電圧が低すぎる場合の警告を行う関数
void warning_battery(void)
{
	LED_clear();
	while(1){
		LED_on(1);
		sound_buzzer(scale[BUZ_LA2], 100);
		LED_off(1);
		wait_ms(100);
	}
}
