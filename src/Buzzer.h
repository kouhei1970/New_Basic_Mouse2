#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZ_LA0		0
#define BUZ_SI0		1
#define BUZ_DO1		2
#define BUZ_RE1		3
#define BUZ_MI1		4
#define BUZ_FA1		5
#define BUZ_SO1		6
#define BUZ_LA1		7
#define BUZ_SI1		8
#define BUZ_DO2		9
#define BUZ_RE2		10
#define BUZ_MI2		11
#define BUZ_FA2		12
#define BUZ_SO2		13
#define BUZ_LA2		14
#define BUZ_SI2		15
#define BUZ_DO3		16

extern volatile short buzzer_count;
extern const short scale[17];

void init_buzzer(void);
void start_buzzer(short frequency, short time);
void sound_buzzer(short frequency, short time);
void buzzer_interrupt(void);
void stop_buzzer(void);
void level_up_buzzer(void);
void hotel_buzzer(void);
void church_save_buzzer(void);
void coin_buzzer(void);
void pipo_buzzer(void);

#endif /* BUZZER_H_ */
