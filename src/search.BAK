#include "search.h"
#include "Run.h"
#include "Map.h"
#include "Mouse_Data.h"
#include "Wall_Sensor.h"
#include "iodefine.h"
#include "Clock.h"
#include "UI.h"
#include "Wait.h"
#include "SCI.h"
#include "xprintf.h"
#include "ADC.h"
#include "Interrupt.h"
#include "Motor.h"
#include "Wall_Sensor.h"
#include "Buzzer.h"
#include "Battery.h"
#include "My_Func.h"
#include "E2Flash_RX220.h"
#include "Map.h"
#include "Control.h"
#include "Run.h"
#include "Log.h"


unsigned char h_map[16][16];

/* ======================================================================= */
/* 等高線作成モジュール                                                    */
/* ======================================================================= */
void hight_map(unsigned char gx, unsigned char gy, unsigned char h,unsigned char mode)
{
    // (gy,gx)ゴールにしたい座標
    // h使っていない
    // mode　未探索区画の歩数を数えるか数えないかT_MODE：未探索区画数えない（最短走行時の計算に使用）

	unsigned char	pt0,pt1,ct;
	short	x,y;
	unsigned char	wdata;

    //等高線マップを最候歩数255にリセット
	for (y=0;y<16;y++){
		for (x=0;x<16;x++){
			h_map[y][x] = 255;
		}
	}
	h_map[gy][gx] = 0;//ゴールの場所の歩数を0歩に

	pt0 = 0;

	do{
		ct = 0;
		pt1 = pt0+1;

		for (y=0;y<16;y++){
			for (x=0;x<16;x++){
				if (h_map[y][x]==pt0){
					wdata=map[y][x];
					if (mode==T_MODE){  //未探索区画には進まないモード
						if (((wdata & 0x11) == 0x10)&&(y!=15)){
							if (h_map[y+1][x] == 255){h_map[y+1][x] = pt1;ct++;}}
						if (((wdata & 0x22) == 0x20)&&(x!=15)){
							if (h_map[y][x+1] == 255){h_map[y][x+1] = pt1;ct++;}}
						if (((wdata & 0x44) == 0x40)&&(y!=0)){
							if (h_map[y-1][x] == 255){h_map[y-1][x] = pt1;ct++;}}
						if (((wdata & 0x88) == 0x80)&&(x!=0)){
							if (h_map[y][x-1] == 255){h_map[y][x-1] = pt1;ct++;}}
					}else{              //未探索区画に進むモード
						if (((wdata & 0x01) == 0x00)&&(y!=15)){
							if (h_map[y+1][x] == 255){h_map[y+1][x] = pt1;ct++;}}
						if (((wdata & 0x02) == 0x00)&&(x!=15)){
							if (h_map[y][x+1] == 255){h_map[y][x+1] = pt1;ct++;}}
						if (((wdata & 0x04) == 0x00)&&(y!=0)){
							if (h_map[y-1][x] == 255){h_map[y-1][x] = pt1;ct++;}}
						if (((wdata & 0x08) == 0x00)&&(x!=0)){
							if (h_map[y][x-1] == 255){h_map[y][x-1] = pt1;ct++;}}
					}
				}
			}
		}
		pt0 = pt0+1;

	}while(ct!=0);
}



void search2018(unsigned char gx,unsigned char gy){

    unsigned char first_f = 1;      //スタートする前か？初期値 １
    unsigned char goal_f  = 0;      //ゴールしたかしてないか？初期値 0
    unsigned char search_end_f = 0; //探索終了か？初期値 0
    short mx,my;
    //short gx = 7;
    //short gy = 7;
    unsigned char dir = NORTH;
    short speed = SEARCH_SPEED;
    short acc = SEARCH_ACC;

    while(1){
        if (first_f == 1){//スタート処理

            //座標等の初期化
            mx=0;
            my=1;
            wait_sw_on();
	        wait_ms(1000);

            //自動的にコースの真ん中にセットしセンサーキャリブレーション
	        turn(RIGHT90);                  //右向き右90度
            straight(70, -100, 0, 2, 0);	//後退ケツタッチ
	        straight(34, 500, 0, 2, 0);     //真ん中に前進
	        turn(LEFT90);                   //前向きなおり左90度
	        straight(70, -100, 0, 2, 0);	//後退ケツタッチ
	        wait_ms(500);
	        update_center_ref();            //センサーキャリブレーション
	        set_wall_gain(5);               //制御ゲイン設定

	        //マウス出発
            straight(H_SECTION + 34, speed, speed, acc, 1);  //124mm前進 スタート
            first_f = 0;//フラグリセット（スタート終わり）

        }

        else if (gx==mx & gy==my){//ゴールに入った時の処理

        }

        else if (first_f==0 && mx==0 && my==0){//スタートに戻った時の処理

        }

        else {//普通の探索
            update_map(mx, my, dir);//壁情報を更新
            unsigned char action = decide_action(mx, my, gx, gy, dir);//直進か左か右か行動を決める
            move(action);//決めた行動を実行
            update_state(action, &mx ,&my, &dir);//行動に基づいて座標と方向を更新
        }
    }

}

