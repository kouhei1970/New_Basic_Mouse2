/*
 * search.h
 *
 *  Created on: 2018/07/14
 *      Author: kouhei
 */

#ifndef SEARCH_H_
#define SEARCH_H_

#define NORTH (0)
#define EAST (1)
#define SOUTH (2)
#define WEST (3)

#define T_MODE (1)
#define S_MODE (0)

#define LEFT90 (90)
#define RIGHT90 (-90)

#define SEARCH_SPEED (350)
#define SEARCH_ACC (4)
#define PRE_LENGTH (40)
#define SLA_LENGTH (85)

#define STRAIGHTRUN (0)
#define LEFTTURN (1)
#define RIGHTTURN (2)
#define PIVOTTURN (3)

extern unsigned char h_map[16][16];

void hight_map(unsigned char gx, unsigned char gy, unsigned char h,unsigned char mode);
void update_map(unsigned char x, unsigned char y, unsigned char dir);
short decide_action(short mx, short my, short gx, short gy, short dir);
void move(unsigned char action);
void update_state(unsigned char act, unsigned char *x, unsigned char *y, unsigned char *dir);
void search2018(unsigned char gx,unsigned char gy);

#endif /* SEARCH_H_ */
