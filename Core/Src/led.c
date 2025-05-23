/*
 * led.c
 *
 *  Created on: May 22, 2025
 *      Author: MS
 */
#include "led.h"

#ifdef _USE_HW_LED

typedef struct
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  GPIO_PinState on_state;
  GPIO_PinState off_state;
} led_tbl_t;

led_tbl_t led_tbl[LED_MAX_CH] =
{
    {LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET, GPIO_PIN_RESET},
};


bool ledInit(void)
{
  bool ret = true;
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  for(int i = 0; i<LED_MAX_CH; i++)
  {
    GPIO_InitStruct.Pin = led_tbl[i].pin;
    HAL_GPIO_Init(led_tbl[i].port, &GPIO_InitStruct);
  }

}

void ledOn(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].on_state);
}

void ledOff(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_WritePin(led_tbl[ch].port, led_tbl[ch].pin, led_tbl[ch].off_state);
}

void ledToggle(uint8_t ch)
{
  if (ch >= LED_MAX_CH) return;

  HAL_GPIO_TogglePin(led_tbl[ch].port, led_tbl[ch].pin);
}


#endif
