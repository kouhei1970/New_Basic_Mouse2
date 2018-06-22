#include "Clock.h"
#include "iodefine.h"

//クロック設定
//32MHzはマイコン内蔵のオシレータ
//20MHzは秋月のボードに載ってるセラロック
void init_clock(void)
{
	SYSTEM.PRCR.WORD = 0xa50b;			//レジスタ書き込み許可

	if(CLOCK == 32){
		SYSTEM.SCKCR3.BIT.CKSEL = 1;	//クロックソースをHOCO(32MHz)に設定
		SYSTEM.HOCOCR.BIT.HCSTP = 0;	//HOCO動作
	}
	else if(CLOCK == 20){
		SYSTEM.SCKCR3.BIT.CKSEL = 2;	//クロックソースをメインクロック(20MHz)に設定
		SYSTEM.MOSCCR.BYTE = 0;			//メインクロック発振器動作
	}

	for(volatile short i = 0; i < 300; i++);//クロックが安定するまで待つ
}
