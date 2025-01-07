/*
    uhm - v1.0 - public domain - https://github.com/F1L1Pv2/uhm
    binary vector graphics file format - Filip Młodzik 2024-2025
                       no warranty implied; use at your own risk
    
    Before #including
        #define UHM_IMPLEMENTATION
    in the file that you want to have the implementation
*/

#ifndef UHM_FILE_FORMAT
#define UHM_FILE_FORMAT
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*
    This function takes in data inside uhm's format size for this data also dimensions for output image and creates bitmap with 4 channels
*/
char* uhm_encode(char* data, uint32_t size, uint32_t width, uint32_t height);


#ifndef UHM_MALLOC
#define UHM_MALLOC(sz)        malloc(sz)
#define UHM_REALLOC(p,newsz)  realloc(p,newsz)
#define UHM_FREE(p)           free(p)
#endif

#define UHM_PI (3.14159265359)

#define UHM_ASSERT assert

#ifndef UHM_DA_INIT_CAP
#define UHM_DA_INIT_CAP 256
#endif

#ifndef __cplusplus
#   ifndef decltype
#      define decltype(v) void*
#   endif
#endif

#define uhm_append(da, item)                                                                                    \
    do {                                                                                                        \
        if ((da)->count >= (da)->capacity) {                                                                    \
            (da)->capacity = (da)->capacity == 0 ? UHM_DA_INIT_CAP : (da)->capacity*2;                          \
            (da)->items = (decltype((da)->items))UHM_REALLOC((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            UHM_ASSERT((da)->items != NULL && "Buy more RAM lol");                                              \
        }                                                                                                       \
                                                                                                                \
        (da)->items[(da)->count++] = (item);                                                                    \
    } while (0)

#ifndef UHM_NO_STDIO
#include <stdio.h>

#define UHM_PRINTF(x, ...) printf(x, ##__VA_ARGS__)
#else
#define UHM_PRINTF(x, ...)
#endif

#ifdef UHM_IMPLEMENTATION
bool uhm_rotateModifierActive = false;
float uhm_rotateModifierVal = 0.0f;

int uhm_peek(char* data, uint32_t size, uint32_t cursor, char* out){
    if(cursor + 1 > size) return -1;
    *out = *(data+cursor);
    return 0;
}

int uhm_chop8(char* data,uint32_t size, uint32_t* cursor, uint8_t* out){
    if(*cursor + 1 > size) return -1;
    *out = (uint8_t)*(data+*cursor + 0);
    *cursor += 1;
    return 0;
}

int uhm_chop16(char* data,uint32_t size, uint32_t* cursor, uint16_t* out){
    if(*cursor + 2 > size) return -1;
    uint16_t p1 = (uint16_t)((uint8_t)*(data+*cursor + 0));
    uint16_t p2 = (uint16_t)((uint8_t)*(data+*cursor + 1));
    *cursor += 2;
    *out = (((uint16_t)p1) << 0 * 8) | (((uint16_t)p2) << 1 * 8);
    return 0;
}

int uhm_chop32(char* data,uint32_t size, uint32_t* cursor, uint32_t* out){
    if(*cursor + 4 > size) return -1;
    uint32_t p1 = (uint32_t)((uint8_t)*(data+*cursor + 0));
    uint32_t p2 = (uint32_t)((uint8_t)*(data+*cursor + 1));
    uint32_t p3 = (uint32_t)((uint8_t)*(data+*cursor + 2));
    uint32_t p4 = (uint32_t)((uint8_t)*(data+*cursor + 3));
    *cursor += 4;
    *out = (((uint32_t)p1) << 0 * 8) | (((uint32_t)p2) << 1 * 8) | (((uint32_t)p3) << 2 * 8) | (((uint32_t)p4) << 3 * 8);
    return 0;
}

int uhm_chop64(char* data,uint32_t size, uint32_t* cursor, uint64_t* out){
    if(*cursor + 8 > size) return -1;

    uint64_t p1 = (uint64_t)((uint8_t)*(data+*cursor + 0));
    uint64_t p2 = (uint64_t)((uint8_t)*(data+*cursor + 1));
    uint64_t p3 = (uint64_t)((uint8_t)*(data+*cursor + 2));
    uint64_t p4 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p5 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p6 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p7 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p8 = (uint64_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 8;

    *out = (((uint64_t)p1) << 0 * 8) | (((uint64_t)p2) << 1 * 8) | (((uint64_t)p3) << 2 * 8) | (((uint64_t)p4) << 3 * 8) |  (((uint64_t)p1) << 4 * 8) | (((uint64_t)p2) << 5 * 8) | (((uint64_t)p3) << 6 * 8) | (((uint64_t)p4) << 7 * 8);
    return 0;
}

int uhm_chopf32(char* data,uint32_t size, uint32_t* cursor, float* out){
    if(*cursor + 4 > size) return -1;

    uint32_t p1 = (uint32_t)((uint8_t)*(data+*cursor + 0));
    uint32_t p2 = (uint32_t)((uint8_t)*(data+*cursor + 1));
    uint32_t p3 = (uint32_t)((uint8_t)*(data+*cursor + 2));
    uint32_t p4 = (uint32_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 4;
    uint32_t intermediate = (((uint32_t)p1) << 0 * 8) | (((uint32_t)p2) << 1 * 8) | (((uint32_t)p3) << 2 * 8) | (((uint32_t)p4) << 3 * 8);
    *out = *((float*)&intermediate);
    return 0;
}

int uhm_chopf64(char* data,uint32_t size, uint32_t* cursor, double* out){
    if(*cursor + 8 > size) return -1;
    uint64_t p1 = (uint64_t)((uint8_t)*(data+*cursor + 0));
    uint64_t p2 = (uint64_t)((uint8_t)*(data+*cursor + 1));
    uint64_t p3 = (uint64_t)((uint8_t)*(data+*cursor + 2));
    uint64_t p4 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p5 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p6 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p7 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p8 = (uint64_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 8;

    uint32_t intermediate = (((uint64_t)p1) << 0 * 8) | (((uint64_t)p2) << 1 * 8) | (((uint64_t)p3) << 2 * 8) | (((uint64_t)p4) << 3 * 8) |  (((uint64_t)p1) << 4 * 8) | (((uint64_t)p2) << 5 * 8) | (((uint64_t)p3) << 6 * 8) | (((uint64_t)p4) << 7 * 8);
    *out = *((double*)&intermediate);
    return 0;
}

int uhm_expect(char* data, uint32_t size, uint32_t cursor, char expected){
    char peeked;
    int e;
    if((e=uhm_peek(data, size, cursor,&peeked))<0) return e;
    return peeked == expected;
}

uint32_t uhm_lerpColors(uint32_t colorA, uint32_t colorB, double t){
    uint8_t AX = (colorA >> 0*8);
    uint8_t AY = (colorA >> 1*8);
    uint8_t AZ = (colorA >> 2*8);
    uint8_t AW = (colorA >> 3*8);

    uint8_t BX = (colorB >> 0*8);
    uint8_t BY = (colorB >> 1*8);
    uint8_t BZ = (colorB >> 2*8);
    uint8_t BW = (colorB >> 3*8);

    uint8_t CX = (double)AX * (1.0 - t) + (double)BX * t;
    uint8_t CY = (double)AY * (1.0 - t) + (double)BY * t;
    uint8_t CZ = (double)AZ * (1.0 - t) + (double)BZ * t;
    uint8_t CW = (double)AW * (1.0 - t) + (double)BW * t;

    return (((uint32_t)CX) << 0*8) | (((uint32_t)CY) << 1*8) | (((uint32_t)CZ) << 2*8) | (((uint32_t)CW) << 3*8);
}

uint32_t uhm_linearGetColor(int32_t px, int32_t py, int32_t bbx, int32_t bby, uint32_t bbWidth, uint32_t bbHeight, float px1, float py1, float px2, float py2, uint32_t color1, uint32_t color2){
    int32_t ax = py1*bbHeight + bby;
    int32_t ay = px1*bbWidth + bbx;
    int32_t bx = py2*bbHeight + bby;
    int32_t by = px2*bbWidth + bbx;

    //Projecting point onto line
    int32_t directionVecX = bx - ax;
    int32_t directionVecY = by - ay;
    int32_t vecAPx = px - ax;
    int32_t vecAPy = py - ay;
    int32_t APdot = (px - ax) * (bx - ax) + (py - ay) * (by - ay);
    int32_t ddDot = (bx - ax) * (bx - ax) + (by - ay) * (by - ay);
    double projScalar = (double)APdot/ddDot;

    double projX = ax + projScalar*(bx-ax);
    double projY = ay + projScalar*(by-ay);

    //finding interpolator
    double vX = px - ax;
    double vY = py - ay;
    double vD = vX * directionVecX + vY * directionVecY;
    double T = vD / (directionVecX*directionVecX + directionVecY*directionVecY);
    if(vD == 0) T = 0;

    //clamping result
    if (T < 0) T = 0;
    if (T > 1) T = 1;

    return uhm_lerpColors(color1,color2, T);
}

uint32_t uhm_circularGetColor(
    int32_t px, int32_t py, 
    int32_t bbx, int32_t bby, 
    uint32_t bbWidth, uint32_t bbHeight, 
    float centerX, float centerY, 
    float radius, 
    uint32_t colorInner, uint32_t colorOuter) {
    // Compute the center position of the circular gradient
    int32_t cy = centerX * bbWidth + bbx;
    int32_t cx = centerY * bbHeight + bby;

    // Calculate the distance of the point (px, py) from the center (cx, cy)
    double dx = px - cx;
    double dy = py - cy;
    double distance = sqrt(dx * dx + dy * dy);

    // Normalize the distance to the range [0, 1] using the radius
    double t = distance / (radius * sqrt(bbWidth * bbWidth + bbHeight * bbHeight));

    // Clamp `t` to the range [0, 1]
    if (t < 0) t = 0;
    if (t > 1) t = 1;

    // Interpolate the colors based on the normalized distance
    return uhm_lerpColors(colorInner, colorOuter, t);
}

#define cx px1
#define cy py1
#define radius px2

typedef struct {
    float x,y,width,height,px1,py1,px2,py2;
    uint32_t color,color2;
    uint8_t fillType;
    float rotation;
} uhm_rectangle;

int uhm_parse_rectangle(uhm_rectangle* rectangle, char* data, uint32_t size, uint32_t* cursor){
    if(uhm_rotateModifierActive){
        rectangle->rotation = uhm_rotateModifierVal;
        uhm_rotateModifierActive = false;
    }
    
    int e;
    if(
        (e=uhm_chopf32(data,size,cursor,&rectangle->x))<0 ||
        (e=uhm_chopf32(data,size,cursor,&rectangle->y))<0 ||
        (e=uhm_chopf32(data,size,cursor,&rectangle->width))<0 ||
        (e=uhm_chopf32(data,size,cursor,&rectangle->height))<0 ||
        (e=uhm_chop8(data,size,cursor,&rectangle->fillType))<0
    ) return e;
        
    if(rectangle->fillType == 'F'){
        if((e=uhm_chop32(data,size,cursor,&rectangle->color))<0) return e;
    }
    else if(rectangle->fillType == 'L'){
        if(
            (e=uhm_chopf32(data,size,cursor,&rectangle->px1))<0 ||
            (e=uhm_chopf32(data,size,cursor,&rectangle->py1))<0 ||
            (e=uhm_chopf32(data,size,cursor,&rectangle->px2))<0 ||
            (e=uhm_chopf32(data,size,cursor,&rectangle->py2))<0 ||
            (e=uhm_chop32(data,size,cursor,&rectangle->color))<0 ||
            (e=uhm_chop32(data,size,cursor,&rectangle->color2))<0
        ) return e;
    }
    else if(rectangle->fillType == 'C'){
        if(
            (e=uhm_chopf32(data,size,cursor,&rectangle->cx))<0 ||
            (e=uhm_chopf32(data,size,cursor,&rectangle->cy))<0 ||
            (e=uhm_chopf32(data,size,cursor,&rectangle->radius))<0 ||
            (e=uhm_chop32(data,size,cursor,&rectangle->color))<0 ||
            (e=uhm_chop32(data,size,cursor,&rectangle->color2))<0
        ) return e;
    }
    else {
        UHM_PRINTF("ParseRectangle: UNKNOWN FILL TYPE\n");
        return -1;
    }

    return 0;
}

int uhm_draw_rectangle(uhm_rectangle* rectangle,uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotateIN){
    float rotate = -(rectangle->rotation + rotateIN);
    float centerX = (rectangle->x + gx) * width;
    float centerY = (rectangle->y + gy) * height;
    float cosTheta = cosf(rotate);
    float sinTheta = sinf(rotate);
    float halfWidth = rectangle->width * width / 2.0f;
    float halfHeight = rectangle->height * height / 2.0f;

    UHM_PRINTF("Drawing rectangle x: %.2f, y: %.2f, width: %.2f, height: %.2f\n", centerX, centerY, halfWidth * 2, halfHeight * 2);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            float localX = (j - centerX) * cosTheta + (i - centerY) * sinTheta;
            float localY = -(j - centerX) * sinTheta + (i - centerY) * cosTheta;
            if (localX >= -halfWidth && localX <= halfWidth && localY >= -halfHeight && localY <= halfHeight) {
                float normX = (localX + halfWidth) / (2 * halfWidth);
                float normY = (localY + halfHeight) / (2 * halfHeight);
                uint32_t color = rectangle->color;
                if (rectangle->fillType == 'L') {
                    float rotatedPx1 = ((rectangle->px1 - 0.5) * cosf(rotate) - (rectangle->py1 - 0.5) * sinf(rotate)) + 0.5;
                    float rotatedPy1 = ((rectangle->px1 - 0.5) * sinf(rotate) + (rectangle->py1 - 0.5) * cosf(rotate)) + 0.5;
                    float rotatedPx2 = ((rectangle->px2 - 0.5) * cosf(rotate) - (rectangle->py2 - 0.5) * sinf(rotate)) + 0.5;
                    float rotatedPy2 = ((rectangle->px2 - 0.5) * sinf(rotate) + (rectangle->py2 - 0.5) * cosf(rotate)) + 0.5;
                    color = uhm_linearGetColor(
                        i, j, centerX - halfWidth, centerY - halfHeight,
                        2 * halfWidth, 2 * halfHeight,
                        rotatedPx1, rotatedPy1, rotatedPx2, rotatedPy2,
                        rectangle->color, rectangle->color2);
                } else if (rectangle->fillType == 'C') {
                    float rotatedCx = ((rectangle->cx - 0.5) * cosf(rotate) - (rectangle->cy - 0.5) * sinf(rotate)) + 0.5;
                    float rotatedCy = ((rectangle->cx - 0.5) * sinf(rotate) + (rectangle->cy - 0.5) * cosf(rotate)) + 0.5;
                    color = uhm_circularGetColor(
                        i, j, centerX - halfWidth, centerY - halfHeight,
                        2 * halfWidth, 2 * halfHeight,
                        rotatedCx, rotatedCy, rectangle->radius,
                        rectangle->color, rectangle->color2);
                }
                ((uint32_t*)output_data)[i * width + j] = color;
            }
        }
    }

    return 0;
}

typedef struct {
    float x,y,r,px1,py1,px2,py2;
    uint8_t fillType;
    uint32_t color,color2;
    float rotation;
} uhm_circle;

int uhm_parse_circle(uhm_circle* circle, char* data, uint32_t size, uint32_t* cursor){
    if(uhm_rotateModifierActive){
        circle->rotation = uhm_rotateModifierVal;
        uhm_rotateModifierActive = false;
    }

    int e;
    if(
        (e=uhm_chopf32(data,size,cursor,&circle->x))<0||
        (e=uhm_chopf32(data,size,cursor,&circle->y))<0||
        (e=uhm_chopf32(data,size,cursor,&circle->r))<0||
        (e=uhm_chop8(data,size,cursor,&circle->fillType))<0
    ) return e;
        
    if(circle->fillType == 'F'){
        if((e=uhm_chop32(data,size,cursor,&circle->color))<0) return e;
    }
    else if(circle->fillType == 'L'){
        if(
            (e=uhm_chopf32(data,size,cursor,&circle->px1))<0||
            (e=uhm_chopf32(data,size,cursor,&circle->py1))<0||
            (e=uhm_chopf32(data,size,cursor,&circle->px2))<0||
            (e=uhm_chopf32(data,size,cursor,&circle->py2))<0||
            (e=uhm_chop32(data,size,cursor,&circle->color))<0||
            (e=uhm_chop32(data,size,cursor,&circle->color2))<0
        ) return e;
    }
    else if(circle->fillType == 'C'){
        if(
            (e=uhm_chopf32(data,size,cursor,&circle->cx))<0||
            (e=uhm_chopf32(data,size,cursor,&circle->cy))<0||
            (e=uhm_chopf32(data,size,cursor,&circle->radius))<0||
            (e=uhm_chop32(data,size,cursor,&circle->color))<0||
            (e=uhm_chop32(data,size,cursor,&circle->color2))<0
        ) return e;
    }

    else {
        UHM_PRINTF("ParseCircle: UNKNOWN FILL TYPE\n");
        return -1;
    }

    return 0;
}

int uhm_draw_circle(uhm_circle* circle, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotateIN){
    float rotate = circle->rotation + rotateIN;
    int32_t realX = (circle->x+gx)*width;
    int32_t realY = (circle->y+gy)*width;
    int32_t realR = circle->r*width;

    UHM_PRINTF("Drawing circle x: %d y: %d radius: %d\n",realX,realY,realR);

    for(int32_t i = realY - realR; i < realY + realR; i++){
        if(i < 0 || i >= width) continue;
        for(int j = realX - realR; j < realX + realR; j++){
            if(j < 0 || j >= height) continue;
            uint32_t y = i - realY;
            uint32_t x = j - realX;
            if(y*y + x*x < realR*realR){
                if(circle->fillType == 'L'){
                    float rotatedPx1 = ((circle->px1 - 0.5) * cosf(-rotate) - (circle->py1 - 0.5) * sinf(-rotate)) + 0.5;
                    float rotatedPy1 = ((circle->px1 - 0.5) * sinf(-rotate) + (circle->py1 - 0.5) * cosf(-rotate)) + 0.5;
                    float rotatedPx2 = ((circle->px2 - 0.5) * cosf(-rotate) - (circle->py2 - 0.5) * sinf(-rotate)) + 0.5;
                    float rotatedPy2 = ((circle->px2 - 0.5) * sinf(-rotate) + (circle->py2 - 0.5) * cosf(-rotate)) + 0.5;

                    ((uint32_t*)output_data)[i*width + j] = uhm_linearGetColor(
                        i,j,
                        realX - realR,realY - realR,
                        realR*2,realR*2,
                        rotatedPx1,rotatedPy1,
                        rotatedPx2,rotatedPy2,
                        circle->color,circle->color2
                    );
                }
                else if(circle->fillType == 'C'){
                    float rotatedCx = ((circle->cx - 0.5) * cosf(-rotate) - (circle->cy - 0.5) * sinf(-rotate)) + 0.5;
                    float rotatedCy = ((circle->cx - 0.5) * sinf(-rotate) + (circle->cy - 0.5) * cosf(-rotate)) + 0.5;

                    ((uint32_t*)output_data)[i*width + j] = uhm_circularGetColor(
                        i,j,
                        realX - realR,realY - realR,
                        realR*2,realR*2,
                        rotatedCx,rotatedCy,
                        circle->radius,
                        circle->color,circle->color2
                    );
                }
                else{
                    ((uint32_t*)output_data)[i*width + j] = circle->color;
                }
            }
        }
    }

    return 0;
}

typedef struct {
    float x,y,rw,rh,px1,py1,px2,py2;
    uint8_t fillType;
    uint32_t color,color2;
    float rotation;
} uhm_ellipse;

int uhm_parse_ellipse(uhm_ellipse* ellipse, char* data, uint32_t size, uint32_t* cursor){
    if(uhm_rotateModifierActive){
        ellipse->rotation = uhm_rotateModifierVal;
        uhm_rotateModifierActive = false;
    }

    int e;
    if(
        (e=uhm_chopf32(data,size,cursor,&ellipse->x))<0||
        (e=uhm_chopf32(data,size,cursor,&ellipse->y))<0||
        (e=uhm_chopf32(data,size,cursor,&ellipse->rw))<0||
        (e=uhm_chopf32(data,size,cursor,&ellipse->rh))<0||
        (e=uhm_chop8(data,size,cursor,&ellipse->fillType))<0
    ) return e;
        
    if(ellipse->fillType == 'F'){
        if((e=uhm_chop32(data,size,cursor,&ellipse->color))<0) return e;
    }
    else if(ellipse->fillType == 'L'){
        if(
            (e=uhm_chopf32(data,size,cursor,&ellipse->px1))<0||
            (e=uhm_chopf32(data,size,cursor,&ellipse->py1))<0||
            (e=uhm_chopf32(data,size,cursor,&ellipse->px2))<0||
            (e=uhm_chopf32(data,size,cursor,&ellipse->py2))<0||
            (e=uhm_chop32(data,size,cursor,&ellipse->color))<0||
            (e=uhm_chop32(data,size,cursor,&ellipse->color2))<0
        ) return e;
    }
    else if(ellipse->fillType == 'C'){
        if(
            (e=uhm_chopf32(data,size,cursor,&ellipse->cx))<0||
            (e=uhm_chopf32(data,size,cursor,&ellipse->cy))<0||
            (e=uhm_chopf32(data,size,cursor,&ellipse->radius))<0||
            (e=uhm_chop32(data,size,cursor,&ellipse->color))<0||
            (e=uhm_chop32(data,size,cursor,&ellipse->color2))<0
        ) return e;
    }

    else {
        UHM_PRINTF("ParseCircle: UNKNOWN FILL TYPE\n");
        return -1;
    }

    return 0;
}


int uhm_draw_ellipse(uhm_ellipse* ellipse, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotateIN) {
    float rotate = -(ellipse->rotation + rotateIN);
    float realX = (ellipse->x + gx) * width;
    float realY = (ellipse->y + gy) * height;
    float centerX = realX;
    float centerY = realY;
    float realRx = ellipse->rw * width;
    float realRy = ellipse->rh * height;
    float cosTheta = cosf(rotate);
    float sinTheta = sinf(rotate);

    UHM_PRINTF("Drawing rotated ellipse at center x: %.2f, y: %.2f, rx: %.2f, ry: %.2f, rotation: %.2f radians\n", centerX, centerY, realRx, realRy, rotate);

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            float localX = (j - centerX) * cosTheta + (i - centerY) * sinTheta;
            float localY = -(j - centerX) * sinTheta + (i - centerY) * cosTheta;
            float normX = localX / realRx;
            float normY = localY / realRy;
            if (normX * normX + normY * normY <= 1.0f) {
                uint32_t color = ellipse->color;
                float rotatedPx1 = ((ellipse->px1 - 0.5) * cosf(rotate) - (ellipse->py1 - 0.5) * sinf(rotate)) + 0.5;
                float rotatedPy1 = ((ellipse->px1 - 0.5) * sinf(rotate) + (ellipse->py1 - 0.5) * cosf(rotate)) + 0.5;
                float rotatedPx2 = ((ellipse->px2 - 0.5) * cosf(rotate) - (ellipse->py2 - 0.5) * sinf(rotate)) + 0.5;
                float rotatedPy2 = ((ellipse->px2 - 0.5) * sinf(rotate) + (ellipse->py2 - 0.5) * cosf(rotate)) + 0.5;

                if (ellipse->fillType == 'L') {
                    color = uhm_linearGetColor(
                        i, j,
                        realX - realRx, realY - realRy,
                        realRx * 2, realRy * 2,
                        rotatedPx1, rotatedPy1,
                        rotatedPx2, rotatedPy2,
                        ellipse->color, ellipse->color2
                    );
                } else if (ellipse->fillType == 'C') {
                    float rotatedCx = ((ellipse->cx - 0.5) * cosf(rotate) - (ellipse->cy - 0.5) * sinf(rotate)) + 0.5;
                    float rotatedCy = ((ellipse->cx - 0.5) * sinf(rotate) + (ellipse->cy - 0.5) * cosf(rotate)) + 0.5;

                    color = uhm_circularGetColor(
                        i, j,
                        realX - realRx, realY - realRy,
                        realRx * 2, realRy * 2,
                        rotatedCx, rotatedCy,
                        ellipse->radius,
                        ellipse->color, ellipse->color2
                    );
                }

                ((uint32_t*)output_data)[i * width + j] = color;
            }
        }
    }

    return 0;
}

#undef cx
#undef cy
#undef radius

typedef struct{
    uint8_t opcode;
    void* data;
    bool skip_draw;
} uhm_instruction;

int uhm_parse_instruction(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction);
int uhm_draw_instruction(uhm_instruction* instruction, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotation);
int uhm_get_location(uhm_instruction* instruction, float* out_x, float* out_y);

typedef struct {
    uhm_instruction *items;
    size_t           count;
    size_t           capacity;
} uhm_instructions;

typedef struct{
    float ox,oy;
    uint16_t rows, cols;
    uhm_instructions instructions;
    float rotation;
} uhm_tiledPattern;

int uhm_parse_tiledPattern(uhm_tiledPattern* tiledPattern, char* data, uint32_t size, uint32_t* cursor){
    if(uhm_rotateModifierActive){
        tiledPattern->rotation = uhm_rotateModifierVal;
        uhm_rotateModifierActive = false;
    }

    int e;
    if(
        (e=uhm_chopf32(data,size,cursor,&tiledPattern->ox))<0||
        (e=uhm_chopf32(data,size,cursor,&tiledPattern->oy))<0||
        (e=uhm_chop16(data,size,cursor,&tiledPattern->rows))<0||
        (e=uhm_chop16(data,size,cursor,&tiledPattern->cols))<0
    ) return e;
    
    tiledPattern->instructions.capacity = 0;
    tiledPattern->instructions.count = 0;
    tiledPattern->instructions.items = 0;

    while(true){
        if(*cursor == size){
            UHM_PRINTF("end clause wasn't found\n");
            return -1;
        }
        uhm_instruction instruction = {0};
        if((e=uhm_parse_instruction(data,size,cursor,&instruction))<0){
            if(instruction.data) UHM_FREE(instruction.data);
            return e;
        }

        if(instruction.skip_draw) continue;
        if(instruction.opcode == ']') break;

        uhm_append(&tiledPattern->instructions,instruction);
    }

    return 0;
}

int uhm_draw_tiledPattern(uhm_tiledPattern* tiledPattern, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotateIN){
    if(rotateIN != 0 || rotateIN < 0 || rotateIN > 0 || tiledPattern->rotation != 0 || tiledPattern->rotation < 0 || tiledPattern->rotation > 0){
        UHM_PRINTF("DrawTiledPattern rotation currently not supported \n");
        return -1;
    }
    int e;
    for(int i = 0; i < tiledPattern->rows; i++){
        for(int j = 0; j < tiledPattern->cols; j++){
            for(int index = 0; index < tiledPattern->instructions.count; index++){
                if(tiledPattern->instructions.items[index].skip_draw) continue;
                if((e=uhm_draw_instruction(&tiledPattern->instructions.items[index],width,height,output_data,tiledPattern->ox*j + gx,tiledPattern->oy*i + gy, rotateIN))<0) return e;
            }
        }
    }
    return 0;
}

typedef struct {
    uint16_t patternID;
    uhm_instructions instructions;
} uhm_pattern;

typedef struct {
    uint16_t patternID;
    float x;
    float y;
    float rotation;
} uhm_place_pattern;

typedef struct {
    uhm_pattern* items;
    size_t       count;
    size_t       capacity;
} uhm_patterns;

uhm_patterns patterns = {0};

int uhm_parse_pattern(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction){
    int e;
    uint8_t mode;
    uint16_t patternID;
    if(
        (e=uhm_chop8(data,size,cursor,&mode))<0||
        (e=uhm_chop16(data,size,cursor,&patternID))<0
    ) return e;

    if(mode == 'P'){
        float x,y;
        if(
            (e=uhm_chopf32(data,size,cursor,&x))<0||
            (e=uhm_chopf32(data,size,cursor,&y))<0
        ) return e;
        instruction->data = UHM_MALLOC(sizeof(uhm_place_pattern));
        ((uhm_place_pattern*)(instruction->data))->patternID = patternID;
        ((uhm_place_pattern*)(instruction->data))->x = x;
        ((uhm_place_pattern*)(instruction->data))->y = y;
        ((uhm_place_pattern*)(instruction->data))->rotation = 0;
        if(uhm_rotateModifierActive){
            ((uhm_place_pattern*)(instruction->data))->rotation = uhm_rotateModifierVal;
            uhm_rotateModifierActive = false;
        }
        return 0;
    }
    else if(mode == 'R'){
        instruction->skip_draw = true;
        uhm_pattern pattern = {0};
        pattern.patternID = patternID;

        while(true){
            if(*cursor == size){
                UHM_PRINTF("end clause wasn't found\n");
                return -1;
            }
            uhm_instruction instruction = {0};
            if((e=uhm_parse_instruction(data,size,cursor,&instruction))<0){
                if(instruction.data) UHM_FREE(instruction.data);
                return e;
            }

            if(instruction.skip_draw) continue;
            if(instruction.opcode == ']') break;

            uhm_append(&pattern.instructions,instruction);
        }

        uhm_append(&patterns, pattern);
        return 0;
    }
    return -1;
}

int uhm_parse_rotateModifier(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction){
    instruction->skip_draw = true;
    int e;
    float intermediate;
    if((e=uhm_chopf32(data,size,cursor,&intermediate))<0) return e;
    if(uhm_rotateModifierActive){
        uhm_rotateModifierVal += intermediate;
    }else{
        uhm_rotateModifierVal = intermediate;
    }
    uhm_rotateModifierActive = true;
    return 0;
}

int uhm_parse_instruction(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction){
    int e;
    uint8_t opcode;
    if((e=uhm_chop8(data,size,cursor,&opcode))<0) return e;

    instruction->opcode = opcode;

    if(opcode == 'R'){
        uhm_rectangle rectangle = {0};
        if((e=uhm_parse_rectangle(&rectangle, data,size,cursor))<0) return e;
        instruction->data = UHM_MALLOC(sizeof(uhm_rectangle));
        *(uhm_rectangle*)(instruction->data) = rectangle;
        return 0;
    }
    else if(opcode == 'C'){
        uhm_circle circle = {0};
        if((e=uhm_parse_circle(&circle, data,size,cursor))<0) return e;
        instruction->data = UHM_MALLOC(sizeof(uhm_circle));
        *(uhm_circle*)(instruction->data) = circle;
        return 0;
    }
    else if(opcode == 'E'){
        uhm_ellipse ellipse = {0};
        if((e=uhm_parse_ellipse(&ellipse, data,size,cursor))<0) return e;
        instruction->data = UHM_MALLOC(sizeof(uhm_ellipse));
        *(uhm_ellipse*)(instruction->data) = ellipse;
        return 0;
    }
    else if(opcode == 'T'){
        uhm_tiledPattern tiledPattern;
        if((e=uhm_parse_tiledPattern(&tiledPattern, data,size,cursor))<0) {
            if(tiledPattern.instructions.items != NULL) UHM_FREE(tiledPattern.instructions.items);
            return e;
        }
        instruction->data = UHM_MALLOC(sizeof(uhm_tiledPattern));
        *(uhm_tiledPattern*)(instruction->data) = tiledPattern;
        return 0;
    }
    else if(opcode == 'P'){
        return uhm_parse_pattern(data,size,cursor,instruction);
    }
    else if(opcode == '|'){
        return uhm_parse_rotateModifier(data,size,cursor,instruction);
    }
    else if(opcode == ']'){
        return 0;
    }

    UHM_PRINTF("Parse: Unknown Opcode: %c\n", opcode);
    return -1;
};

int uhm_draw_placePattern(uhm_place_pattern* patternDesc, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotateIN){
    float rotate = patternDesc->rotation + rotateIN;
    
    uhm_pattern* pattern = NULL;
    int e;
    for(int i = 0; i < patterns.count; i++){
        if(patterns.items[i].patternID == patternDesc->patternID){
            pattern = &patterns.items[i];
            break;
        }
    }
    if(pattern == NULL){
        UHM_PRINTF("Unknown patternID %d\n",patternDesc->patternID);
        return -1;
    }

    float realX = patternDesc->x + gx;
    float realY = patternDesc->y + gy;

    for(int i = 0; i < pattern->instructions.count; i++){
        if(pattern->instructions.items[i].skip_draw) continue;

        float localX, localY;
        if((e=uhm_get_location(&pattern->instructions.items[i],&localX,&localY))<0) return e;

        float rotatedX = (localX * cosf(-rotate) - localY *sinf(-rotate));
        float rotatedY = (localX * sinf(-rotate) + localY *cosf(-rotate));

        float outX = realX + (rotatedX - localX);
        float outY = realY + (rotatedY - localY);
        
        // UHM_PRINTF("Angle %f deg\nNormal x: %fy %f\nRotated x: %fy: %f\n", rotate*180/UHM_PI,localX,localY,rotatedX,rotatedY);

        if((e=uhm_draw_instruction(&pattern->instructions.items[i],width,height,output_data,outX,outY, rotate))<0) return e;
    }
    return 0;
}

int uhm_get_location(uhm_instruction* instruction, float* out_x, float* out_y){
    int e;
    if(instruction->opcode == 'R'){
        *out_x = ((uhm_rectangle*)instruction->data)->x;
        *out_y = ((uhm_rectangle*)instruction->data)->y;
    }
    else if(instruction->opcode == 'C'){
        *out_x = ((uhm_circle*)instruction->data)->x;
        *out_y = ((uhm_circle*)instruction->data)->y;
    }
    else if(instruction->opcode == 'E'){
        *out_x = ((uhm_ellipse*)instruction->data)->x;
        *out_y = ((uhm_ellipse*)instruction->data)->y;
    }
    else if(instruction->opcode == 'T'){
        *out_x = ((uhm_tiledPattern*)instruction->data)->ox;
        *out_y = ((uhm_tiledPattern*)instruction->data)->oy;
    }
    else if(instruction->opcode == 'P'){
        *out_x = ((uhm_place_pattern*)instruction->data)->x;
        *out_y = ((uhm_place_pattern*)instruction->data)->y;
    }
    else{
        UHM_PRINTF("GetInstructionLocation: Unknown Opcode %c\n", instruction->opcode);
        return -1;
    }
    return 0;
}

int uhm_draw_instruction(uhm_instruction* instruction, uint32_t width, uint32_t height, char* output_data, float gx, float gy, float rotation){
    int e;
         if(instruction->opcode == 'R') {if((e=uhm_draw_rectangle((uhm_rectangle*)instruction->data,width,height,output_data, gx, gy, rotation))<0) return e;}
    else if(instruction->opcode == 'C') {if((e=uhm_draw_circle((uhm_circle*)instruction->data,width,height,output_data, gx, gy, rotation))<0) return e;}
    else if(instruction->opcode == 'E') {if((e=uhm_draw_ellipse((uhm_ellipse*)instruction->data,width,height,output_data, gx, gy, rotation))<0) return e;}
    else if(instruction->opcode == 'T') {if((e=uhm_draw_tiledPattern((uhm_tiledPattern*)instruction->data,width,height,output_data, gx, gy, rotation))<0) return e;}
    else if(instruction->opcode == 'P') {if((e=uhm_draw_placePattern((uhm_place_pattern*)instruction->data,width,height,output_data, gx, gy, rotation))<0) return e;}
    else{
        UHM_PRINTF("Draw: Unknown Opcode %c\n", instruction->opcode);
        return -1; 
    }
    return 0;
}

char* uhm_encode(char* data, uint32_t size, uint32_t width, uint32_t height){
    UHM_PRINTF("Got %u bytes\n", size);
    char* output_data = (char*)UHM_MALLOC(width*height*4);
    uint32_t cursor = 0;

    if(patterns.count > 0){
        for(int i = 0; i < patterns.count; i++){
            if(patterns.items[i].instructions.items != NULL) UHM_FREE(patterns.items[i].instructions.items);
        }
    }

    patterns.count = 0;

    if(!uhm_expect(data,size,cursor,'U')) {
        UHM_FREE(output_data);
        return NULL;
    }
    cursor++;
    if(!uhm_expect(data,size,cursor,'H')) {
        UHM_FREE(output_data);
        return NULL;
    }
    cursor++;
    if(!uhm_expect(data,size,cursor,'M')) {
        UHM_FREE(output_data);
        return NULL;
    }
    cursor++;
    UHM_PRINTF("File Verified\n");

    // setting image background color
    uint32_t backgroundColor;
    int e;
    if((e=uhm_chop32(data,size,&cursor,&backgroundColor))<0) return NULL;
    for(int i = 0; i < width * height; i++) ((uint32_t*)output_data)[i] = backgroundColor;

    uhm_instruction instruction = {0};
    while(cursor < size){
        instruction = {0};
        if((e=uhm_parse_instruction(data,size,&cursor,&instruction))<0) {
            UHM_FREE(output_data);
            if(instruction.data) UHM_FREE(instruction.data);
            if(instruction.opcode == 'T') UHM_FREE(((uhm_tiledPattern*)instruction.data)->instructions.items);
            return NULL;
        }

        if(!instruction.skip_draw){
            if((e=uhm_draw_instruction(&instruction,width,height,output_data, 0, 0, 0))<0) {
                UHM_FREE(output_data);
                if(instruction.data) UHM_FREE(instruction.data);
                if(instruction.opcode == 'T') UHM_FREE(((uhm_tiledPattern*)instruction.data)->instructions.items);
                return NULL;
            }
        }

        if(instruction.data) UHM_FREE(instruction.data);
    }

    return output_data;
}

#endif

#endif