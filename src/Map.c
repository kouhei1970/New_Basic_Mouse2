#include "Map.h"
#include "E2Flash_RX220.h"
#include "UI.h"
#include "xprintf.h"

#define MAP_BLOCK	10	//データフラッシュマップ保存用ブロック(10~11)
#define GOAL_BLOCK	12	//データフラッシュゴール座標保存用ブロック

unsigned char map[MAZE_SIZE][MAZE_SIZE];	//壁情報格納配列
char mouse_x;					//マウスX座標
char mouse_y;					//マウスY座標
char mouse_dir;					//マウス方角
char goal_x;					//ゴールX座標
char goal_y;					//ゴールY座標

//壁情報をmap配列にセット
//x		x座標
//y		y座標
//dir	向き 　0:北 1:東 2:南 3:西
//state	壁の有無　 0:壁なし　 1以上:壁あり
void set_map(char x, char y, char dir, char state)
{
	if(x < 0 || y < 0 || x >= MAZE_SIZE || y >= MAZE_SIZE || dir < 0 || dir > 3){
		return;
	}

	if(state){
		switch(dir){
		case 0:map[y][x]|=0x11;	if(y<MAZE_SIZE-1){map[y+1][x]|=0x44;}break;
		case 1:map[y][x]|=0x22;	if(x<MAZE_SIZE-1){map[y][x+1]|=0x88;}break;
		case 2:map[y][x]|=0x44;	if(y>0)			 {map[y-1][x]|=0x11;}break;
		case 3:map[y][x]|=0x88;	if(x>0)			 {map[y][x-1]|=0x22;}break;
		}
	}
	else if(state == 0){
		switch(dir){
		case 0:map[y][x]|=0x10;map[y][x]&=0xfe;	if(y<MAZE_SIZE-1){map[y+1][x]|=0x40;map[y+1][x]&=0xfb;}break;
		case 1:map[y][x]|=0x20;map[y][x]&=0xfd;	if(x<MAZE_SIZE-1){map[y][x+1]|=0x80;map[y][x+1]&=0xf7;}break;
		case 2:map[y][x]|=0x40;map[y][x]&=0xfb;	if(y>0)			 {map[y-1][x]|=0x10;map[y-1][x]&=0xfe;}break;
		case 3:map[y][x]|=0x80;map[y][x]&=0xf7;	if(x>0)			 {map[y][x-1]|=0x20;map[y][x-1]&=0xfd;}break;
		}
	}
}

//壁情報をmap配列から取得
//x			x座標
//y			y座標
//dir		向き 0:北 1:東 2:南 3:西
//return値	-1:エラー 0:壁なし 1:壁あり 2:未探索
char get_map(char x, char y, char dir)
{
	if(x < 0 || y < 0 || x >= MAZE_SIZE || y >= MAZE_SIZE || dir < 0 || dir > 3){
		return -1;
	}

	//未探索
	if(((map[y][x] >> dir)&0x10) == 0){
		return 2;
	}
	else{
		return ((map[y][x] >> dir)&0x01);
	}
}

//map配列をリセット
//迷路外周とスタート区画の壁情報は既知なので同時にセットする
void reset_map(void)
{
	//mouse_x = 0;
	//mouse_y = 0;
	//mouse_dir = 0;

	for(short y = 0; y < MAZE_SIZE; y++){
		for(short x = 0; x < MAZE_SIZE; x++){
			map[y][x] = 0x00;
			if((x == 0)&&(y == 0))	map[y][x]  = 0xfe;
			if((x == 0)&&(y == 1))	map[y][x]  = 0xc8;
			if((x == 1)&&(y == 0))	map[y][x]  = 0xcc;
			if(y == (MAZE_SIZE-1))	map[y][x] |= 0x11;
			if(x == (MAZE_SIZE-1))	map[y][x] |= 0x22;
			if(y == 0)				map[y][x] |= 0x44;
			if(x == 0)				map[y][x] |= 0x88;
		}
	}
}

