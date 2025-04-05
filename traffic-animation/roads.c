///// This fiel contains the main setup and animation functionss
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "overflow.h"
#include "cars.h"
#include "roads.h"

int ENTRIES;
int *traffic; //[ENTRIES];// = {1,2,3,4,5}; // How many come from each entry per round
int *arrivalTraffic; //[ENTRIES];// = {1,2,2,2,3}; // How many cars arrive in a tick of frames(1000)

car *cars; // Main storage for the existing cars
DWORD pixels[WIDTH * HEIGHT] = {0}; // Pixels array
coord *entryCoords; //[ENTRIES]; // Entry coordinates for the roads

int frameCounter = 0;
int numberOfCars = 0; // How many cars their are currently
int currentEntry = 0;
int added = 0; // no. of added cars for this entry
int completed = 0;  // no. of completed cars for this entry
int clear = 1;
int currentExit = 0;

// Helper function to find max value in an array
int maxValue(int * arr, int len){
    int max = 0;
    for(int i = 0; i < len; i++){
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
    if(y < topRoadHeight + halfRoadWidth){
            paintLight(x - halfRoadWidth, y, colour);
        }else if(y > topRoadHeight + halfRoadWidth){
            paintLight(x + halfRoadWidth, y, colour);
        }else{
            if(x < WIDTH / 2){
                paintLight(x, y + halfRoadWidth, colour);
            }else{
                paintLight(x, y - halfRoadWidth, colour);
            }
        }
}

// Main setup function for adding entry coords, drawing roads and traffic lights.
void setupRoads(){
    int tempEntries = ENTRIES;
    if(tempEntries < 2 || tempEntries > 10) return; // invalid road numbers

    entryCoords[0].x = PADDING;
    entryCoords[0].y = topRoadHeight + halfRoadWidth;
    entryCoords[1].x = WIDTH - PADDING;
    entryCoords[1].y = topRoadHeight + halfRoadWidth;

    // Add two main roads
    for(int i = WIDTH * topRoadHeight + PADDING; i < WIDTH * (topRoadHeight + 1) - PADDING; i++) pixels[i] = white;
    for(int i = WIDTH * bottomRoadHeight + PADDING; i < WIDTH * (bottomRoadHeight + 1) - PADDING; i++) pixels[i] = white;

    tempEntries -= 2;
    int topEntries = tempEntries / 2;
    int bottomEntries = tempEntries - topEntries;

    // draw gaps and vertical roads
    int length = WIDTH - 2 * PADDING;
    int remainingRoadSegments = (length - roadWidth * topEntries) / (topEntries + 1);
    int counter = WIDTH * topRoadHeight + PADDING;
    // top roads and breaks
    for(int i = 0; i < topEntries; i++){
        counter += remainingRoadSegments;
        for(int j = 0; j < verticalRoadLength; j++) pixels[counter - j * WIDTH] = white;

        entryCoords[2 + i].x = counter % WIDTH + halfRoadWidth;
        entryCoords[2 + i].y = topRoadHeight - verticalRoadLength;

        for(int j = 0; j < roadWidth;j ++) pixels[counter + j] = black;
        counter += roadWidth;

        for(int j = 0; j < verticalRoadLength; j++) pixels[counter - j * WIDTH] = white;        
    }
    // bottom roads and breaks
    remainingRoadSegments = (length - roadWidth * bottomEntries) / (bottomEntries + 1);
    counter = PADDING + WIDTH * bottomRoadHeight;
    for(int i = 0; i < bottomEntries; i++){
        counter += remainingRoadSegments;
        for(int j = 0; j < verticalRoadLength; j++) pixels[counter + j * WIDTH] = white;

        entryCoords[2 + topEntries + i].x = counter % WIDTH + halfRoadWidth;
        entryCoords[2 + topEntries + i].y = bottomRoadHeight + verticalRoadLength;

        for(int j = 0; j < roadWidth;j ++) pixels[counter + j] = black;
        counter += roadWidth;

        for(int j = 0; j < verticalRoadLength; j++) pixels[counter + j * WIDTH] = white;
    }

    // Setup traffic lights
    for(int i = 0; i < ENTRIES; i++) changeLight(entryCoords[i].x, entryCoords[i].y, 0);
    changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
}

void initCars(int entries, int departureTraffic[], int incomingTraffic[]){

    ENTRIES = entries;
    arrivalTraffic = incomingTraffic;
    traffic = departureTraffic;
    entryCoords = (coord *)malloc(entries * sizeof(coord));

    int carsLength = maxValue(traffic, ENTRIES);
    cars = (car *)malloc(carsLength * sizeof(car));
    setupRoads();
    initOverflows(entries);
}

// Main animation function for moving/adding/counting/deleting traffic
void animateTraffic(){
    Sleep(1);
    for(int i =0; i< ENTRIES; i++){
        if((frameCounter * arrivalTraffic[i]) % 1000 < arrivalTraffic[i]) changeOverflow(i, 1, entryCoords, pixels);
    }
    frameCounter += 1;

    // Change which road cars are leaving from
    if(completed >= traffic[currentEntry] ||
     (getOverflow(currentEntry) == 0 && 1000 <= frameCounter * arrivalTraffic[currentEntry] && completed == added)){
        completed = 0;
        added = 0;
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 0);
        currentEntry += 1;
        currentEntry %= ENTRIES;
        Sleep(100);
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
    }

    if(currentExit == currentEntry) currentExit = (currentEntry + 1) % ENTRIES; // make sure the exit /= the entry
    // Add new cars (if need be)
    if(added < traffic[currentEntry] && clear == 1 && getOverflow(currentEntry) > 0){
        cars[added].x = entryCoords[currentEntry].x;
        cars[added].y = entryCoords[currentEntry].y;
        cars[added].destX = entryCoords[currentExit].x;
        cars[added].destY = entryCoords[currentExit].y;
        cars[added].completed = 0;
        if(entryCoords[currentEntry].y == topRoadHeight + halfRoadWidth){
            cars[added].xPreferred = 1;
        }else{
            cars[added].xPreferred = 0;
        }
        if(entryCoords[currentEntry].y == entryCoords[currentExit].y &&
           entryCoords[currentEntry].y != topRoadHeight + halfRoadWidth){
            cars[added].travelToMiddle = 1;
        }
        currentExit++;
        currentExit %= ENTRIES;
        added++;
        changeOverflow(currentEntry, 0, entryCoords, pixels);
        clear = 0;
    }

    // Check if the car is far enough from the entry so a new one can be added
    if(added > 0) {
        if(abs(cars[added - 1].x - entryCoords[currentEntry].x) > roadWidth || abs(cars[added - 1].y - entryCoords[currentEntry].y) > roadWidth){
            clear = 1;
            if(added == traffic[currentEntry]) changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 0);
        }
    }

    // move each car for each frame
    for(int i = 0; i < added; i++){
        if(cars[i].completed) continue;

        if(cars[i].y == topRoadHeight + halfRoadWidth){
            cars[i].xPreferred = 1;
            cars[i].travelToMiddle = 0;
        }
        //move right
        if(cars[i].x < cars[i].destX && cars[i].xPreferred && !cars[i].travelToMiddle){
            drawCar(cars[i].x, cars[i].y, 2, pixels);
            cars[i].x += 1;
        }
        // move left
        else if(cars[i].x > cars[i].destX && cars[i].xPreferred && !cars[i].travelToMiddle){
            drawCar(cars[i].x, cars[i].y, 1, pixels);
            cars[i].x -= 1;
        }
        // move down
        else if(cars[i].y < cars[i].destY && !cars[i].travelToMiddle || (cars[i].travelToMiddle && cars[i].y < topRoadHeight + halfRoadWidth)){
            drawCar(cars[i].x, cars[i].y, 4, pixels);
            cars[i].y += 1;
        }
        // move up
        else if(cars[i].y > cars[i].destY && !cars[i].travelToMiddle || (cars[i].travelToMiddle && cars[i].y > topRoadHeight + halfRoadWidth)){
            drawCar(cars[i].x, cars[i].y, 3, pixels);
            cars[i].y -= 1;
        } 
        // If it has x distance to do still do do it first unless travel to middle
        else if(!cars[i].xPreferred){
            cars[i].xPreferred = 1;
        }
        // delete the car
        else {
            int carPixel = cars[i].y * WIDTH + cars[i].x;
            pixels[carPixel] = RGB(0, 0, 0);
            completed += 1;
            clearCar(cars[i].x, cars[i].y, pixels);
            cars[i].completed = 1;
        }
    }
}