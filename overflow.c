#include "overflow.h"

int overflows[ENTRIES] = {0};

void clearNumber(int x, int y, DWORD *pixels){
    for(int i = -counterNumberSize/2 - 1; i < counterNumberSize/2 + 1; i++){
        for(int j = -counterNumberSize/2 - 1; j < counterNumberSize/2 + 1; j++){
            pixels[x + j + (y+i) * WIDTH] =  black;
        }
    }
}

void displayDigit(int x, int y, int digit, DWORD *pixels){
    clearNumber(x,y, pixels);

    // horizontal lines
    if(digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -counterNumberSize/2; i < counterNumberSize/2; i++){
            pixels[x + i + (y + counterNumberSize/2 - 1) * WIDTH] = white;
        }
    }
    if(digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -counterNumberSize/2; i < counterNumberSize/2; i++){
            pixels[x + i + (y - counterNumberSize/2 + 1) * WIDTH] = white;
        }
    }
    if(digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9){
        for(int i = -counterNumberSize/2; i < counterNumberSize/2; i++){
            pixels[x + i + (y) * WIDTH] = white;
        }
    }

    // vertical lines
    if(digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = 0; i < counterNumberSize/2; i++){
            pixels[x + counterNumberSize/2 - 1 + (y + i) * WIDTH] = white;
        }
    }
    if(digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -counterNumberSize/2; i < 0; i++){
            pixels[x + counterNumberSize/2 - 1 + (y + i) * WIDTH] = white;
        }
    }
    if(digit == 2 || digit == 6 || digit == 8 || digit == 0){
        for(int i = 0; i < counterNumberSize/2; i++){
            pixels[x - counterNumberSize/2 - 1 + (y + i) * WIDTH] = white;
        }
    }
    if(digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -counterNumberSize/2; i < 0; i++){
            pixels[x - counterNumberSize/2 - 1 + (y + i) * WIDTH] = white;
        }
    }
 
        
}

void displayPlus(int x, int y, DWORD *pixels){
    for(int i = -10; i < 10; i++){
        pixels[x +  (y + i) * WIDTH] = RGB(255, 255, 255);
    }
    for(int i = -10; i < 10; i++){
        pixels[x + i + (y) * WIDTH] = RGB(255, 255, 255);
    }
}

void displayNumber(int x, int y, int number, DWORD *pixels){
    if(number > 99){
        number = 99;
        displayPlus(x + 34, y, pixels);
    }
    int digit = number % 10;
    int tens = number / 10;
    displayDigit(x + 12, y, digit, pixels);
    if(tens > 0){
        displayDigit(x - 12, y , tens, pixels);
    }
}

void changeOverflow(int index, int increase, coord entryCoords[], DWORD *pixels){
    if(increase){
        overflows[index]++;
    }else{
        if(overflows[index] < 1){
            return;
        }
        overflows[index]--;
    }
    if(overflows[index] < 101){
        if(entryCoords[index].y == topRoadHeight + halfRoadWidth){
            if(entryCoords[index].x < WIDTH / 2){
                displayNumber(entryCoords[index].x - roadWidth, entryCoords[index].y, overflows[index], pixels);
            }else{
                displayNumber(entryCoords[index].x + roadWidth, entryCoords[index].y, overflows[index], pixels);
            }
        }
        else if(entryCoords[index].y > topRoadHeight + halfRoadWidth){
            displayNumber(entryCoords[index].x, entryCoords[index].y + roadWidth, overflows[index], pixels);

        }
        else {
            displayNumber(entryCoords[index].x, entryCoords[index].y - roadWidth, overflows[index], pixels);

        }
    }
}

void setOverflow(int i, int j){
    overflows[i] = j;
}

int getOverflow(int i){
    return overflows[i];
}