#ifndef __LOG_HEADER__
#define __LOG_HEADER__

//ログを取得する変数をセットするマクロ
//var		変数名
//type		変数の型 char or short or long
//i			何番目にセットするか
#define set_log_var(var, type, i)	_set_log_var(&var, #var, #type, i)

//ログ用の配列のサイズを決める定数
//3*1000*4=12kByteなのでこれ以上増やすとRAMが足りなくなるかも
#define LOG_NUM			3
#define LOG_TIME		1000

extern volatile char	log_en;
extern volatile char	ring_log_en;
extern void*			log_add[LOG_NUM];
extern long				log[LOG_NUM][LOG_TIME];
extern char				log_type[LOG_NUM];
extern short			log_count;

void reset_log(void);
void _set_log_var(void* add, const char* name, char* type, short i);
void start_log(void);
void start_ring_log(void);
void log_interrupt(void);
void output_log(void);

#endif
