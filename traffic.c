#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "test.h"

//C:\Users\evilm\mingw64\bin\gcc.exe traffic.c -o traffic.exe -lgdi32 -luser32

// Predefined variables
#define WIDTH 640
#define HEIGHT 480
#define PADDING 120
#define CARLENGTH 20
const int length = WIDTH * HEIGHT;
DWORD pixels[WIDTH * HEIGHT] = {0};
HDC hdc;
HDC hdcMem;
HBITMAP hBitmap;
BITMAPINFO bitmapInfo;
int random;

// car struct
typedef struct {
    int x;
    int y;
    int destX;
    int destY;
    int xPreferred;
    int completed;
} car;

typedef struct {
    int x;
    int y;
} coord;

// important editable variables
#define ENTRIES 2

int traffic[ENTRIES] = {1,3}; // how many come from each entry per round
int arrivalTraffic[ENTRIES] = {1,2};
int frameCounter = 0;
int overflows[ENTRIES];
coord entryCoords[ENTRIES]; // entry locations
int numberOfCars = 0; // how many cars their are currently
int currentEntry = 0;
car *cars;
int added = 0;
int completed = 0;
int clear = 1;
int currentExit = 0;

int maxValue(int * arr){
    int max = 0;
    int length = sizeof(arr) / sizeof(1);
    for(int i = 0; i < length; i++){
        if(arr[i] > max){
            max = arr[i];
        }
    }
    return max;
}

void paintLight(int x, int y, int colour){
    if(colour < 0 || colour > 1){
        return;
    }
    if(colour == 0){
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                pixels[(x+ j) + (y + i) * WIDTH] = RGB(0, 0, 255);
         }
        }
    }else{
        for(int i = -1; i < 2; i++){
            for(int j = -1; j < 2; j++){
                pixels[(x+ j) + (y + i) * WIDTH] = RGB(0, 255, 0);
         }
        }
    }
}

void changeLight(int x, int y, int colour){
    if(y < 220){
            paintLight(x - 30, y, colour);
        }else if(y > 220){
            paintLight(x + 30, y, colour);
        }else{
            if(x < WIDTH / 2){
                paintLight(x, y + 30, colour);
            }else{
                paintLight(x, y - 30, colour);
            }
        }
}

int setupRoads(){
    int tempEntries = ENTRIES;
    if(tempEntries < 2 || tempEntries > 10){
        // invalid road numbers
        return 1;
    }

    entryCoords[0].x = PADDING;
    entryCoords[0].y = 220;
    entryCoords[1].x = WIDTH - PADDING;
    entryCoords[1].y = 220;

    // Add two main roads
    for(int i = WIDTH * 190 + PADDING; i < WIDTH * 191 - PADDING; i++){
        pixels[i] = RGB(255, 255, 255);
    }
    for(int i = WIDTH * 250 + PADDING; i < WIDTH * 251 - PADDING; i++){
        pixels[i] = RGB(255, 255, 255);
    }
    tempEntries -= 2;
    int topEntries = tempEntries / 2;
    int bottomEntries = tempEntries - topEntries;

    // draw gaps and vertical roads
    int length = WIDTH - 2 * PADDING;
    int remainingRoadSegments = (length - 60 * topEntries) / (topEntries + 1);
    int counter = WIDTH * 190 + PADDING;
    // top roads and breaks
    for(int i = 0; i < topEntries; i++){
        counter += remainingRoadSegments;
        for(int j = 0; j < 120; j++){
            pixels[counter - j * WIDTH] = RGB(255, 255, 255);
        }
        entryCoords[2 + i].x = counter % WIDTH + 30;
        entryCoords[2 + i].y = 70;

        for(int j = 0; j < 60;j ++){
            pixels[counter + j] = RGB(0, 0, 0);
        }
        counter += 60;
        for(int j = 0; j < 120; j++){
            pixels[counter - j * WIDTH] = RGB(255, 255, 255);
        }
        
    }
    // bottom roads and breaks
    remainingRoadSegments = (length - 60 * bottomEntries) / (bottomEntries + 1);
    counter = PADDING + WIDTH * 250;
    for(int i = 0; i < bottomEntries; i++){
        counter += remainingRoadSegments;
        for(int j = 0; j < 120; j++){
            pixels[counter + j * WIDTH] = RGB(255, 255, 255);
        }
        entryCoords[2 + topEntries + i].x = counter % WIDTH + 30;
        entryCoords[2 + topEntries + i].y = HEIGHT - 100;

        for(int j = 0; j < 60;j ++){
            pixels[counter + j] = RGB(0, 0, 0);
        }
        counter += 60;
        for(int j = 0; j < 120; j++){
            pixels[counter + j * WIDTH] = RGB(255, 255, 255);
        }
    }

    for(int i = 0; i < ENTRIES; i++){
        changeLight(entryCoords[i].x, entryCoords[i].y, 0);
    }
    changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
}

void clearNumber(int x, int y){
    for(int i = -10; i < 10; i++){
        for(int j = -10; j < 10; j++){
            pixels[x + j + (y+i) * WIDTH] =  RGB(0, 0, 0);
        }
    }
}

