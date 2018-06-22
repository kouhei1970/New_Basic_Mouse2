#ifndef WAIT_H_
#define WAIT_H_

extern volatile unsigned short wait_count;

void init_wait(void);
void wait_ms(long ms);
void wait_us(long us);


#endif /* WAIT_H_ */
