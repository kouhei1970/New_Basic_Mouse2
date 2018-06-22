#ifndef MY_FUNC_H_
#define MY_FUNC_H_

#include "Mouse_Data.h"

#define PI 					(3.14159265)
#define my_abs(x)			((x) >= 0) ? (x) : (-(x))
//距離[mm]をステップ数に変換
#define len_to_step(l)		((unsigned long)(MOTOR_STEP_NUM * 2 * 1000) * (l) / (unsigned long)(PI*TIRE_DIAMETER))
//ステップ数を距離[mm]に変換
#define step_to_len(s)		((unsigned long)(PI*TIRE_DIAMETER) * (s) / (MOTOR_STEP_NUM * 2 * 1000))
//角度[dig]をステップ数に変換
#define angle_to_step(a)	((unsigned long)TREAD * MOTOR_STEP_NUM * 2 / (unsigned long)TIRE_DIAMETER * (a)) / (360)

#endif /* MY_FUNC_H_ */
