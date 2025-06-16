/*SEG???????*/
#ifndef __SEG_H            //???,????
#define __SEG_H            //???
#include "main.h"
	
/*????*/
void SEG_ClearScreen(void);
void SEG_SetNum(uint8_t Index);
void SEG_Static_Display(uint8_t Index);
void SEG_Dynamic_Display(uint8_t *Indexes, uint8_t Len);
void SEG_Cror(uint8_t *Indexes, uint8_t Len, uint8_t Rate);
uint8_t SEG_SetNumber_Base(uint32_t Number, uint8_t Base, uint8_t *Indexes, uint8_t Len);
void SEG_Dynamic_Display_Dot(uint8_t *Indexes, uint8_t Len, uint8_t Dp);
#endif   