void update_map(unsigned char x, unsigned char y, unsigned char dir){

    if ( dir == NORTH){//北向きだったら
        set_map(x, y, NORTH, wall_lf | wall_rf );//前壁はOR取り
        set_map(x, y, WEST , wall_ls );
        set_map(x, y, EAST , wall_rs);
    }

    else if ( dir == EAST){//東向きだったら
        set_map(x, y, EAST ,wall_lf|wall_rf );
        set_map(x, y, NORTH, wall_ls );
        set_map(x, y, SOUTH, wall_rs);

    }

    else if ( dir == SOUTH){//南向きだったら
        set_map(x, y, SOUTH, wall_lf|wall_rf );
        set_map(x, y, EAST , wall_ls );
        set_map(x, y, WEST , wall_rs);
    }

    else if ( dir == WEST){//西向きだったら
        set_map(x, y, WEST , wall_lf|wall_rf );
        set_map(x, y, SOUTH, wall_ls );
        set_map(x, y, NORTH, wall_rs);

    }
    return;
}

short decide_action(short mx, short my, short gx, short gy, short dir){

    short front=255, left=255, right=255;

    hight_map(gx, gy, 0, 0);//等高線マップ作る

    if ( dir == NORTH){//北向きだったら
        if (my<15 && get_map(mx,my,NORTH)==0) front = h_map[my+1][mx];
        if (mx>0  && get_map(mx,my,WEST)==0) left  = h_map[my][mx-1];
        if (mx<15 && get_map(mx,my,EAST)==0) right = h_map[my][mx+1];

        if (front==255&&left==255&&right==255) return PIVOTTURN;
        if (front<=left && front<=right)       return STRAIGHTRUN;
        if (left<=front && left<=right)        return LEFTTURN;
        if (right<=front && right<=left)       return RIGHTTURN;
    }

    else if ( dir == EAST){//東向きだったら
        if (mx<15 && get_map(mx,my,EAST)==0) front = h_map[my][mx+1];
        if (my<15 && get_map(mx,my,NORTH)==0) left  = h_map[my+1][mx];
        if (my>0  && get_map(mx,my,SOUTH)==0) right = h_map[my-1][mx];

        if (front==255&&left==255&&right==255) return PIVOTTURN;
        if (front<=left && front<=right)       return STRAIGHTRUN;
        if (left<=front && left<=right)        return LEFTTURN;
        if (right<=front && right<=left)       return RIGHTTURN;
    }

    else if ( dir == SOUTH){//南向きだったら
        if (my>0  && get_map(mx,my,SOUTH)==0) front = h_map[my-1][mx];
        if (mx<15 && get_map(mx,my,EAST)==0) left  = h_map[my][mx+1];
        if (mx>0  && get_map(mx,my,WEST)==0) right = h_map[my][mx-1];

        if (front==255&&left==255&&right==255) return PIVOTTURN;
        if (front<=left && front<=right)       return STRAIGHTRUN;
        if (left<=front && left<=right)        return LEFTTURN;
        if (right<=front && right<=left)       return RIGHTTURN;

    }

    else if ( dir == WEST){//西向きだったら
        if (mx>0  && get_map(mx,my,WEST)==0) front = h_map[my][mx-1];
        if (my>0  && get_map(mx,my,SOUTH)==0) left  = h_map[my-1][mx];
        if (my<15 && get_map(mx,my,NORTH)==0) right = h_map[my][mx+1];

        if (front==255&&left==255&&right==255) return PIVOTTURN;
        if (front<=left && front<=right)       return STRAIGHTRUN;
        if (left<=front && left<=right)        return LEFTTURN;
        if (right<=front && right<=left)       return RIGHTTURN;

    }

    return;
}


