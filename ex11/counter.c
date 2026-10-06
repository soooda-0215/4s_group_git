#include"counter.h"
static int count = 0;

void countup(void){
    count++;
}

int getcount(void){
    return count;
}

void countreset(void){
    count = 0;
}