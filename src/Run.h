#ifndef RUN_H_
#define RUN_H_

void straight(short length, short top_speed, short end_speed, short acc, char wall_control);
void straight_diag(short length, short top_speed, short end_speed, short acc, char wall_control);
void slalom(short length, short acc, char dir, char wall_control);
void turn(short angle);
void cturn(short angle);

#endif /* RUN_H_ */
