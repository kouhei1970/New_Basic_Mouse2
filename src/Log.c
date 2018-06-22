#include "Log.h"
#include "My_Func.h"
#include "UI.h"
#include "xprintf.h"
#include "string.h"

#define CHAR_TYPE	1
#define SHORT_TYPE	2
#define LONG_TYPE	3

volatile char	log_en = 0;
volatile char	ring_log_en = 0;
const char*		log_var_name[LOG_NUM];
void*			log_add[LOG_NUM];
long			log[LOG_NUM][LOG_TIME];
char			log_type[LOG_NUM];
short			log_count = 0;

//log関連の変数をリセット
void reset_log(void)
{
	log_count = 0;
	for(short i = 0; i < LOG_NUM; i++){
		log_add[i] = 0;
		log_type[i] = 0;
		log_var_name[i] = 0;
		for(short j = 0; j < LOG_TIME; j++){
			log[i][j] = 0;
		}
	}
}

//set_log_varマクロから呼ばれる関数
//add		セットする変数のアドレス
//name		セットする変数の名前
//type		セットする変数の型
//i			何番目にセットするか
void _set_log_var(void* add, const char* name, char* type, short i)
{
	if(i < 0 || i >=LOG_NUM){return;}
	
	log_add[i] = add;
	log_var_name[i] = name;

	if(!strncmp(type, "char", 4)){
		log_type[i] = CHAR_TYPE;
	}
	else if(!strncmp(type, "short", 5)){
		log_type[i] = SHORT_TYPE;
	}
	else if(!strncmp(type, "long", 4)){
		log_type[i] = LONG_TYPE;
	}
	else{
		xprintf("%s\n", type);		//エラー
		return;
	}
}

//ログ取り開始
void start_log(void)
{
	log_en = 1;
}

//ログ取り開始（リングログ）
void start_ring_log(void)
{
	ring_log_en = 1;
}

//1ms割り込みから呼ばれる関数
void log_interrupt(void)
{
	if(!log_en && !ring_log_en){return;}
	
	if(log_en && ring_log_en){
		log_en = 0;
		ring_log_en = 0;
		return;
	}

	for(short i = 0; i < LOG_NUM; i++){
		switch(log_type[i]){
		case CHAR_TYPE :log[i][log_count] = *(char* )log_add[i];break;
		case SHORT_TYPE:log[i][log_count] = *(short*)log_add[i];break;
		case LONG_TYPE :log[i][log_count] = *(long* )log_add[i];break;
		}
	}

	if(++log_count >= LOG_TIME){
		log_count = 0;
		if(log_en){log_en = 0;}
	}
}

//ログをPC画面に出力する関数
void output_log(void)
{
	if(ring_log_en){ring_log_en = 0;}

	wait_sw_on();
	
	for(short i = 0; i < LOG_NUM; i++){
		if(log_var_name[i]){
			xprintf("%s ", log_var_name[i]);
		}
	}
	xprintf("\n");

	for(short j = 0; j < LOG_TIME; j++){
		xprintf("%4d ", log_count);
		for(short i = 0; i < LOG_NUM; i++){
			xprintf("%ld ", log[i][log_count]);
		}
		xprintf("\n");
		if(++log_count >= LOG_TIME){log_count = 0;}
	}

	reset_log();
	LED_flash(15, 2);
}
