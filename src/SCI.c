#include "SCI.h"
#include "iodefine.h"
#include "Clock.h"
#include "Wait.h"

#define	SCI_BITRATE	38400		//ボーレート

//SCI設定
void init_SCI(void)
{
	unsigned short dummy;

	MSTP_SMCI1 = 0;				//SCI1モジュールスタンバイ解除
	SCI1.SCR.BYTE = 0x00;		//送信,受信及び割り込み要求を禁止
	MPC.PWPR.BIT.B0WI = 0;		//PFSWEビットへの書き込みを許可
	MPC.PWPR.BIT.PFSWE = 1;		//PFSレジスタへの書き込みを許可
	MPC.P26PFS.BIT.PSEL = 10;	//P26をTXD1に設定
	MPC.P30PFS.BIT.PSEL = 10;	//P30をRXD1に設定
	MPC.PWPR.BIT.PFSWE = 0;		//PFSレジスタへの書き込みを禁止
	MPC.PWPR.BIT.B0WI = 1;		//PFSWEビットへの書き込みを禁止
	PORT2.PMR.BIT.B6 = 1;		//TXD1を周辺モジュール入出力に設定
	PORT3.PMR.BIT.B0 = 1;		//RXD1を周辺モジュール入出力に設定
	SCI1.SCR.BIT.CKE = 0;		//クロック設定
	SCI1.SMR.BYTE = 0x00;		//b0 					0：
								//b1クロック選択ビット			0：PCLKクロック(n=0)
								//b2 マルチプロセッサモードビット	0：マルチプロセッサ通信機能を禁止
								//b3 ストップビット長選択		0：1ストップビット
								//b4 パリティモードビット			0：偶数パリティで送受信
								//b5 パリティ許可ビット			0：パリティビットを付加
								//b6 キャラクタ長ビット			0：データ長8ビットで送受信
								//b7 コミュニケーションモードビット	0：調歩同期式モードで動作

	//(CLOCK)/(64*SCI_BITRATE*2^(2n-1))-1
	long sci_brr;
	sci_brr = (CLOCK * 1000000 * 10) / (64 * SCI_BITRATE / 2) - 10;
	sci_brr = (sci_brr + 5) / 10;

	SCI1.BRR = (unsigned char)sci_brr;	//ビットレート調整
	IPR(SCI1,TXI1) = 14;		//割り込み優先度設定
	IPR(SCI1,RXI1) = 14;		//割り込み優先度設定
	IR(SCI1,TXI1) = 0;			//割り込み要求フラグクリア
	IR(SCI1,RXI1) = 0;			//割り込み要求フラグクリア
	wait_ms(10);				//ちょっと待つ
	SCI1.SCR.BIT.TIE = 1;		//TXI割り込み許可
	SCI1.SCR.BIT.RIE = 1;		//RXI割り込み許可
	SCI1.SCR.BIT.TE = 1;		//送信開始
	SCI1.SCR.BIT.RE = 1;		//受信開始
	dummy = SCI1.SSR.BYTE;		//ダミーリード
	SCI1.SSR.BYTE = 0xC0;		//フラグクリア
}

//unsigned char型の文字をSCIで送る関数
//data	文字（ASCII）
void print_char_sci(unsigned char data)
{
	while(!IR(SCI1,TXI1));
	SCI1.TDR = data;			//データを転送
	IR(SCI1,TXI1) = 0;
}
