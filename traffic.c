#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "traffic-animation/roads.h"

//C:\Users\evilm\mingw64\bin\gcc.exe traffic.c -o traffic.exe -lgdi32 -luser32
// C:\Users\evilm\mingw64\bin\gcc.exe traffic.c traffic-animation/cars.c traffic-animation/constants.c traffic-animation/overflow.c traffic-animation/roads.c -o traffic.exe -lgdi32 -luser32

HDC hdc;
HDC hdcMem;
HBITMAP hBitmap;
BITMAPINFO bitmapInfo;

int isNumber(const char *str) {
    if (*str == '\0') return 0;  // Empty input

    while (*str) {
        if (!isdigit(*str)) return 0;  // If any non-digit found, return false
        str++;
    }
    return 1;  // All characters are digits
}

void setupLoop() {
    char move[50];
    printf("WELCOME TO TRAFFIC, Type a number(2-10) to start \n");
    int entries;
    // Get the number of entries
    while (1) {
        printf("Enter a number: ");
        
        if (scanf("%49[^\n]", move) == 1) {  // Read input (max 49 chars)
            while (getchar() != '\n');  // Clear buffer
            
            if (isNumber(move)) {  // Check if input is a valid number
                entries = atoi(move);  // Convert to integer
                if (entries >= 2 && entries <= 10) {
                    printf("Valid number: %d\n", entries);
                    break;  // Exit loop if valid
                }
            }
        }
        printf("Invalid input! Please enter a number between 2 and 10.\n");
    }

    int departureTraffic[entries];  // Store values
    for(int i = 0; i< entries; i++){
        while (1) {
            printf("Enter traffic departure values for entry %d: \n", i);
            
            if (scanf("%49[^\n]", move) == 1) {  // Read input (max 49 chars)
                while (getchar() != '\n');  // Clear buffer
                
                if (isNumber(move)) {  // Check if input is a valid number
                    departureTraffic[i] = atoi(move);  // Convert to integer
                    if (departureTraffic[i] >= 1 && departureTraffic[i] <= 100) {
                        printf("Valid number: %d\n", departureTraffic[i]);
                        break;  // Exit loop if valid
                    }
                }
            }
            printf("Invalid input! Please enter a number between 1 and 100.\n");
        }
    }


    // Get traffic arriving each 1000 frames
    printf("Enter how much traffic arrives at each entrance in the form (1,2,3,10) \n");
    while(1){
        if (scanf("%99[^\n]", move) == 1){
            while (getchar() != '\n');
            int arrivalTraffic[entries];
            for(int i =0; i<sizeof(move)/sizeof(move[0]); i++){

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
    initCars();

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