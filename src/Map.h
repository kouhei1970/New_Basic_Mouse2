#ifndef MAP_H_
#define MAP_H_

#define MAZE_SIZE	16	//迷路区画数

extern unsigned char map[MAZE_SIZE][MAZE_SIZE];
extern char mouse_x;
extern char mouse_y;
extern char mouse_dir;
extern char goal_x;
extern char goal_y;

void set_map(char x, char y, char dir, char state);
char get_map(char x, char y, char dir);
void reset_map(void);
void round_dir(char d);
char update_mouse_position(char dir);
char check_searched(char x, char y);
void save_map(void);
void load_map(void);
void save_goal(void);
void load_goal(void);
void output_map(void);

#endif /* MAP_H_ */
