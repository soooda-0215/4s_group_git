/**
 * @file led.c
 * @brief LEDの制御モジュール - V1対応版
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

/**
 * @brief LEDの点灯に関するデータを扱う静的変数
 */
static ledv _ledValue = {
  .pattern = 0x05,  // デフォルト点灯パターン
  .period = 1000,   // デフォルト点滅周期[msec]
  .mode = 1         // デフォルト点滅モード
};

/**
 * @brief LED点滅制御用の内部変数
 */
static int last_blink_time = 0;  // 前回の点灯/消灯切り替え時刻
static int led_is_on = 1;         // LED点灯状態フラグ（1:点灯, 0:消灯）

/**
 * @brief パラメータへのアクセサ関数
 * @return _ledValueへのポインタ
 */
ledvp lv(void)
{
  return &_ledValue;
}

/**
 * @brief 点灯パターンの設定関数
 * @param [in] bitPattern 点灯パターンのビットマスク指定
 */
void led(int bitPattern)
{
  _ledValue.pattern = bitPattern;
}

/**
 * @brief 点滅モードの設定関数
 * @param [in] mode 点滅有無(1～3:有効, 0:常時点灯)
 */
void ledblinkmode(int mode)
{
  _ledValue.mode = mode;
  last_blink_time = timer_get_timerv0()->count;  // タイマーをリセット
  led_is_on = 1;  // 点灯状態に初期化
}

/**
 * @brief 点滅周期の設定関数
 * @param [in] msec 点滅周期[msec]
 */
void ledblinktime(int msec)
{
  _ledValue.period = msec;
  last_blink_time = timer_get_timerv0()->count;  // タイマーをリセット
  led_is_on = 1;  // 点灯状態に初期化
}

/**
 * @brief 点灯・消灯の判断および実行関数
 * @details
 * 定期的に呼び出されることにより、
 * - _ledValue.modeが0(常時点灯)であれば点灯する
 * - _ledValue.modeが1(点滅)であれば、(_ledValue.period / 2)[msec]経過ごとに、
 *   点灯と消灯を切り替える
 */
void ledexec(void)
{
  int current_time = timer_get_timerv0()->count;
  int half_period = _ledValue.period / 2;  // 周期の半分[msec]
  int elapsed_time = (current_time - last_blink_time) * 10;  // タイマーの分解能は10msec

  if (_ledValue.mode == 0) {
    // Mode 0: 常時点灯
    ledout(_ledValue.pattern);
  } else if (_ledValue.mode >= 1) {
    // Mode 1～3: 点滅
    if (elapsed_time >= half_period) {
      // 周期の半分経過したので、点灯/消灯を切り替える
      led_is_on = !led_is_on;
      
      if (led_is_on) {
        ledout(_ledValue.pattern);  // 点灯
      } else {
        ledout(0);  // 消灯
      }
      
      last_blink_time = current_time;  // 時刻を更新
    }
  }
}

/**
 * @brief LED点灯実行関数(内部用)
 * @param [in] n 2進表示する数値
 * @details
 * - V0のled関数から名称変更したledout
 * - bit4が使用できないので、bit4を除いた7bitで2進数表示
 */
void ledout(int n)
{
  // bit4は使用できない、bit4以外は1で点灯
  static int oled = 0;
  if (oled != n) {
    // 同じ場合は実行しない
    oled = n;
    PBDR = ~(((n & 0xf0) << 1) | (n & 0x0f));
  }
  return;
}
