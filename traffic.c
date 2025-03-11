#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "roads.h"

//C:\Users\evilm\mingw64\bin\gcc.exe traffic.c -o traffic.exe -lgdi32 -luser32
// C:\Users\evilm\mingw64\bin\gcc.exe traffic.c test.c -o traffic.exe -lgdi32 -luser32

HDC hdc;
HDC hdcMem;
HBITMAP hBitmap;
BITMAPINFO bitmapInfo;


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

    animateTraffic();
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
    initCars();
    

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