void displayDigit(int x, int y, int digit){
    clearNumber(x,y);

    // horizontal lines
    if(digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -10; i < 10; i++){
            pixels[x + i + (y + 9) * WIDTH] = RGB(255, 255, 255);
        }
    }
    if(digit == 2 || digit == 3 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -10; i < 10; i++){
            pixels[x + i + (y - 9) * WIDTH] = RGB(255, 255, 255);
        }
    }
    if(digit == 2 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9){
        for(int i = -10; i < 10; i++){
            pixels[x + i + (y) * WIDTH] = RGB(255, 255, 255);
        }
    }

    // vertical lines
    if(digit == 1 || digit == 3 || digit == 4 || digit == 5 || digit == 6 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = 0; i < 10; i++){
            pixels[x + 9 + (y + i) * WIDTH] = RGB(255, 255, 255);
        }
    }
    if(digit == 1 || digit == 2 || digit == 3 || digit == 4 || digit == 7 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -10; i < 0; i++){
            pixels[x + 9 + (y + i) * WIDTH] = RGB(255, 255, 255);
        }
    }
    if(digit == 2 || digit == 6 || digit == 8 || digit == 0){
        for(int i = 0; i < 10; i++){
            pixels[x - 9 + (y + i) * WIDTH] = RGB(255, 255, 255);
        }
    }
    if(digit == 4 || digit == 5 || digit == 6 || digit == 8 || digit == 9 || digit == 0){
        for(int i = -10; i < 0; i++){
            pixels[x - 9 + (y + i) * WIDTH] = RGB(255, 255, 255);
        }
    }
 
        
}



void displayPlus(int x, int y){
    for(int i = -10; i < 10; i++){
        pixels[x +  (y + i) * WIDTH] = RGB(255, 255, 255);
    }
    for(int i = -10; i < 10; i++){
        pixels[x + i + (y) * WIDTH] = RGB(255, 255, 255);
    }
}

void displayNumber(int x, int y, int number){
    if(number > 99){
        number = 99;
        displayPlus(x + 34, y);
    }
    int digit = number % 10;
    int tens = number / 10;
    displayDigit(x + 12, y, digit);
    if(tens > 0){
        displayDigit(x - 12, y , tens);
    }
}

void changeOverflow(int index, int increase){
    if(increase){
        overflows[index]++;
    }else{
        if(overflows[index] < 1){
            return;
        }
        overflows[index]--;
    }
    if(overflows[index] < 101){
        if(entryCoords[index].y == 220){
            if(entryCoords[index].x < WIDTH / 2){
                displayNumber(entryCoords[index].x - 50, entryCoords[index].y, overflows[index]);
            }else{
                displayNumber(entryCoords[index].x + 50, entryCoords[index].y, overflows[index]);
            }
        }
        else if(entryCoords[index].y > 220){
            displayNumber(entryCoords[index].x, entryCoords[index].y + 50, overflows[index]);

        }
        else {
            displayNumber(entryCoords[index].x, entryCoords[index].y - 50, overflows[index]);

        }
    }
}

void clearCar(int x, int y){
    for(int i = -CARLENGTH/2;i<CARLENGTH/2 + 1;i++){
        for(int j = -CARLENGTH/2;j<CARLENGTH/2 + 1;j++){
            pixels[WIDTH * (y + i) + (x + j)] = RGB(0,0,0);
        }
    }
}

void drawCar(int x, int y, int direction){
    if(direction > 4 || direction < 1){
        return;
    }
    clearCar(x,y);
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
    // left side

    for(int i = -carWidth; i < carWidth; i++){
        pixels[WIDTH * (y + i) + (x - carLength)] = RGB(255,255,255);
    }
    for(int i = -carWidth; i < carWidth; i++){
        pixels[WIDTH * (y + i) + (x + carLength)] = RGB(255,255,255);
    }
    for(int i = -carLength; i < carLength; i++){
        pixels[WIDTH * (y + carWidth) + (x + i)] = RGB(255,255,255);
    }
    for(int i = -carLength; i < carLength; i++){
        pixels[WIDTH * (y  - carWidth) + (x + i)] = RGB(255,255,255);
    }
}

