/**
 * @file led.c
 * @brief LEDの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */
#include "led.h"
#include "timer.h"

#ifndef H8_H
#define H8_H
#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>
#endif /* H8_H */

static ledv _ledValue = {0x05, 1000, 1};
static int last_blink_time = 0;
static int led_is_on = 1;

ledvp lv(void)
{
  return &_ledValue;
}

void led(int bitPattern)
{
  _ledValue.pattern = bitPattern;
  return;
}

void ledblinkmode(int mode)
{
  _ledValue.mode = mode;
  last_blink_time = timer_get_timerv0()->count;
  led_is_on = 1;
  return;
}

void ledblinktime(int msec)
{
  _ledValue.period = msec;
  last_blink_time = timer_get_timerv0()->count;
  led_is_on = 1;
  return;
}

void ledexec(void)
{
  int current_time = timer_get_timerv0()->count;
  int current_msec = current_time * 100; /* COUNT_100MSECなので1count=100msec */
  int adjusted_period = _ledValue.period;
  int half_period;
  int elapsed_time;

  if (adjusted_period < 200) {
    adjusted_period = 200;
  }

  if (_ledValue.mode == 0) {
    ledout(_ledValue.pattern);
    return;
  }
  if (_ledValue.mode == 1) {
    adjusted_period = (adjusted_period / 200) * 200;
    if (adjusted_period < 200) { adjusted_period = 200; }
  }
  if (_ledValue.mode == 2) {
    adjusted_period = ((adjusted_period + 100) / 200) * 200;
    if (adjusted_period < 200) { adjusted_period = 200; }
  }
  if (_ledValue.mode == 3) {
    if ((current_msec % adjusted_period) < (adjusted_period / 2)) {
      ledout(_ledValue.pattern);
    }
    else {
      ledout(0);
    }
    return;
  }

  half_period = adjusted_period / 2;
  elapsed_time = (current_time - last_blink_time) * 100;

  if (elapsed_time >= half_period) {
    led_is_on = !led_is_on;
    if (led_is_on) {
      ledout(_ledValue.pattern);
    }
    else {
      ledout(0);
    }
    last_blink_time = current_time;
  }
  return;
}

void ledout(int n)
{ // bit4は使用できない、bit4以外は1で点灯
  static int oled = 0;
  if (oled != n)
  { // 同じ場合は実行しない。
    oled = n;
    PBDR = ~(((n & 0xf0) << 1) | (n & 0x0f)); // 2022.01.26 mod
  }
  return;
}
