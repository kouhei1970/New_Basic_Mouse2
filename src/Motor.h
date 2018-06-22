#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_ON		0
#define MOTOR_OFF		1

extern volatile unsigned long motor_step_l;
extern volatile unsigned long motor_step_r;
extern volatile unsigned long motor_step_sum;

void init_motor(void);
void motor_interrpt_l(void);
void motor_interrpt_r(void);
void reset_motor_step(void);
void change_motor_speed(short speed_l, short speed_r);
void turn_off_motor_timer(short time);
void turn_on_motor(void);
void turn_off_motor(void);
char get_motor_en_status(void);





#endif /* MOTOR_H_ */
