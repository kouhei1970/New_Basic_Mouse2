#ifndef WALL_SENSOR_H_
#define WALL_SENSOR_H_

//壁センサdefine
#define SEN_LF		0
#define SEN_LS		1
#define SEN_RS		2
#define SEN_RF		3

extern char		wall_sensor_en;
extern char		wall_control_en;
extern short	sensor_lf;
extern short	sensor_ls;
extern short	sensor_rs;
extern short	sensor_rf;
extern short	ref_ls;
extern short	ref_rs;
extern char		wall_lf;
extern char		wall_ls;
extern char		wall_rs;
extern char		wall_rf;
extern short	thre_lf;
extern short	thre_ls;
extern short	thre_rs;
extern short	thre_rf;

void init_wall_sensor(void);
void update_wall_sensor(char sensor, char led);
short get_wall_diff(void);
void update_center_ref(void);
void output_wall_sensor(void);


#endif /* WALL_SENSOR_H_ */