void animateTraffic(){
    // SLEEP - this will change speed snow falls
    Sleep(1);
    for(int i =0; i< ENTRIES; i++){
        if((frameCounter * arrivalTraffic[i]) % 1000 < arrivalTraffic[i]){
            changeOverflow(i, 1);
        }
    }
    frameCounter += 1;

    if(completed >= traffic[currentEntry] ||
     (overflows[currentEntry] == 0 && 1000 <= frameCounter * arrivalTraffic[currentEntry] && completed == added)){
        completed = 0;
        added = 0;
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 0);
        currentEntry += 1;
        currentEntry %= ENTRIES;
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
        //currentExit = 0;
    } // change currentEntry completed = 0 added = 0

    if(currentExit == currentEntry){
        currentExit += 1;
    }

    // check whcihc is current entry and how many cars ahve been added/completed
    if(added < traffic[currentEntry] && clear == 1 && overflows[currentEntry] > 0){ //add a car

        currentExit %= ENTRIES;
        cars[added].x = entryCoords[currentEntry].x;
        cars[added].y = entryCoords[currentEntry].y;
        cars[added].destX = entryCoords[currentExit].x;
        cars[added].destY = entryCoords[currentExit].y;
        cars[added].completed = 0;
        if(entryCoords[currentEntry].y == 220){
            cars[added].xPreferred = 1;
        }else{
            cars[added].xPreferred = 0;
        }
        currentExit++;

        added++;
        changeOverflow(currentEntry, 0);
        clear = 0;
        
    }

    if(abs(cars[added - 1].x - entryCoords[currentEntry].x) > 30 || abs(cars[added - 1].y - entryCoords[currentEntry].y) > 30){
        clear = 1;
        if(added == traffic[currentEntry]){
            changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 0);
        }
    }

    // move each car for each frame

    for(int i = 0; i < added; i++){
            if(cars[i].completed){
                continue;
            }
            if(cars[i].y == 220){
                cars[i].xPreferred = 1;
            }
            //move right
            int carPixel = cars[i].y * WIDTH + cars[i].x;

            if(cars[i].x < cars[i].destX && cars[i].xPreferred){
                //pixels[carPixel] = RGB(0, 0, 0);
                //pixels[carPixel + 1] = RGB(255, 255, 255);
                drawCar(cars[i].x, cars[i].y, 2);
                cars[i].x += 1;
            }
            // move left
            else if(cars[i].x > cars[i].destX && cars[i].xPreferred){
                //pixels[carPixel] = RGB(0, 0, 0);
                //pixels[carPixel - 1] = RGB(255, 255, 255);
                drawCar(cars[i].x, cars[i].y, 1);
                cars[i].x -= 1;
            }
            // move up
            else if(cars[i].y < cars[i].destY){
                //pixels[carPixel] = RGB(0, 0, 0);
                //pixels[carPixel + WIDTH] = RGB(255, 255, 255);
                drawCar(cars[i].x, cars[i].y, 4);
                cars[i].y += 1;
            }
            // move down
            else if(cars[i].y > cars[i].destY){
                //pixels[carPixel] = RGB(0, 0, 0);
                //pixels[carPixel - WIDTH] = RGB(255, 255, 255);
                drawCar(cars[i].x, cars[i].y, 3);
                cars[i].y -= 1;
            } 
        
            else if(!cars[i].xPreferred){
                cars[i].xPreferred = 1;
            }
            // delete the car
            else {
                pixels[carPixel] = RGB(0, 0, 0);
                completed += 1;
                clearCar(cars[i].x, cars[i].y);
                cars[i].completed = 1;
                // At this point car shouldnt be visible
            }

            if(abs(cars[i].x - cars[i].destX) < 6 && abs(cars[i].y - cars[i].destY) < 6){
                // remove/cleanup some of the car pixels
            }
    }
}

// function to handle screen setup
void setupScreen(HWND hwnd, WPARAM wParam, LPARAM lParam){
    hdc = GetDC(hwnd);
    hdcMem = CreateCompatibleDC(hdc);

    hBitmap = CreateCompatibleBitmap(hdc, WIDTH, HEIGHT);
    SelectObject(hdcMem, hBitmap);

    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = WIDTH;
    bitmapInfo.bmiHeader.biHeight = -HEIGHT;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    GetDIBits(hdcMem, hBitmap, 0, HEIGHT, pixels, &bitmapInfo, DIB_RGB_COLORS);
}

int counter = 0;

void editScreen(HWND hwnd, WPARAM wParam, LPARAM lParam){

    // needs to know waht road it is doing , therefore wht set of cars on rotation
    int durations[4] = {1,2,3,4};
    // each frame counter += 1;
    int index = 0;
    for(int i = 0; i <counter; i++){
        if(i > durations[index]){ // plus previous indexes 
            index += 1;
        }
    }
    counter++;
    counter %= 10;//sum of durations
    animateTraffic(index);

    // Update the bitmap with modified pixel data
    SetDIBits(hdcMem, hBitmap, 0, HEIGHT, pixels, &bitmapInfo, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
}

// Windows procedure 
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
            editScreen(hwnd, wParam, lParam);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main() {
    // initialise random
    srand(time(NULL));
    int carsLength = maxValue(traffic);
    cars = (car *)malloc(carsLength * sizeof(car));
    for(int i = 0; i < ENTRIES; i++){
        overflows[i] = 0;
    }

    add(5,3); // test for import
    // Setup window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = "Traffic";
    wc.hInstance = GetModuleHandle(NULL);
    RegisterClass(&wc); // Let windows create the window

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Traffic",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
                               NULL, NULL, wc.hInstance, NULL);

    // setup roads
    setupScreen(hwnd, WIDTH, HEIGHT);
    setupRoads();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);

    return 0;
}