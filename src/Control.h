#ifndef CONTROL_H_
#define CONTROL_H_

#define STRAIGHT	1
#define TURN		2

extern char		speed_control_en;
extern short	target_speed;
extern short	acceleration;
extern short	now_speed;
extern short	now_speed_l;
extern short	now_speed_r;
extern char		run_mode;
extern char		turn_dir;
extern short	wall_gain;

void control_speed(void);
void set_wall_gain(short gain);

#endif /* CONTROL_H_ */
