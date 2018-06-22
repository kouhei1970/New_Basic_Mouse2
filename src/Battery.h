#ifndef BATTERY_H_
#define BATTERY_H_

//バッテリのAD変換ポートの数字（AN00X）
#define AN_BAT			6

extern short battery_vol;

void update_battry(void);
void check_battery(void);
void check_battery_interrupt(void);
void warning_battery(void);

#endif /* BATTERY_H_ */
