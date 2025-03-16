// Assumptions:
// - Traffic takes equal time to reach the exit no matter the exit
// - That every time an exit goes green it hAs eNough cars to release to meet its quota

// Lets say one car takes 7 ticks to finish then each following car takes +1 on this so 2 = 8 3 = 9 etc
// We want to minimise the total waiting time of cars
// Incoming traffic is basically rate of flow

#include <stdio.h>
#include <stdlib.h>

#include "diff-eq-solver.h"

#define SWITCHTIME 1

// thousand ticks and at the end see what configuration keeps the total number fo cars the lowest
void solveDiffEq(int entries, int incomingTraffic[]){
    // TBD
}

int main() {
    int test[5] = {1,1,1,1,1};
    solveDiffEq(5, test);
}