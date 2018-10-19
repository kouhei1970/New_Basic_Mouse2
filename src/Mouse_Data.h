#ifndef MOUSE_DATA_H_
#define MOUSE_DATA_H_

//lipoセル数
#define BAT_CELL		(3)

//タイヤ直径[mm] 1000倍の値 進み過ぎる時は増やす
#define TIRE_DIAMETER	((26.687) * 1000.0)
//トレッド幅[mm] 1000倍の値 回転しすぎる時は減らす
#define TREAD			((74.622) * 1000.0)
#define MOTOR_STEP_NUM	(400L)

//最低速度[mm/s] 小さくしすぎないこと
#define MIN_SPEED		(50)
//最高速度[mm/s]
#define MAX_SPEED		(2000)
//ターン時の最高速度[mm/s]
#define TURN_SPEED		MAX_SPEED
//ターン時の加速度[m/ss]
#define TURN_ACC		(4)
//左手デモ走行速度
#define LEFT_SPEED		(840)


//迷路1区画の距離[mm]
#define SECTION			(180)
//迷路半区画の距離[mm]
#define H_SECTION		(SECTION/2)

#define GOALX (6)
#define GOALY (10)


#endif /* MOUSE_DATA_H_ */