void move(unsigned char action){

    //物理パラメータ設定
    short acc=4;
	short speed = SEARCH_SPEED;
	short tspeed= SEARCH_SPEED;
	short pre=PRE_LENGTH;//40;// defo 47
	short slalen=SLA_LENGTH;//85;// defo 73


    if (action == STRAIGHT) {
        //直進180mm
        straight(SECTION, speed, speed, acc, 1);
    }

    else if (action == LEFTTURN) {
        //Left Turn
        straight(pre, speed, tspeed, acc, 1);
		slalom(slalen, acc, 1, 0);
		straight(pre, speed, speed, acc, 1);
    }

    else if (action == RIGHTTURN) {
        //Right Turn
		straight(pre, speed, tspeed, acc, 1);
		slalom(slalen, acc, -1, 0);
		straight(pre, speed, speed, acc, 1);
    }

    else if (action == PIVOTTURN) {
        //Pivot Turn
		straight(H_SECTION, speed, 0, acc, 1);
		turn(180);
		straight(H_SECTION, speed, speed, acc, 1);
    }
}

void update_state(unsigned char act, unsigned char *x, unsigned char *y, unsigned char *dir){

	if (act==STRAIGHT){
		if (*dir==NORTH){
			(*y)++;
		}
		else if (*dir == EAST){
			(*x)++;
		}
		else if (*dir == SOUTH ){
			(*y)--;
		}
		else if (*dir == WEST){
			(*x)--;
		}
	}

	else if (act==LEFTTURN){
		if (*dir==NORTH){
			(*x)--;
		}
		else if (*dir == EAST){
			(*y)++;
		}
		else if (*dir == SOUTH ){
			(*x)++;
		}
		else if (*dir == WEST){
			(*y)--;
		}
		(*dir)--;
		*dir=(*dir) & 0x03;
	}

	else if (act==RIGHTTURN){
		if (*dir==NORTH){
			(*x)++;
		}
		else if (*dir == EAST){
			(*y)--;
		}
		else if (*dir == SOUTH ){
			(*x)--;
		}
		else if (*dir == WEST){
			(*y)++;
		}

		(*dir)++;
		*dir=(*dir) & 0x03;

	}

}


short make_sim_map(void){
    char smap[33][66]={
        "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+",
        "|   |                                                           |",//15
        "+   +   +   +---+---+---+---+---+---+---+---+---+---+   +   +---+",
        "|       |   |               |               |       |   |       |",//14
        "+   +---+   +   +---+---+   +   +---+---+   +   +   +---+---+   +",
        "|           |   |       |       |       |       |           |   |",//13
        "+   +---+   +   +   +   +---+---+   +   +---+---+---+---+   +   +",
        "|           |       |               |           |   |       |   |",//12
        "+   +---+---+---+---+---+---+---+---+---+---+---+---+   +---+   +",
        "|   |       |   |   |   |   |   |   |   |   |   |   |       |   |",//11
        "+   +   +---+---+---+---+---+---+---+---+---+---+---+---+   +   +",
        "|   |   |   |   |   |   |   |   |   |   |   |   |           |   |",//10
        "+   +   +---+---+---+---+---+---+---+---+---+---+---+   +---+   +",
        "|   |       |   |   |   |   |   |   |   |   |   |           |   |",// 9
        "+   +---+---+---+---+---+---+---+---+---+---+---+---+   +---+   +",
        "|   |       |   |   |   |   |       |   |   |   |           |   |",// 8
        "+   +   +---+---+---+---+---+   +   +---+---+---+---+   +---+   +",
        "|   |   |   |   |   |   |   |       |   |                   |   |",// 7
        "+   +   +---+---+---+---+---+---+---+---+---+   +---+---+---+   +",
        "|   |       |   |   |   |   |   |   |   |   |               |   |",// 6
        "+   +---+---+---+---+---+---+---+---+---+---+---+---+---+   +   +",
        "|   |       |   |   |   |   |   |   |   |   |   |   |   |   |   |",// 5
        "+   +   +---+---+---+---+---+---+---+---+---+---+---+---+---+   +",
        "|   |       |   |   |   |   |   |   |   |   |   |   |   |   |   |",// 4
        "+   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+   +",
        "|   |       |   |   |   |   |   |   |   |   |   |   |   |   |   |",// 3
        "+   +   +---+---+---+---+---+---+---+---+---+---+---+---+---+   +",
        "|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |",// 2
        "+   +   +---+---+---+---+---+---+---+---+---+---+---+---+---+   +",
        "|       |   |   |   |   |   |   |   |   |   |   |   |   |   |   |",// 1
        "+   +---+   +   +   +---+---+---+---+---+---+---+---+---+---+   +",
        "|   |                                                           |",// 0
        "+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+"
        };
        // 0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15

}

