/*
 * adc.h
 *
 *  Created on: May 22, 2025
 *      Author: MS
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "hw_def.h"

#define ADC_MAX_CH  HW_ADC_MAX_CH

bool adcInit(void);
bool adcOpen(uint8_t ch);

#endif /* INC_ADC_H_ */
