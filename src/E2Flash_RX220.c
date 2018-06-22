#include "iodefine.h"
#include "Clock.h"
#include "Wait.h"

//書き込み時に削除されるのを防止する
//#pragma address ID_CODE = 0xFFFFFFA0
//const unsigned long ID_CODE[4] = {0x52000000, 0x00000000, 0x00000000, 0x00000000}; //idコードを0x00...0x00に設定
//e2studioの場合はvector_table.cを書き換えること

//エラーマクロ
#define FLD_OK			0
#define FLD_ILGL		1
#define FLD_ERS			2
#define FLD_PRG			4
#define FLD_TMOUT		8

#define _FCLK_			CLOCK		//FlashIFクロック(MHz)

#define _DFTA_			(0x0100000)	//データフラッシュの先頭

//RX220ではブロック単位は128Byteなので消去も128Byte
//書込は2Byteまたは8Byte単位
//ブロック番号は0～63

//伊藤先輩のRX631用をRX220用に書き直しました


/*---------------------------プロトタイプ宣言--------------------------*/
// 初期化関数
void init_Data_Flash(void);

//データフラッシュから配列を使って読み出す
unsigned short * fld_read(unsigned short block);

//データフラッシュに書き込む 引数はブロック番号(0~63)と配列長(1~64),配列の先頭
//消すのめんどいので消去関数と統合　返り値は無視してもいい
unsigned long fld_program(unsigned short block,short len,unsigned short *ram);



/*-------------使うのはこのファイル内だけであろう関数の宣言------------*/
//クロック通知
void fld_init_pclk_notification(void);
//コマンド完了まで待つ関数
int wait_FRDY(int t);
//エラーチェック
unsigned long fld_error_check(unsigned short block);
//読み出し許可関数
void fld_enable_read(void);
//FCUリセット
void reset_fcu(void);



//データフラッシュ初期化
void init_Data_Flash(void)
{
	//FCUリセット
	reset_fcu();
	//クロック通知
	fld_init_pclk_notification();
	//読み出し許可関数
	fld_enable_read();
}

//周辺クロックをFCUに通知する関数
void fld_init_pclk_notification(void)
{
	short i;
	//00100000hはDB00ブロックのトップアドレス
	volatile unsigned char *addr_b = (unsigned char *)_DFTA_;
	volatile unsigned short *addr_w = (unsigned short *)_DFTA_;
	
	
	//P/Eモードにチェンジ
	if((FLASH.FENTRYR.WORD & 0x00ff) != 0x0080){
		FLASH.FENTRYR.WORD = 0xAA80;	//キーコードはAAh
	}
	
	
	//消去プロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	
	fld_error_check(0);
	
	//周辺クロックを設定
	FLASH.PCKAR.BIT.PCKA = _FCLK_;
	
	for(i=0;i<64;i++){//ブロック数の分だけループ
		//周辺クロック通知コマンド発行
		*addr_b = 0xE9;
		*addr_b = 0x03;
		*addr_w = 0x0F0F;
		*addr_w = 0x0F0F;
		*addr_w = 0x0F0F;
		*addr_b = 0xD0;
	
		//コマンド完了を待つ
		if(wait_FRDY(120)==FLD_TMOUT){
			reset_fcu();	
		}
		fld_error_check(i);
		addr_b += 0x80;//0x80：1BLOCKのサイズ
		addr_w += 0x80;//0x80：1BLOCKのサイズ
	}
	//プロテクト
	FLASH.FWEPROR.BIT.FLWE = 0;
	
}


