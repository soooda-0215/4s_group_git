#include <stdio.h>
#include"counter.h"
int main(void)
{
    countup();
    countup();
    printf("現在のカウンター値は%dです。\n", getcount());
    countreset();
    printf("現在のカウンター値は%dです。\n", getcount());
    return 0;
}