#include "Control.h"
#include "Motor.h"
#include "My_Func.h"
#include "Wall_Sensor.h"

char	speed_control_en = 0;	//速度制御有効フラグ
short	target_speed;			//目標速度[mm/s]
short	acceleration;			//加速度[m/s^2]
short	now_speed;				//現在の中心速度[mm/s]
short	now_speed_l;			//現在の左モータ速度[mm/s]
short	now_speed_r;			//現在の右モータ速度[mm/s]
char	run_mode;				//走行モード	STRAIGHT:直線 TURN:超信地旋回
char	turn_dir;				//超信地旋回時の回転方向	1:反時計回り -1:時計回り
short	wall_gain;				//壁制御ゲイン

//速度制御を行う関数
//1ms割り込みから呼ばれる
//speed_control_enを1に設定し，target_speedとaccelerationを指定すると
//台形加減速を行う
void control_speed(void)
{
	//速度制御フラグが0ならリターン
	if(!speed_control_en){return;}

	//加速度は常に正にする
	acceleration = my_abs(acceleration);

	//台形加速
	if(now_speed < target_speed){
		now_speed += acceleration;
		if(now_speed > target_speed){
			now_speed = target_speed;
		}
	}
	//台形減速
	else if(now_speed > target_speed){
		now_speed -= acceleration;
		if(now_speed < target_speed){
			now_speed= target_speed;
		}
	}

	now_speed_l = now_speed_r = now_speed;

	short wall_diff;

	switch(run_mode){
	case STRAIGHT://直進
		//壁制御
		wall_diff = get_wall_diff() * wall_gain / 100;
		now_speed_l += wall_diff;
		now_speed_r -= wall_diff;
		break;
	case TURN://その場ターン
		if(turn_dir == 1)		{now_speed_l = -now_speed_l;}
		else if(turn_dir == -1)	{now_speed_r = -now_speed_r;}
		break;
	}

	change_motor_speed(now_speed_l, now_speed_r);
}

//壁制御ゲインをセットする
void set_wall_gain(short gain)
{
	wall_gain = gain;
}

