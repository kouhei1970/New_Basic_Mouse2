#ifndef __E2FLASH_HEADER__
#define __E2FLASH_HEADER__

void init_Data_Flash(void);
unsigned short * fld_read(unsigned short block);
unsigned long fld_program(unsigned short block,short len,unsigned short *ram);

#endif