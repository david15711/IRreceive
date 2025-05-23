/*
 * ap.c
 *
 *  Created on: May 2, 2025
 *      Author: MS
 */
#include "ap.h"

#define MOV_AVG_BUF 2048
#define NEC_BURST 1
#define NEC_SPACE 0
volatile uint32_t timer_tick = 0;
uint16_t adc_buffer[MOV_AVG_BUF];
uint16_t digit_buffer[MOV_AVG_BUF];

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim1;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  (void)hadc;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM1)
  {
    timer_tick++;
  }
}

void apInit(void)
{
	uartOpen(_DEF_UART2, 115200);
}

/**
 * TODO:  run first adc to check threshold voltage,
 * \      every 1us, execute run Exponential Moving Average Filter,
 * \      binarization,
 * \      determinate rising edge and count the length of pulse,
 * \      if 9ms HIGH (start signal), set CNT = 0, or 9ms LOW (end signal),
 * \      if length of edge to edge is near 1125us mean 0
 * \      if length of edge to edge is near 2250us mean 1
 * \      decode to uint32_t
   */
void apMain(void)
{

  bool bit, last_bit, nec_start;
  uint16_t pos, prev_pos, raw, filtered, threshold, bit_count;
  uint32_t data, t, pulse_width, burst_width, space_width, last_edge_time;

  for(int i = 0; i < 128; i++) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    threshold += HAL_ADC_GetValue(&hadc1);
  }
  threshold >>= 7;
  threshold += 200;
  HAL_ADC_Stop(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, adc_buffer, MOV_AVG_BUF);

  __HAL_TIM_SET_COUNTER(&htim1, 0);
  HAL_TIM_Base_Start_IT(&htim1);
  while(1)
  {
    pos = MOV_AVG_BUF - __HAL_DMA_GET_COUNTER(&hdma_adc1);
    if (pos != prev_pos)
    {
      //alpha = 0.25 exponential mv avg, digitalization
      raw = adc_buffer[pos];
      filtered = filtered - (filtered>>2) + (raw>>2);
      bit = (filtered < threshold) ? NEC_SPACE : NEC_BURST;
      digit_buffer[pos] = bit;

      //edge detect
      if (bit != last_bit)
      {
        if (!bit)//falling edge
        {
          t = (timer_tick << 16) | __HAL_TIM_GET_COUNTER(&htim1);;
          burst_width = t - last_edge_time;
          last_edge_time = t;
        }
        else     //rising edge
        {
          t = (timer_tick << 16) | __HAL_TIM_GET_COUNTER(&htim1);;
          space_width = t - last_edge_time;
          last_edge_time = t;
          pulse_width = burst_width + space_width;
        }

        if(burst_width > 8000 && space_width > 3500)
        {
          nec_start = 1;
        }
        else if(burst_width < 600 && space_width > 5500)
        {
          nec_start = 0;
          uartPrintf(_DEF_UART2, "data: 0x%08x  bit: %d\r\n", data, bit_count);
          data = 0;
          bit_count = 0;
        }

        if(nec_start)
        {
          if (pulse_width > 1000 && pulse_width < 1250)
          {  // 약 1.125ms
            data <<= 1;  // 0비트
            bit_count++;
          }
          else if (pulse_width > 2125 && pulse_width < 2375)
          {  // 약 2.25ms
            data = (data << 1) | 1;  // 1비트
            bit_count++;
          }
        }
      }
      last_bit = bit;
      prev_pos = pos;
    }
  }
}

#if 0
#endif
