
#include "diff-eq-solver/diff-eq-solver.h"

int entries;
int *incomingTraffic;

int isNumber(const char *str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

void setupLoop() {
    char move[50];
    printf("WELCOME TO TRAFFIC, Type the number(2-10) of desired entrues to start. \n");

    // Get the number of entries
    while (1) {
        printf("Enter a number: "); 
        if (scanf("%49[^\n]", move) == 1) {
            while (getchar() != '\n');
            if (isNumber(move)) {
                entries = atoi(move);
                if (entries >= 2 && entries <= 10) break;
            }
        }
        printf("Invalid input! Please enter a number between 2 and 10.\n");
    }

    incomingTraffic = (int *)malloc(entries * sizeof(int));

    for(int i = 0; i< entries; i++){
        while (1) {
            printf("Enter traffic departure values for entry %d: \n", i + 1);
            if (scanf("%49[^\n]", move) == 1) {
                while (getchar() != '\n');
                if (isNumber(move)) {
                    incomingTraffic[i] = atoi(move);
                    if (incomingTraffic[i] >= 1 && incomingTraffic[i] <= 100) break;
                }
            }
            printf("Invalid input! Please enter a number between 1 and 100.\n");
        }
    }

}

int main() {
    // Gather user inputs
    setupLoop();
    // Solve the optimal traffic light configuration
    solveDiffEq(entries, incomingTraffic);
}