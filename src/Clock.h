#ifndef CLOCK_H_
#define CLOCK_H_

#define CLOCK	32	//クロックソース[MHz]

//32または20以外の値が設定されたらコンパイル時にエラーを出す
#if (CLOCK != 32) && (CLOCK != 20)
	#error Set the CLOCK to 32 or 20.
#endif

void init_clock(void);

#endif /* CLOCK_H_ */
