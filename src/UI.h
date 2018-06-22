#ifndef UI_H_
#define UI_H_

#define DOWN	0
#define UP		1
#define EXEC	2

void init_ui(void);
void LED_clear(void);
void LED_on(char led);
void LED_off(char led);
void LED_num(char n);
void LED_flash(char n, short x);
void LED_stream(void);
void wait_sw_off(void);
char get_num(void);
char get_sw_state(char sw);
void wait_sw_on(void);


#endif /* UI_H_ */
