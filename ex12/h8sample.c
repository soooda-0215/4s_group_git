/**
 * @file h8sample.c
 * @brief h8モーターを回転するプログラム
 * @author Yuji KATSUTA, Makoto TANABE
 * @date 2022.4.22
 */
#include "timer.h"

#include <stdlib.h>
#include <h8/reg3067.h>
#include <mes2.h>

void display_count(void)
{
    static int last_count = -1;
    int count = timer_get_timerv0()->count;

    if (count % 10 == 0 && count != last_count)
    {
        printf("%d\n", count);
        last_count = count;
    }
}

int main(void)
{
  init_timer0(COUNT_100MSEC, display_count);
  start_timer0();

  while (1)
  {
		
  }

  stop_timer0();
  return 0;
}
