/**
 * @file main.c
 * @brief h8モーターを回転するプログラム（V1対応版）
 * @author Yuji KATSUTA, Makoto TANABE
 * @date 2025.4.28
 */
#include "getint.h"
#include "timer.h"
#include "motor.h"
#include "led.h"
#include "nled.h"
#include "hardware_init.h"

int main(int argc, char **argv)
{
  // モーターとエンコーダーに関する変数
  int current_encoder_state;     // 現在のエンコーダーの状態
  int previous_encoder_state;    // 前回のエンコーダーの状態
  int motor_rotation_count = 0;  // モーターの回転数
  int motor_on = 0;              // モーターの回転有無（0:停止、1:回転）
  
  // LEDに関する変数
  int led_pattern = 0x05;        // LEDの点灯パターン（デフォルト：0x05）
  int led_blink_mode = 1;        // 点滅モード（デフォルト：1）
  int led_blink_period = 1000;   // LEDの点滅周期[msec]（デフォルト：1000）
  
  // デバッグモードに関する変数
  int debug = 0;                 // デバッグモード（デフォルト：無効）

  // 引数の解析
  if (getint(argc - 1, argv + 1, "-N", &led_pattern) == 0) {
    printf("N=0x%02x", led_pattern);
    printf(LF);
  }
  if (getint(argc - 1, argv + 1, "-F", &led_blink_mode) == 0) {
    printf("F=%d", led_blink_mode);
    printf(LF);
  }
  if (getint(argc - 1, argv + 1, "-B", &led_blink_period) == 0) {
    printf("B=%d", led_blink_period);
    printf(LF);
  }
  if (getint(argc - 1, argv + 1, "-DEBUG", &debug) == 0 && debug) {
    printf("DEBUG MODE");
    printf(LF);
  }

  // ハードウェア初期化
  hardware_init();
  init_timer0(COUNT_100MSEC, NULL);
  start_timer0();
  previous_encoder_state = motor_encoder_state();

  // LED点滅パラメータの初期化
  ledblinktime(led_blink_period);   // 点滅周期の設定
  ledblinkmode(led_blink_mode);     // 点滅モードの設定

  while (1)
  {
    if ((P5DR & 3) == 0)  { break; }           // スイッチが押されたら終了
    if ((P5DR & 1) == 0)  { motor_on = 1; }   // モーターON
    if ((P5DR & 2) == 0)  { motor_on = 0; }   // モーターOFF
    
    if (motor_on) {
      if ((P5DR & 8) == 0) { motor_fwd(); }   // 正転
      else { motor_rev(); }                    // 逆転
    }
    else { motor_stop(); }                     // 停止

    // エンコーダーの状態を読み込み、回転数をカウント
    current_encoder_state = motor_encoder_state();
    if ((previous_encoder_state == 3) && (current_encoder_state == 1)) {
      motor_rotation_count++;
      if (motor_rotation_count > 9999) { motor_rotation_count = 0; }
    }
    if ((previous_encoder_state == 3) && (current_encoder_state == 2)) {
      motor_rotation_count--;
      if (motor_rotation_count < 0) { motor_rotation_count = 9999; }
    }
    previous_encoder_state = current_encoder_state;

    // 数値LEDに回転数を表示
    number_led(motor_rotation_count);

    // デバッグモード：回転数を標準出力に表示
    if (debug) {
      printf("motor_rotation_count = %d", motor_rotation_count);
      printf(LF);
    }

    // LED点滅・点灯処理
    led(led_pattern);        // 点滅パターンの設定
    ledexec();               // 点灯・消灯の判断および実行
  }

  // 終了処理
  motor_stop();
  stop_timer0();
  led(0);
  number_led_off();

  // 使用方法の表示
  printf("usage: %s [-Nzz] [-Fy] [-Bxxx] [-DEBUG1]" LF, argv[0]);
  printf("  zz: bit pattern" LF);
  printf("  y: 0:nonblink, 1:blink(truncate/200), 2:blink(round/200), 3:blink(approx)" LF);
  printf("  xxx: LED blink cycle [msec]" LF);
  printf("  -DEBUG1: turns on debug mode" LF);

  return 0;
}
