#include "ADC.h"
#include "iodefine.h"
#include "Wait.h"

//AD変換設定
//AD変換がうまくいかない時はサンプリング時間を増やすと改善するかも
void init_ADC(void)
{
	MSTP_S12AD = 0;				//12bitAD変換コンバータスタンバイ解除
	S12AD.ADCSR.BIT.ADST = 0;	//AD変換停止
	S12AD.ADCSR.BIT.ADCS = 0;	//シングルスキャンモード
	S12AD.ADSSTR0 = 50;			//サンプリング時間
	S12AD.ADSSTR1 = 50;			//サンプリング時間
	S12AD.ADSSTR2 = 50;			//サンプリング時間
	S12AD.ADSSTR3 = 50;			//サンプリング時間
	S12AD.ADSSTR4 = 50;			//サンプリング時間
	S12AD.ADSSTR6 = 50;			//サンプリング時間
	S12AD.ADSSTRL = 50;			//サンプリング時間
	wait_ms(1);					//1ms待つ
}

//AD変換
//指定したポートが存在しない時は-1を返す
//an		AD変換するポート（AN00X）
//return値	AD変換結果（0~4095）
short ad_convert(short an)
{
	short ch = (1 << an);
	if(!(ch & 0x3f5f)){return -1;}		//AD変換ポートが存在しないときはreturn
	S12AD.ADANSA.WORD = ch;
	S12AD.ADCSR.BIT.ADST = 1;			//AD変換スタート
	while(S12AD.ADCSR.BIT.ADST == 1);	//AD変換終了まで待つ
	short data = *(&S12AD.ADDR0 + an);
	S12AD.ADCSR.BIT.ADST = 0;			//AD変換停止
	return data;
}
