/**
 * @file led.c
 * @brief LEDの制御モジュール - V1対応版
 */

#include "led.h"
#include "timer.h"

#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>

/* ===== 状態構造体 ===== */
static ledv _ledValue = {
  .pattern = 0x05,
  .period  = 1000,
  .mode    = 1
};

/* ===== 内部状態 ===== */
static int last_blink_time = 0;
static int led_is_on = 1;

/* ===== アクセサ ===== */
ledvp lv(void)
{
  return &_ledValue;
}

/* ===== 設定関数 ===== */
void led(int bitPattern)
{
  _ledValue.pattern = bitPattern;
}

void ledblinkmode(int mode)
{
  _ledValue.mode = mode;
  last_blink_time = timer_get_timerv0()->count;
  led_is_on = 1;
}

void ledblinktime(int msec)
{
  _ledValue.period = msec;
  last_blink_time = timer_get_timerv0()->count;
  led_is_on = 1;
}

/* ===== 実行関数 ===== */
void ledexec(void)
{
  int current_time = timer_get_timerv0()->count;
  int adjusted_period = _ledValue.period;
  int half_period;
  int elapsed_time;

  /* ===== mode 0: 常時点灯 ===== */
  if (_ledValue.mode == 0) {
    ledout(_ledValue.pattern);
    return;
  }

  /* ===== mode 1: 切り捨て ===== */
  else if (_ledValue.mode == 1) {
    adjusted_period = (_ledValue.period / 200) * 200;
  }

  /* ===== mode 2: 四捨五入 ===== */
  else if (_ledValue.mode == 2) {
    adjusted_period = ((_ledValue.period + 100) / 200) * 200;
  }

  /* ===== mode 3: 位相制御 ===== */
  else if (_ledValue.mode == 3) {
    int blink_phase = (current_time * 200) % _ledValue.period;

    if (blink_phase < (_ledValue.period / 2)) {
      ledout(_ledValue.pattern);
    } else {
      ledout(0);
    }
    return;
  }

  /* ===== 共通点滅処理（mode1,2） ===== */
  half_period = adjusted_period / 2;
  elapsed_time = (current_time - last_blink_time) * 10;

  if (elapsed_time >= half_period) {
    led_is_on = !led_is_on;

    if (led_is_on) {
      ledout(_ledValue.pattern);
    } else {
      ledout(0);
    }

    last_blink_time = current_time;
  }
}

/* ===== 出力処理 ===== */
void ledout(int n)
{
  static int oled = 0;

  if (oled != n) {
    oled = n;
    PBDR = ~(((n & 0xf0) << 1) | (n & 0x0f));
  }
}