#if 0
//-------------------------------------------------------------------------
//	探索関数
//-------------------------------------------------------------------------
void mouse_search( uchar y, uchar x, int spd )
{
  //探索が終わる条件
  //1. ゴールした時に４分以上経過していた場合
  //2. 一度ゴールしていてスタートに戻った時
  //元プログラムは、選択できましたが原則スラ探にします。
  
  uchar wall_data,vec,i,r_flag,l_flag,s_flag,tmp;

  recovery();   //けつかっちん
  mode6(1);     //Sensor Check
  run_flag=1;
  control_mode=1;
  goal_flag = 0;
  sura_flag=0;

  while( 1 ){
    // １つのループは区間中心から次の区間中心まで
    // 最初に半区画直進

    rdir = 0; ldir = 0;           // 回転方向を直進
    STEP = 0;                     // 距離カウンタリセット
    speed = spd;                  // 速度設定(これで自動的にマウスが移動開始)
    control_mode = 1;             // 姿勢制御ON

    if(sura_flag!=1){
      while( STEP < GO_STEP / 2 )
      if(SW_EXEC==SW_ON)return;
    }  // 半区間進む
    else STEP = GO_STEP/2;//スラロームフラグは

    place_update();               // 現在座標更新
    // 柱まで進んだら
    wall_data = get_wall_data();  // 壁情報取得

    if(map[my][mx]<0xf0)          // マッピングしていない区画なら
      map_save(wall_data);        // マップを更新
    
    LED_wall(wall_data);
    wall_data|=symu_wall();

    sura_flag=0;

    if(mx==x&&my==y&&goal_flag==0)                          //初めてゴールした
    {
      while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
      speed=1;
      while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;
      goal_flag=1;
      com_turn(2);
      countdown();
      if(over_flag==1){
        com_stop();
        hight_map(0,0,0,T_MODE);
        most_short(y,x,spd);
        control_mode=1;
        return;
      }
    }
    else if(mx==0&&my==0)                                   //スタートにいる
    {
      if(goal_flag==1)
      {
        while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
        speed = 1;
        while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;
        com_turn( 2 );
        goal_flag=0;
        run_flag=0;
        countdown();
        return;
      }
      else
      {
        while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
        speed = 1;
        while(STEP<GO_STEP);
        com_turn(2);
        map_initialize();
        countdown();
      }
    }
    else if(over_flag==1 && goal_flag ==1){                 //一度はゴールし時間オーバー
      while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
      speed =1;
      while(STEP<GO_STEP);
      com_stop();
      control_mode=1;
      hight_map(0,0,0,T_MODE);
      wall_data=map_load();
      if(wall_data!=3)com_turn(wall_data);
      most_short(0,0,spd);
      countdown();
      return;
    }
    
    else if(spd>=200){                                      //スピードが200以上　スラローム探索
      r_flag=l_flag=s_flag=0;
      if((wall_data&0x08)==0x00)     // 左に壁がなかったら
        l_flag=1;//com_sura(1);
      if((wall_data&0x01)==0x00) // 前に壁がなかったら
        s_flag=1;//while( STEP < GO_STEP );      // さらに半区間進む
      if((wall_data&0x02)==0x00)// 右に壁がなかったら
        r_flag=1;//com_sura(0);

      if(l_flag==0&&r_flag==0&&s_flag==0){
        vec=vector();
        if(vec==2)
        {
          while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
          speed = 1;                    // 残り半区間を減速しながら直進
          while( STEP < GO_STEP );      // さらに半区間進む
          com_turn( vec );
        }
        else if(vec<2)
          com_sura(vec); 
        else
          while(STEP<GO_STEP);
      }
      
      else if(r_flag&&l_flag&&s_flag){
        tmp=wait_timer%3;
        if(tmp==0)com_sura(0);
        else if(tmp==1)com_sura(1);
        else {while( STEP < GO_STEP );}
      }
      else if(l_flag&&s_flag){
        tmp=wait_timer%2;
        if(tmp==0)com_sura(1);
        else {while( STEP < GO_STEP );}
      }
      else if(r_flag&&s_flag){
        tmp=wait_timer%2;
        if(tmp==0)com_sura(0);
        else {while( STEP < GO_STEP );}
      }
      else if(r_flag&&l_flag){
        tmp=wait_timer%2;
        if(tmp==0)com_sura(0);
        else if(tmp==1)com_sura(1);
      }
      else if(l_flag){
        com_sura(1);
      }
      else if(r_flag){
        com_sura(0);
      }
      else {while( STEP < GO_STEP );}
      
      
    }
    
/*
    else if(spd>=200){
      if((wall_data&0x08)==0x00)     // 左に壁がなかったら
        com_sura(1);
      else if((wall_data&0x01)==0x00) // 前に壁がなかったら
        while( STEP < GO_STEP );      // さらに半区間進む
      else if((wall_data&0x02)==0x00)// 右に壁がなかったら
        com_sura(0);
      else{
        vec=vector();
        if(vec==2)
        {
          while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
          speed = 1;                    // 残り半区間を減速しながら直進
          while( STEP < GO_STEP );      // さらに半区間進む
          com_turn( vec );
        }
        else if(vec<2)
          com_sura(vec); 
        else
          while(STEP<GO_STEP);
      }
    }
*/
    
    else{                                                                   //極地旋回モードで探索
      if((wall_data&0x08)==0x00){     // 左に壁がなかったら
        while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
        speed = 1;                    // 残り半区間を減速しながら直進
        while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;      // さらに半区間進む
        com_turn( 1 );                // 左90度旋回
      }
      else if((wall_data&0x01)==0x00) // 前に壁がなかったら
        while( STEP < GO_STEP );      // さらに半区間進む
      else if((wall_data&0x02)==0x00){// 右に壁がなかったら
        while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
        speed = 1;                    // 残り半区間を減速しながら直進
        while( STEP < GO_STEP );      // さらに半区間進む
        com_turn( 0 );                // 右90度旋回
      }
      else{
        vec=vector();
        if(vec<=2)
        {
          while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
          speed = 1;                    // 残り半区間を減速しながら直進
          while( STEP < GO_STEP );      // さらに半区間進む
          com_turn( vec );
        }
        else
          while(STEP<GO_STEP);
      }
    }
  }
}


