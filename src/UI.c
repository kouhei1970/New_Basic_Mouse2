#include "UI.h"
#include "iodefine.h"
#include "Wait.h"
#include "Buzzer.h"

#define LED1	PORTB.PODR.BIT.B0
#define LED2	PORTB.PODR.BIT.B1
#define LED3	PORTB.PODR.BIT.B6
#define LED4	PORTC.PODR.BIT.B2
#define LED_ON	1
#define LED_OFF	0

#define SW_DOWN	PORT2.PIDR.BIT.B7
#define SW_UP	PORT3.PIDR.BIT.B1
#define SW_EXEC	PORT3.PIDR.BIT.B2
#define SW_ON	0
#define SW_OFF	1

//UI設定
void init_ui(void)
{
	PORT2.PCR.BIT.B7 = 1;		//P27(DOWNタクトスイッチ)の内部プルアップを有効にする
	PORT3.PCR.BIT.B1 = 1;		//P31(UPタクトスイッチ)の内部プルアップを有効にする
	PORT3.PCR.BIT.B2 = 1;		//P32(EXECタクトスイッチ)の内部プルアップを有効にする

	PORTB.PDR.BIT.B0 = 1;		//PB0(LED1)を出力に設定
	PORTB.PDR.BIT.B1 = 1;		//PB1(LED2)を出力に設定
	PORTB.PDR.BIT.B6 = 1;		//PB6(LED3)を出力に設定
	PORTC.PDR.BIT.B2 = 1;		//PC2(LED4)を出力に設定

	LED_clear();
}

//LEDをすべて消灯
void LED_clear(void)
{
	LED1 = LED_OFF;
	LED2 = LED_OFF;
	LED3 = LED_OFF;
	LED4 = LED_OFF;
}

//指定した番号のLEDを点灯
//led	LED番号
void LED_on(char led)
{
	switch(led){
	case 1:LED1 = LED_ON;break;
	case 2:LED2 = LED_ON;break;
	case 3:LED3 = LED_ON;break;
	case 4:LED4 = LED_ON;break;
	}
}

//指定した番号のLEDを消灯
//led	LED番号
void LED_off(char led)
{
	switch(led){
	case 1:LED1 = LED_OFF;break;
	case 2:LED2 = LED_OFF;break;
	case 3:LED3 = LED_OFF;break;
	case 4:LED4 = LED_OFF;break;
	}
}

//整数を2進数で表示
//n		表示する整数
void LED_num(char n)
{
	n &= 0x0f;

	if((n&0x08) == 0x08)	{LED_on(1);}
	else					{LED_off(1);}
	if((n&0x04) == 0x04)	{LED_on(2);}
	else					{LED_off(2);}
	if((n&0x02) == 0x02)	{LED_on(3);}
	else					{LED_off(3);}
	if((n&0x01) == 0x01)	{LED_on(4);}
	else					{LED_off(4);}
}

//渡された整数を指定回数点滅させる
//n		表示する整数
//x		点滅させる回数
void LED_flash(char n, short x)
{
	for(; x > 0; x--){
		LED_num(n);
		wait_ms(100);
		LED_clear();
		wait_ms(100);
	}
}

//LED4からLED1へ順に光らせる
void LED_stream(void)
{
	LED_clear();

	for(char i = 4; i >= 1; i--){
		LED_on(i);
		if(i % 2 == 0){
			start_buzzer(scale[BUZ_SI1], 50);
		}
		wait_ms(50);
		LED_off(i);
	}
}

//チャタリング防止処理
//タクトスイッチが離されるまで待つ
void wait_sw_off(void)
{
	wait_ms(50);	//50ms待つ
	//全てのスイッチがOFFになるまでループして待つ
	while((get_sw_state(DOWN)) || (get_sw_state(UP)) || (get_sw_state(EXEC)));
	wait_ms(50);	//50ms待つ
}

//タクトスイッチを使って数字を取得
//return値	入力した数字（0~15）
char get_num(void)
{
	char count = 0;

	while(1){
		LED_num(count);

		if(get_sw_state(UP)){
			wait_sw_off();
			count++;
			if(count > 15){
				count = 15;
				start_buzzer(scale[BUZ_DO2], 50);
				wait_ms(100);
				start_buzzer(scale[BUZ_DO2], 50);
			}
			else{
				start_buzzer(scale[BUZ_SO2], 50);
			}
		}

		if(get_sw_state(DOWN)){
			wait_sw_off();
			count--;
			if(count < 0){
				count = 0;
				start_buzzer(scale[BUZ_DO2], 50);
				wait_ms(100);
				start_buzzer(scale[BUZ_DO2], 50);
			}
			else{
				start_buzzer(scale[BUZ_SO1], 50);
			}
		}

		if(get_sw_state(EXEC)){
			wait_sw_off();
			LED_stream();
			return count;
		}
	}
}

//タクトスイッチの状態を返す
//sw		取得するスイッチの名前 DOWN or UP or EXEC
//return値	タクトスイッチの状態 押されていたら1，押されてなかったら0
char get_sw_state(char sw)
{
	switch(sw){
	case DOWN:return (SW_DOWN == SW_ON);
	case   UP:return (SW_UP   == SW_ON);
	case EXEC:return (SW_EXEC == SW_ON);
	}
	return 0;
}

//タクトスイッチ(EXEC)が押されるまで待つ
void wait_sw_on(void)
{
	short i = 0;

	LED_clear();

	while(1){
		if(get_sw_state(EXEC)){
			wait_sw_off();
			LED_clear();
			start_buzzer(scale[BUZ_DO3], 100);
			break;
		}

		i++;
		if(i >= 100){
			i = 0;
			if(LED3 == LED_ON){
				LED_off(3);
			}
			else{
				LED_on(3);
			}
		}

		wait_ms(1);
	}
}
