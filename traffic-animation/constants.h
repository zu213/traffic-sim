#define ENTRIES 5
#define WIDTH 640
#define HEIGHT 480
#define PADDING 120
#define CARLENGTH 20
extern const int roadWidth;
extern const int halfRoadWidth;
extern const int topRoadHeight;
extern const int bottomRoadHeight;
extern const int verticalRoadLength;
extern const int counterNumberSize;
extern const int counterNumberSize;

extern const int black;
extern const int white;

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    int x;
    int y;
} coord;

// car struct
typedef struct {
    int x;
    int y;
    int destX;
    int destY;
    int xPreferred;
    int completed;
    int travelToMiddle;
} car;

#endif