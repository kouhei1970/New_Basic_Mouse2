#include "Wait.h"
#include "iodefine.h"
#include "Clock.h"

//wait設定
void init_wait(void)
{
	MSTP_CMT0 = 0; 				//スタンバイ解除
	CMT.CMSTR0.BIT.STR0 = 0;	//タイマ停止
	CMT0.CMCR.BIT.CKS = 1;		//動作周波数設定:CLOCK／32

	MSTP_CMT1 = 0;				//スタンバイ解除
	CMT.CMSTR0.BIT.STR1 = 0;	//タイマ停止
	CMT1.CMCR.BIT.CKS = 0;		//動作周波数設定:CLOCK／8
}

//wait関数
//ms	待つ時間[ms]
void wait_ms(long ms)
{
	unsigned volatile long count;
	const short clk_num = CLOCK*1000/32;//1msにかかるクロック数
	const short one_time = 0xffff / clk_num;

	while(ms > 0){
		if(ms > one_time){
			count = one_time;
		}
		else{
			count = ms;
		}
		ms -= one_time;
		count = count * clk_num - 1;
		CMT0.CMCNT = 0;
		CMT.CMSTR0.BIT.STR0 = 1;
		while(CMT0.CMCNT < count);
		CMT.CMSTR0.BIT.STR0 = 0;
	}
}

//wait関数
//us	待つ時間[μs]
void wait_us(long us)
{
	unsigned volatile long count;
	const short clk_num = CLOCK*10/8;//10usにかかるクロック数
	const short one_time = 0xffff / (clk_num) * 10;

	while(us > 0){
		if(us > one_time){
			count = one_time;
		}
		else{
			count = us;
		}
		us -= one_time;
		count = count * clk_num / 10 - 1;
		CMT1.CMCNT = 0;
		CMT.CMSTR0.BIT.STR1 = 1;
		while(CMT1.CMCNT < count);
		CMT.CMSTR0.BIT.STR1 = 0;
	}
}
