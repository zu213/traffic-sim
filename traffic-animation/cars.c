#include <windows.h>
#include "cars.h"
#include "constants.h"

void clearCar(int x, int y, DWORD *pixels){
    for(int i = -CARLENGTH/2;i<CARLENGTH/2 + 1;i++){
        for(int j = -CARLENGTH/2;j<CARLENGTH/2 + 1;j++){
            pixels[WIDTH * (y + i) + (x + j)] = RGB(0, 0, 0);
        }
    }
}

void drawCar(int x, int y, int direction, DWORD *pixels){
    if(direction > 4 || direction < 1) return;
    clearCar(x, y, pixels);

    // How the car looks varies absed on direction it is headed
    int carLength;
    int carWidth;
    // left
    if(direction == 1){
        x--;
        carLength = CARLENGTH/2;
        carWidth = carLength/2;
    }
    //right
    else if(direction == 2){
        x++;
        carLength = CARLENGTH/2;
        carWidth = carLength/2;
    }
    // up
    else if(direction == 3){
        y--;
        carWidth = CARLENGTH/2;
        carLength = carWidth/2;
    }
    // down
    else{
        y++;
        carWidth = CARLENGTH/2;
        carLength = carWidth/2;
    }

    for(int i = -carWidth; i < carWidth; i++) pixels[WIDTH * (y + i) + (x - carLength)] = white;
    for(int i = -carWidth; i < carWidth; i++) pixels[WIDTH * (y + i) + (x + carLength)] = white;
    for(int i = -carLength; i < carLength; i++) pixels[WIDTH * (y + carWidth) + (x + i)] = white;
    for(int i = -carLength; i < carLength; i++) pixels[WIDTH * (y  - carWidth) + (x + i)] = white;
}