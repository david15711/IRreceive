/*
 * adc.c
 *
 *  Created on: May 22, 2025
 *      Author: MS
 */

#include "adc.h"

#ifdef _USE_HW_ADC

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

static bool is_open[ADC_MAX_CH];

bool adcInit(void)
{
  for (int i=0; i<ADC_MAX_CH; i++)
  {
    is_open[i] = false;
  }
  return true;
}

bool adcOpen(uint8_t ch)
{
  bool ret;
  switch(ch)
  {
    case _DEF_ADC1_10:
      ADC_ChannelConfTypeDef sConfig = {0};

      hadc1.Instance = ADC1;
      hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
      hadc1.Init.ContinuousConvMode = ENABLE;
      hadc1.Init.DiscontinuousConvMode = DISABLE;
      hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
      hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
      hadc1.Init.NbrOfConversion = 1;
      if (HAL_ADC_Init(&hadc1) != HAL_OK)
      {
        ret = false;
      }
      else
      {
        ret = true;
        is_open[ch] = true;
        sConfig.Channel = ADC_CHANNEL_10;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
        if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
        {
          ret = false;
        }
      }
      break;
  }
  return ret;
}

__weak void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  (void)hadc;
}

#endif