//書き込み関数
unsigned long fld_program(unsigned short block,short len,unsigned short *ram)
{
	short i;
	volatile unsigned char *addr_b;
	volatile unsigned short *addr_w;
	
	unsigned long ret = FLD_OK;
	
	addr_b = (volatile unsigned char *)(_DFTA_+block*0x80);	//0x80：1BLOCKのサイズ
	addr_w = (volatile unsigned short *)(_DFTA_+block*0x80);//0x80：1BLOCKのサイズ
	
	
	//P/Eモードにチェンジ
	if((FLASH.FENTRYR.WORD & 0x00ff) != 0x0080){
		FLASH.FENTRYR.WORD = 0xAA80;	//キーコードはAAh
	}
	
	//書き込みプロテクト解除
	FLASH.FWEPROR.BIT.FLWE = 1;
	
	//ブロック単位の書き込み許可
	FLASH.DFLWE0.WORD = 0x1EFF;		//キーコードは1Eh
	
	
	//ブロック消去コマンド発行
	*addr_b = 0x20;
	*addr_b = 0xD0;
	
	
	//コマンド完了を待つ
	if(wait_FRDY(250*1000*1.1)==FLD_TMOUT){
		reset_fcu();
		ret |= FLD_TMOUT;	
	}
	//エラーチェック
	ret |= fld_error_check(block);
	ret = (ret << 8)&0xFF00;
	//プログラムコマンド発行
	for(i=0;i<len;i++){				//データ数分ループ
		*addr_b = 0xE8;
		*addr_b = 0x01;
		*addr_w = *ram++;
		*addr_b = 0xD0;
		//コマンド完了を待つ
		if(wait_FRDY(2*1000*1.1)==FLD_TMOUT){
			reset_fcu();
			ret |= FLD_TMOUT;
		}
		//エラーチェック
		ret |= fld_error_check(block);
		addr_w++;
		addr_b++;
		addr_b++;
	}
	
	
	//プロテクトと消去無効
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	
	return ret;
}


//データフラッシュから配列を使って読み出す
unsigned short * fld_read(unsigned short block){
	//読み出し許可
	fld_enable_read();
	return (unsigned short *)(_DFTA_ + block*0x80);//0x80：1BLOCKのサイズ
}

//読み出し許可関数
void fld_enable_read(void)
{
	
	//データフラッシュ読み出し許可
	FLASH.DFLRE0.WORD = 0x2DFF;		//キーコードは2Dh
	
	//リードモードへ
	do{
		FLASH.FENTRYR.WORD = 0xAA00;	//キーコードはAAh
	}
	while(FLASH.FENTRYR.WORD & 0x00ff);
	FLASH.FWEPROR.BIT.FLWE = 2;
	
	
}
//FCUリセット関数
void reset_fcu(void)
{
	volatile unsigned long w;
	
	//ソフトウェアリセット
	FLASH.FRESETR.BIT.FRESET =1;
	
	//35us待つ
	wait_us(35);
	
	
	//ソフトウェアリセット解除
	FLASH.FRESETR.BIT.FRESET =0;
}
//エラーチェック
unsigned long fld_error_check(unsigned short block){
	volatile unsigned char *addr_b;
	unsigned long ret = FLD_OK;
	addr_b = (unsigned char *)_DFTA_+block*0x80;//0x80：1BLOCKのサイズ
	
	if((FLASH.FSTATR0.BIT.ILGLERR)|(FLASH.FSTATR0.BIT.ERSERR)|(FLASH.FSTATR0.BIT.PRGERR)){
		if(FLASH.FSTATR0.BIT.ILGLERR)ret |= FLD_ILGL;
		if(FLASH.FSTATR0.BIT.ERSERR)ret |= FLD_ERS;
		if(FLASH.FSTATR0.BIT.PRGERR)ret |= FLD_PRG;
		if(FLASH.FSTATR0.BIT.ILGLERR){
			if(FLASH.FASTAT.BYTE!=0x10){
				FLASH.FASTAT.BYTE=0x10;
			}
			//ステータスクリアコマンド発行
			*addr_b = 0x50;
		}
	}
	return ret;
}
//コマンド完了まで待つ関数
int wait_FRDY(int t)
{
	int cn=0;
	while(FLASH.FSTATR0.BIT.FRDY==0)
	{
		wait_us(1);//1us　wait
		cn++;
		if(cn==t)return FLD_TMOUT;
	}
	return FLD_OK;
}

