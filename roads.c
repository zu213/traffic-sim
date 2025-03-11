///// This fiel contains the main setup and animation functionss
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "constants.h"
#include "overflow.h"
#include "cars.h"
#include "roads.h"

int random;
int traffic[ENTRIES] = {1,2,3,4,5}; // how many come from each entry per round
int arrivalTraffic[ENTRIES] = {1,2,2,2,3}; // how many cars arrive ina  tick of frames 1000
car *cars;

DWORD pixels[WIDTH * HEIGHT] = {RGB(255,0,0)};

int frameCounter = 0;
coord entryCoords[ENTRIES]; // entry locations
int numberOfCars = 0; // how many cars their are currently
int currentEntry = 0;
int added = 0;
int completed = 0;
int clear = 1;
int currentExit = 0;

// helper function to find max value in an array
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

void initCars(){
    int carsLength = maxValue(traffic);
    cars = (car *)malloc(carsLength * sizeof(car));
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

void setupRoads(){
    int tempEntries = ENTRIES;
    if(tempEntries < 2 || tempEntries > 10){
        // invalid road numbers
        return;
    }

    entryCoords[0].x = PADDING;
    entryCoords[0].y = topRoadHeight + halfRoadWidth;
    entryCoords[1].x = WIDTH - PADDING;
    entryCoords[1].y = topRoadHeight + halfRoadWidth;

    // Add two main roads
    for(int i = WIDTH * topRoadHeight + PADDING; i < WIDTH * (topRoadHeight + 1) - PADDING; i++){
        pixels[i] = white;
    }
    for(int i = WIDTH * bottomRoadHeight + PADDING; i < WIDTH * (bottomRoadHeight + 1) - PADDING; i++){
        pixels[i] = white;
    }
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
        for(int j = 0; j < verticalRoadLength; j++){
            pixels[counter - j * WIDTH] = white;
        }
        entryCoords[2 + i].x = counter % WIDTH + halfRoadWidth;
        entryCoords[2 + i].y = topRoadHeight - verticalRoadLength;

        for(int j = 0; j < roadWidth;j ++){
            pixels[counter + j] = black;
        }
        counter += roadWidth;
        for(int j = 0; j < verticalRoadLength; j++){
            pixels[counter - j * WIDTH] = white;
        }
        
    }
    // bottom roads and breaks
    remainingRoadSegments = (length - roadWidth * bottomEntries) / (bottomEntries + 1);
    counter = PADDING + WIDTH * bottomRoadHeight;
    for(int i = 0; i < bottomEntries; i++){
        counter += remainingRoadSegments;
        for(int j = 0; j < verticalRoadLength; j++){
            pixels[counter + j * WIDTH] = white;
        }
        entryCoords[2 + topEntries + i].x = counter % WIDTH + halfRoadWidth;
        entryCoords[2 + topEntries + i].y = bottomRoadHeight + verticalRoadLength;

        for(int j = 0; j < roadWidth;j ++){
            pixels[counter + j] = black;
        }
        counter += 60;
        for(int j = 0; j < verticalRoadLength; j++){
            pixels[counter + j * WIDTH] = white;
        }
    }

    for(int i = 0; i < ENTRIES; i++){
        changeLight(entryCoords[i].x, entryCoords[i].y, 0);
    }
    changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
}

void animateTraffic(){
    Sleep(1);
    for(int i =0; i< ENTRIES; i++){
        if((frameCounter * arrivalTraffic[i]) % 1000 < arrivalTraffic[i]){
            changeOverflow(i, 1, entryCoords, pixels);
        }
    }
    frameCounter += 1;

    // chaneg emitter
    if(completed >= traffic[currentEntry] ||
     (getOverflow(currentEntry) == 0 && 1000 <= frameCounter * arrivalTraffic[currentEntry] && completed == added)){
        completed = 0;
        added = 0;
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 0);
        currentEntry += 1;
        currentEntry %= ENTRIES;
        Sleep(100);
        changeLight(entryCoords[currentEntry].x, entryCoords[currentEntry].y, 1);
        //currentExit = 0;
    } // change currentEntry completed = 0 added = 0

    if(currentExit == currentEntry){
        currentExit += 1;
    }

    // check whcihc is current entry and how many cars ahve been added/completed
    if(added < traffic[currentEntry] && clear == 1 && getOverflow(currentEntry) > 0){ //add a car

        currentExit %= ENTRIES;
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
        if(entryCoords[currentEntry].y == entryCoords[currentExit].y && entryCoords[currentEntry].y != topRoadHeight + halfRoadWidth){
            cars[added].travelToMiddle = 1;
        }
        currentExit++;
        added++;
        changeOverflow(currentEntry, 0, entryCoords, pixels);
        clear = 0;
        
    }

    if(abs(cars[added - 1].x - entryCoords[currentEntry].x) > roadWidth || abs(cars[added - 1].y - entryCoords[currentEntry].y) > roadWidth){
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
        if(cars[i].y == topRoadHeight + halfRoadWidth){
            cars[i].xPreferred = 1;
            cars[i].travelToMiddle = 0;
        }
        //move right
        int carPixel = cars[i].y * WIDTH + cars[i].x;

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
    
        else if(!cars[i].xPreferred){
            cars[i].xPreferred = 1;
        }
        // delete the car
        else {
            pixels[carPixel] = RGB(0, 0, 0);
            completed += 1;
            clearCar(cars[i].x, cars[i].y, pixels);
            cars[i].completed = 1;
            // At this point car shouldnt be visible
        }
    }
}