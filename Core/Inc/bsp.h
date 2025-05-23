/*
 * bsp.h
 *
 *  Created on: May 2, 2025
 *      Author: MS
 */

#ifndef INC_BSP_H_
#define INC_BSP_H_

#include "def.h"
#include "stm32f1xx_hal.h"

void bspInit(void);
void SystemClock_Config(void);
void Error_Handler(void);


#endif /* INC_BSP_H_ */
