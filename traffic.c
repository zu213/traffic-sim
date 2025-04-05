#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "traffic-animation/roads.h"

// C:\Users\evilm\mingw64\bin\gcc.exe traffic.c traffic-animation/cars.c traffic-animation/constants.c traffic-animation/overflow.c traffic-animation/roads.c -o traffic.exe -lgdi32 -luser32

HDC hdc;
HDC hdcMem;
HBITMAP hBitmap;
BITMAPINFO bitmapInfo;
int entries;
int *departureTraffic;
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
            while (getchar() != '\n'  && getchar() != EOF);
            if (isNumber(move)) {
                entries = atoi(move);
                if (entries >= 2 && entries <= 10) break;
            }
        } else {
            // Flush
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        printf("Invalid input! Please enter a number between 2 and 10.\n");
    }

    departureTraffic = (int *)malloc(entries * sizeof(int));
    incomingTraffic = (int *)malloc(entries * sizeof(int));

    for(int i = 0; i< entries; i++){
        while (1) {
            printf("Enter traffic departure values for entry %d: \n", i + 1);
            if (scanf("%49[^\n]", move) == 1) {
                while (getchar() != '\n');
                if (isNumber(move)) {
                    departureTraffic[i] = atoi(move);
                    if (departureTraffic[i] >= 1 && departureTraffic[i] <= 100) break;
                }
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            printf("Invalid input! Please enter a number between 1 and 100.\n");
        }
    }

    // Get traffic arriving each 1000 frames
    printf("Enter how much traffic arrives at each entrance in the form (1,2,3,10) \n");
    for(int i = 0; i< entries; i++){
        while (1) {
            printf("Enter traffic arrival values for entry %d: \n", i + 1);
            if (scanf("%49[^\n]", move) == 1) {
                while (getchar() != '\n');
                if (isNumber(move)) { 
                    incomingTraffic[i] = atoi(move);
                    if (incomingTraffic[i] >= 1 && incomingTraffic[i] <= 100) break;
                }
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
            }
            printf("Invalid input! Please enter a number between 1 and 100.\n");
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
    // Main function to animate cars
    animateTraffic();
    // Update the bitmap with modified pixel data
    SetDIBits(hdcMem, hBitmap, 0, HEIGHT, pixels, &bitmapInfo, DIB_RGB_COLORS);
    BitBlt(hdc, 0, 0, WIDTH, HEIGHT, hdcMem, 0, 0, SRCCOPY);
}

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
    // Gather user inputs
    setupLoop();

    // Setup window
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = "Traffic";
    wc.hInstance = GetModuleHandle(NULL);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Traffic",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
                               NULL, NULL, wc.hInstance, NULL);

    setupScreen(hwnd, WIDTH, HEIGHT);
    
    // initialise our cars array and setup roads
    initCars(entries, departureTraffic, incomingTraffic);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdc);

    return 0;
}