#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//C:\Users\evilm\mingw64\bin\gcc.exe traffic.c -o traffic.exe -lgdi32 -luser32

// Predefined variables
#define WIDTH 640
#define HEIGHT 480
#define PADDING 120
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
    int visible;
    int destX;
    int destY;
} car;

// important editable variables
#define ENTRIES 7

int traffic[ENTRIES];
int entryCoords[ENTRIES];
int numberOfCars = 0;
car *cars; // = (car *)malloc(sizeof(car));

int setupRoads(){
    int tempEntries = ENTRIES;
    if(tempEntries < 2 || tempEntries > 10){
        // invalid road numbers
        return 1;
    }
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
        for(int j = 0; j < 60;j ++){
            pixels[counter + j] = RGB(0, 0, 0);
        }
        counter += 60;
        for(int j = 0; j < 120; j++){
            pixels[counter + j * WIDTH] = RGB(255, 255, 255);
        }
    }
}

void animateTraffic(){
    // SLEEP - this will change speed snow falls
    Sleep(50);

    // move each car for each frame
    for(int i = 0; i < sizeof(cars)/sizeof(car); i++){
        if(cars[i].visible == 2){
            //move right
            if(cars[i].x < cars[i].destX){

            }
            // move left
            else if(cars[i].x > cars[i].destX){

            }
            // move up
            else if(cars[i].y > cars[i].destY){

            }
            // move down
            else if(cars[i].y > cars[i].destY){

            }
            // delete the car
            else {
                for(int j = i; j < sizeof(cars)/sizeof(car) - 1; j++) cars[j] = cars[j + 1];
                // realloc mem
                // At this point car shouldnt be visible
            }

            if(abs(cars[i].x - cars[i].destX) < 6 && abs(cars[i].y - cars[i].destY) < 6){
                // remove/cleanup some of the car pixels
            }
            
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