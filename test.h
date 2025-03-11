// utils.h
#ifndef UTILS_H
#define UTILS_H
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "test.h"

typedef struct {
    int x;
    int y;
} coord;

void changeOverflow(int index, int increase, coord entryCoords[], DWORD *pixels);

int getOverflow(int a);

#endif