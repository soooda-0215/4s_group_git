/**
 * @file led.h
 * @brief LEDの制御モジュール
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

/**
 * @brief LEDの点灯に関するデータを扱う構造体
 */
typedef struct
{
  int pattern;  // 点灯パターン
  int period;   // 点滅周期[msec]
  int mode;     // 点滅モード 0:常時点灯, 1:切り捨て, 2:四捨五入, 3:位相制御
} ledv;

/**
 * @brief 構造体ledvへのポインタの型定義
 */
typedef ledv *ledvp;

/**
 * @brief LEDの状態データへのアクセス関数
 * @return LEDの状態データへのポインタ
 */
ledvp lv(void);

/**
 * @brief 点灯パターンを設定する
 * @param bitPattern 点灯パターン
 */
void led(int bitPattern);

/**
 * @brief 点滅モードを設定する
 * @param mode 点滅モード
 */
void ledblinkmode(int mode);

/**
 * @brief 点滅周期を設定する
 * @param msec 点滅周期[msec]
 */
void ledblinktime(int msec);

/**
 * @brief LEDの点灯または消灯を実行する
 */
void ledexec(void);

/**
 * @brief LEDに値を出力する
 * @param n LEDに出力する値
 */
void ledout(int n);