//向きを変更
//d		正:時計回り 負:反時計回り
void round_dir(char d)
{
	mouse_dir += d;
	mouse_dir &= 0x03;
}

//座標更新
//dir		現在の方角
//return値	-1:エラー 1:正常
char update_mouse_position(char dir)
{
	if(dir < 0 || dir > 3){return -1;}

	const char dx[4] = {0,1,0,-1};
	const char dy[4] = {1,0,-1,0};

	char temp_x = mouse_x + dx[dir];
	char temp_y = mouse_y + dy[dir];

	if(temp_x < 0 || temp_y < 0 || temp_x >= MAZE_SIZE || temp_y >= MAZE_SIZE){
		return -1;
	}
	else{
		mouse_x = temp_x;
		mouse_y = temp_y;
		return 1;
	}
}

//探索済みかどうか
//x			x座標
//y			y座標
//return値	-1:エラー 0:未探索 1:既探索
char check_searched(char x, char y)
{
	if(x < 0 || y < 0 || x >= MAZE_SIZE || y >= MAZE_SIZE){
		return -1;
	}
	return ((map[y][x]&0xf0) == 0xf0);
}

//map情報をデータフラッシュに保存
//迷路サイズが16*16の場合，使用するブロックは2つ
void save_map(void)
{
	char i = 0, b = 0;
	unsigned short save[64];

	for(short y = 0; y < MAZE_SIZE; y++){
		for(short x = 0; x < MAZE_SIZE; x += 2){
			save[i] = ((map[y][x] << 8)&0xff00) | (map[y][x+1]&0x00ff);
			if(++i >= 64){
				fld_program(MAP_BLOCK + b, 64, save);
				b++;
				i = 0;
			}
		}
	}
	if(i != 0){
		fld_program(MAP_BLOCK + b, i, save);
	}
}

//map情報をデータフラッシュからロード
void load_map(void)
{
	char i = 0, b = 0;
	unsigned short* load;

	load = fld_read(MAP_BLOCK);
	for(short y = 0; y < MAZE_SIZE; y++){
		for(short x = 0; x < MAZE_SIZE; x += 2){
			map[y][x]   = (load[i] >> 8)&0xff;
			map[y][x+1] = (load[i])&0xff;
			if(++i >= 64){
				b++;
				i = 0;
				load = fld_read(MAP_BLOCK + b);
			}
		}
	}
}

//ゴール座標を取得してデータフラッシュに保存
void save_goal(void)
{
	unsigned short temp_g[2];

	xprintf("goal_x:");
	goal_x = get_num();
	xprintf("%2d\n", goal_x);

	xprintf("goal_y:");
	goal_y = get_num();
	xprintf("%2d\n", goal_y);

	temp_g[0] = goal_x;
	temp_g[1] = goal_y;

	fld_program(GOAL_BLOCK, 2, temp_g);

	LED_flash(15, 2);
}

//ゴール座標をデータフラッシュからロード
void load_goal(void)
{
	unsigned short *temp_g;

	temp_g = fld_read(GOAL_BLOCK);

	goal_x = temp_g[0];
	goal_y = temp_g[1];
}

//mapを出力
void output_map(void)
{
	for(short y = MAZE_SIZE - 1; y >= 0; y--){
		xprintf("  +");
		for(short x = 0; x < MAZE_SIZE; x++){
			switch(get_map(x, y, 0)){
			case 0:xprintf("   +");break;
			case 1:xprintf("---+");break;
			case 2:xprintf("...+");break;
			}

		}

		xprintf("\n%2d|", y);

		for(short x = 0; x < MAZE_SIZE; x++){
			switch(get_map(x, y, 1)){
			case 0:xprintf("    ");break;
			case 1:xprintf("   |");break;
			case 2:xprintf("   :");break;
			}
		}

		xprintf("\n");
	}

	xprintf("  +");
	for(short x = 0; x < MAZE_SIZE; x++){
		xprintf("---+");
	}
	xprintf("\n  ");
	for(short x = 0; x < MAZE_SIZE; x++){
		xprintf("  %2d", x);
	}
	xprintf("\n\n");
}
