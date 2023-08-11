#ifndef _WWDG_H
#define _WWDG_H
#include "sys.h"

void WWDG_Init(uint8_t tr,uint8_t wr,uint32_t fprer);
void WWDG_IRQHandler(void);
#endif