//------------------------------------------------------------------------
//最短
//------------------------------------------------------------------------
void most_short(char y,char x,int spd)
{
	uchar course;
	LED_num(spd/50);
	pause(500);
	control_mode=1;
	if(over_flag!=1){
	  hight_map_initalize();
	  hight_map(y,x,0,T_MODE);
	}
	else
	  over_flag=0;
	 
	 while( 1 ){
    // １つのループは区間中心から次の区間中心まで
    // 最初に半区画直進
    rdir = 0; ldir = 0;           // 回転方向を直進
    STEP = 0;                     // 距離カウンタリセット
    speed = spd;                  // 速度設定
    control_mode = 1;             // 姿勢制御ON
    
    if(sura_flag!=1){while( STEP < GO_STEP / 2 );}  // 半区間進む
    else {STEP = GO_STEP/2;}
    
  	sura_flag=0;
    place_update();               // 現在座標更新
    LED_num(h_map[my][mx]);

    if(my==0&&mx==0&&goal_flag==1)
    {
      while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
    	speed = 1;
		  while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;
		  com_turn(2);
    	com_stop();
    	countdown();
    	goal_flag=0;
    	hight_map_initalize();
    	hight_map(x,y,0,T_MODE);
    	control_mode=1;
    	return;
    }
    else if(my==y&&mx==x)
    {
    	while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
    	speed = 1;
      while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;
      com_turn(2);
    	com_stop();
    	hight_map_initalize();
    	hight_map(0,0,0,T_MODE);
    	countdown();
    	goal_flag=1;
    	control_mode=1;
    }
    else{
	  	course=map_load();
	  	if(course==2)
	  	{
	  		while( STEP < GO_STEP - ( speed_now - 1 ) * 2 );
	  		speed = 1;                    // 残り半区間を減速しながら直進
      	while( STEP < GO_STEP )if(SW_EXEC==SW_ON)return;      // さらに半区間進む
      	com_turn( 2 );                // 右90度旋回
	  	}
	  	else if(course<2)
	  	  com_sura(course);
	  	else
	  		while(STEP<GO_STEP)if(SW_EXEC==SW_ON)return;
	  }
	  
  }
  
}

//------------------------------------------------------------------------
//矢印をたどる方向
//------------------------------------------------------------------------
unsigned char vector(void)
{
	uchar vec_tmp;
	
	vec_tmp=(back_map[my][mx]&0x0f);
	
	if(vec_tmp>head)
		switch(vec_tmp/head)
		{
			case 1:
				return 3;
				break;
			case 2:
				return 0;
				break;
			case 4:
				return 2;
				break;
			case 8:
				return 1;
				break;
		}
	else
		switch(head/vec_tmp)
		{
			case 1:
				return 3;
				break;
			case 2:
				return 1;
				break;
			case 4:
				return 2;
				break;
			case 8:
				return 0;
				break;
		}
  return 3;
}
#endif
