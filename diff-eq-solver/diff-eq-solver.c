// Assumptions:
// - Traffic takes equal time to reach the exit no matter the exit
// - That every time an exit goes green it ahs neough cars to release to meet its quota

// Lets say one car takes 7 ticks to finish then each following car takes +1 on this so 2 = 8 3 = 9 etc
// We want to minimise the total waiting time of cars
// Incoming traffic is basically rate of flow

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "traffic-animation/roads.h"

// C:\Users\evilm\mingw64\bin\gcc.exe traffic.c traffic-animation/cars.c traffic-animation/constants.c traffic-animation/overflow.c traffic-animation/roads.c -o traffic.exe -lgdi32 -luser32

// going to use a numerical method approach which will iterate througha 
// thousand ticks and at the end see what configuration keeps the total number fo cars the loweest
void solveDiffEq(int entries, int incomingTraffic[]){
    for(int i = 0; i<10;i++);
}

int main() {

    int test[5] = {1,1,1,1,1};
    solveDiffEq(5, test);
}