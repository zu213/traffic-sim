#include "constants.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int roadWidth = HEIGHT / 8;
const int halfRoadWidth = HEIGHT / 16;
const int topRoadHeight = HEIGHT / 2 - roadWidth / 2;
const int bottomRoadHeight = HEIGHT / 2 + roadWidth / 2;
const int verticalRoadLength = HEIGHT / 2 - PADDING - halfRoadWidth;
const int counterNumberSize = PADDING / 6;

const int black = RGB(0,0,0);
const int white = RGB(255,255,255);