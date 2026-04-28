/**
 * @file led.h
 * @brief LEDの制御モジュール(ヘッダー) - V1対応版
 * @author Yuji KATSUTA, Makoto TANABE
 */

#pragma once

/**
 * @brief LEDの点灯に関するデータを扱う構造体
 */
typedef struct
{
  int pattern;   // 点灯パターン
  int period;    // 点滅周期 [msec]
  int mode;      // 点滅/点灯flag (1～3:点滅, 0:点灯)
} ledv;

/**
 * @brief 構造体ledvへのポインタの型定義
 */
typedef ledv *ledvp;

/**
 * @brief パラメータへのアクセスor関数
 * @return _ledValueへのポインタ
 */
ledvp lv(void);

/**
 * @brief 点灯パターンの設定関数
 * @param [in] bitPattern 点灯パターンのビットマスク指定
 * @details 引数bitPatternで与えられた点灯パターンを_ledValueのpatternメンバー値に代入する
 */
void led(int bitPattern);

/**
 * @brief 点滅モードの設定関数
 * @param [in] mode 点滅有無(1～3:有効, 0:常時点灯)
 * @details 引数modeで与えられた点滅有無を_ledValueのmodeメンバー値に代入する
 */
void ledblinkmode(int mode);

/**
 * @brief 点滅周期の設定関数
 * @param [in] msec 点滅周期[msec]
 * @details 引数msecで与えられた点滅周期を_ledValueのperiodメンバー値に代入する
 */
void ledblinktime(int msec);

/**
 * @brief 点灯・消灯の判断および実行関数
 * @details
 * 定期的に呼び出されることにより、
 * - _ledValue.modeが0(常時点灯)であれば点灯(ledout(_ledValue.pattern))する
 * - _ledValue.modeが1(点滅)であれば、(_ledValue.period / 2)[msec]経過ごとに、
 *   点灯(ledout(_ledValue.pattern))と消灯(ledout(0))を切り替える
 */
void ledexec(void);

/**
 * @brief LED点灯実行関数(内部用)
 * @param [in] n 2進表示する数値
 * @details
 * - V0のled関数から名称変更したledout
 * - bit4が使用できないので、bit4を除いた7bitで2進数表示
 */
void ledout(int n);
