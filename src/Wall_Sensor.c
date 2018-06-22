#include "Wall_Sensor.h"
#include "iodefine.h"
#include "ADC.h"
#include "UI.h"
#include "xprintf.h"
#include "Wait.h"

#define SEN_LED_LF	PORT5.PODR.BIT.B5
#define SEN_LED_LS	PORTH.PODR.BIT.B0
#define SEN_LED_RS	PORTC.PODR.BIT.B6
#define SEN_LED_RF	PORT5.PODR.BIT.B4
#define SEN_LED_ON	1
#define SEN_LED_OFF	0

//壁センサのAD変換ポートの数字（AN00X）
#define AN_LF		0
#define AN_LS		1
#define AN_RS		2
#define AN_RF		3

char wall_sensor_en = 0;//壁センサ有効フラグ
						//この変数を1にすると割り込み内で壁センサの値を自動で取得する
char	wall_control_en = 0;//壁制御有効フラグ
short	sensor_lf;		//左前センサー値
short	sensor_ls;		//左横センサー値
short	sensor_rs;		//右横センサー値
short	sensor_rf;		//右前センサー値
short	ref_ls;			//左横センサー基準値
short	ref_rs;			//右横センサー基準値
char	wall_lf;		//左前センサー壁有無
char	wall_ls;		//左横センサー壁有無
char	wall_rs;		//右前センサー壁有無
char	wall_rf;		//右前センサー壁有無
short	thre_lf = 300;	//左前センサー壁有無しきい値
short	thre_ls = 500;	//左横センサー壁有無しきい値
short	thre_rs = 500;	//右横センサー壁有無しきい値
short	thre_rf = 300;	//右前センサー壁有無しきい値

void init_wall_sensor(void)
{
	PORTH.PDR.BIT.B0 = 1;//SEN_LED_LS出力
	PORT5.PDR.BIT.B5 = 1;//SEN_LED_LF出力
	PORT5.PDR.BIT.B4 = 1;//SEN_LED_RF出力
	PORTC.PDR.BIT.B6 = 1;//SEN_LED_RS出力
	SEN_LED_LF = SEN_LED_OFF;
	SEN_LED_LS = SEN_LED_OFF;
	SEN_LED_RS = SEN_LED_OFF;
	SEN_LED_RF = SEN_LED_OFF;
}

//壁センサー値取得
//割り込み関数から呼ぶこと
//sensor	どのセンサの値を取得するか
//led		0:LED消灯 1:LED点灯
void update_wall_sensor(char sensor, char led)
{
	if(!wall_sensor_en){
		SEN_LED_LF = SEN_LED_OFF;
		SEN_LED_LS = SEN_LED_OFF;
		SEN_LED_RS = SEN_LED_OFF;
		SEN_LED_RF = SEN_LED_OFF;
		return;
	}

	static short raw0, raw1;
	if(led == 0){
		switch(sensor){
		case SEN_LF:
			raw0 = ad_convert(AN_LF);		//LED消灯時のセンサー値取得
			SEN_LED_LF = SEN_LED_ON;		//LED点灯
			break;
		case SEN_LS:
			raw0 = ad_convert(AN_LS);
			SEN_LED_LS = SEN_LED_ON;
			break;
		case SEN_RS:
			raw0 = ad_convert(AN_RS);
			SEN_LED_RS = SEN_LED_ON;
			break;
		case SEN_RF:
			raw0 = ad_convert(AN_RF);
			SEN_LED_RF = SEN_LED_ON;
			break;
		}
	}
	else if(led == 1){
		switch(sensor){
		case SEN_LF:
			raw1 = ad_convert(AN_LF);			//LED点灯時のセンサー値取得
			SEN_LED_LF = SEN_LED_OFF;			//LED消灯
			sensor_lf = raw1 - raw0;			//差分を取る
			if(sensor_lf < 0){sensor_lf = 0;}	//センサー値が負になったら0にする
			wall_lf = sensor_lf / thre_lf;		//壁有無をセット
			break;
		case SEN_LS:
			raw1 = ad_convert(AN_LS);
			SEN_LED_LS = SEN_LED_OFF;
			sensor_ls = raw1 - raw0;
			if(sensor_ls < 0){sensor_ls = 0;}
			wall_ls = sensor_ls / thre_ls;
			break;
		case SEN_RS:
			raw1 = ad_convert(AN_RS);
			SEN_LED_RS = SEN_LED_OFF;
			sensor_rs = raw1 - raw0;
			if(sensor_rs < 0){sensor_rs = 0;}
			wall_rs = sensor_rs / thre_rs;
			break;
		case SEN_RF:
			raw1 = ad_convert(AN_RF);
			SEN_LED_RF = SEN_LED_OFF;
			sensor_rf = raw1 - raw0;
			if(sensor_rf < 0){sensor_rf = 0;}
			wall_rf = sensor_rf / thre_rf;
			break;
		}
	}
}

//壁との差を返す
//左壁に近い場合は正の値を返す
//右壁に近い場合は負の値を返す
//return値	壁基準値との差
short get_wall_diff(void)
{
	//壁制御が無効もしくは壁センサ値取得が無効の場合は0を返す
	if(wall_control_en == 0 || wall_sensor_en == 0){return 0;}

	//基準値との差を取る
	short diff_l, diff_r;
	diff_l = (sensor_ls - ref_ls);
	diff_r = (sensor_rs - ref_rs);

	//壁の有無情報をwall_satateに入れる
	char wall_state = 0;
	if(wall_ls){
		wall_state += 1;
	}
	if(wall_rs){
		wall_state += 2;
	}

	short diff;
	switch(wall_state){
	case 0://両壁なし
		diff = 0;
		break;
	case 1://左壁のみ
		diff = diff_l;
		break;
	case 2://右壁のみ
		diff = -diff_r;
		break;
	case 3://両壁あり
		if(diff_l > diff_r){	//壁に近い方のセンサを使う
			diff = diff_l;
		}
		else{
			diff = -1.0 * diff_r;
		}
		break;
	}

	return diff;
}

//マウスを区画中央に置いてあるときの左右のセンサー値(基準値)を更新する
void update_center_ref(void)
{
	long ls_temp = 0, rs_temp = 0;
	const short ave_num = 50;

	wall_sensor_en = 1;
	wait_ms(5);

	//平均化のための積算
	for(short i = 0; i < ave_num; i++){
		ls_temp += sensor_ls;
		rs_temp += sensor_rs;
		wait_ms(1);
	}

	//平均化
	ref_ls = ls_temp / ave_num;
	ref_rs = rs_temp / ave_num;

	wall_sensor_en = 0;
}

//壁センサーの値を出力する
void output_wall_sensor(void)
{
	wall_sensor_en = 1;
	while(!get_sw_state(EXEC)){//EXECスイッチが押されたらループから抜ける
		wait_ms(50);
		xprintf("lf:%4d   ls:%4d   rs:%4d   rf:%4d\n", sensor_lf, sensor_ls, sensor_rs, sensor_rf);
	}
	wait_sw_off();
	wall_sensor_en = 0;
}
