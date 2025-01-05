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

#define UHM_ASSERT assert

#ifndef UHM_DA_INIT_CAP
#define UHM_DA_INIT_CAP 256
#endif

#ifndef __cplusplus
#   ifndef decltype
#      define decltype(v) void*
#   endif
#endif

#define uhm_append(da, item)                                                             \
    do {                                                                                 \
        if ((da)->count >= (da)->capacity) {                                             \
            (da)->capacity = (da)->capacity == 0 ? UHM_DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = (decltype((da)->items))UHM_REALLOC((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            UHM_ASSERT((da)->items != NULL && "Buy more RAM lol");                       \
        }                                                                                \
                                                                                         \
        (da)->items[(da)->count++] = (item);                                             \
    } while (0)

#ifndef UHM_NO_STDIO
#include <stdio.h>

#define UHM_PRINTF(x, ...) printf(x, __VA_ARGS__)
#else
#define UHM_PRINTF(x, ...)
#endif

#ifdef UHM_IMPLEMENTATION

#define IF_ERR if(error != NULL)

#define IF_ERR2 if(error != NULL && *error)

char uhm_peek(char* data, uint32_t size, uint32_t cursor, bool* error){
    if(cursor + 1 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    return *(data+cursor);
}

uint32_t uhm_chop8(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 1 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint8_t ch = (uint8_t)*(data+*cursor + 0);
    *cursor += 1;

    return ch;
}

uint32_t uhm_chop16(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 2 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint16_t p1 = (uint32_t)((uint8_t)*(data+*cursor + 0));
    uint16_t p2 = (uint32_t)((uint8_t)*(data+*cursor + 1));

    *cursor += 2;

    uint16_t out = (((uint32_t)p1) << 0 * 8) | (((uint32_t)p2) << 1 * 8);
    return out;
}

uint32_t uhm_chop32(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 4 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint32_t p1 = (uint32_t)((uint8_t)*(data+*cursor + 0));
    uint32_t p2 = (uint32_t)((uint8_t)*(data+*cursor + 1));
    uint32_t p3 = (uint32_t)((uint8_t)*(data+*cursor + 2));
    uint32_t p4 = (uint32_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 4;

    uint32_t out = (((uint32_t)p1) << 0 * 8) | (((uint32_t)p2) << 1 * 8) | (((uint32_t)p3) << 2 * 8) | (((uint32_t)p4) << 3 * 8);
    return out;
}

uint64_t uhm_chop64(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 8 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint64_t p1 = (uint64_t)((uint8_t)*(data+*cursor + 0));
    uint64_t p2 = (uint64_t)((uint8_t)*(data+*cursor + 1));
    uint64_t p3 = (uint64_t)((uint8_t)*(data+*cursor + 2));
    uint64_t p4 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p5 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p6 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p7 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p8 = (uint64_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 8;

    uint64_t out = (((uint64_t)p1) << 0 * 8) | (((uint64_t)p2) << 1 * 8) | (((uint64_t)p3) << 2 * 8) | (((uint64_t)p4) << 3 * 8) |  (((uint64_t)p1) << 4 * 8) | (((uint64_t)p2) << 5 * 8) | (((uint64_t)p3) << 6 * 8) | (((uint64_t)p4) << 7 * 8);
    return out;
}

float uhm_chopf32(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 4 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint32_t p1 = (uint32_t)((uint8_t)*(data+*cursor + 0));
    uint32_t p2 = (uint32_t)((uint8_t)*(data+*cursor + 1));
    uint32_t p3 = (uint32_t)((uint8_t)*(data+*cursor + 2));
    uint32_t p4 = (uint32_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 4;

    uint32_t out = (((uint32_t)p1) << 0 * 8) | (((uint32_t)p2) << 1 * 8) | (((uint32_t)p3) << 2 * 8) | (((uint32_t)p4) << 3 * 8);
    return *((float*)&out);
}

double uhm_chopf64(char* data, uint32_t size, uint32_t* cursor, bool* error){
    if(*cursor + 8 > size){
        IF_ERR {
            *error = true;
            return 0;
        }
    }

    uint64_t p1 = (uint64_t)((uint8_t)*(data+*cursor + 0));
    uint64_t p2 = (uint64_t)((uint8_t)*(data+*cursor + 1));
    uint64_t p3 = (uint64_t)((uint8_t)*(data+*cursor + 2));
    uint64_t p4 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p5 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p6 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p7 = (uint64_t)((uint8_t)*(data+*cursor + 3));
    uint64_t p8 = (uint64_t)((uint8_t)*(data+*cursor + 3));

    *cursor += 8;

    uint64_t out = (((uint64_t)p1) << 0 * 8) | (((uint64_t)p2) << 1 * 8) | (((uint64_t)p3) << 2 * 8) | (((uint64_t)p4) << 3 * 8) |  (((uint64_t)p1) << 4 * 8) | (((uint64_t)p2) << 5 * 8) | (((uint64_t)p3) << 6 * 8) | (((uint64_t)p4) << 7 * 8);
    return *((double*)&out);
}

bool uhm_expect(char* data, uint32_t size, uint32_t cursor, char expected){
    bool error = false;
    char peeked = uhm_peek(data, size, cursor,&error);
    if(error) return false;
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

typedef struct {
    float x,y,width,height,px1,py1,px2,py2;
    uint32_t color,color2;
    uint8_t fillType;
} uhm_rectangle;

bool uhm_parse_rectangle(uhm_rectangle* rectangle, char* data, uint32_t size, uint32_t* cursor){
    bool error = false;
    rectangle->x = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    rectangle->y = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    rectangle->width = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    rectangle->height = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    rectangle->fillType = uhm_chop8(data,size,cursor,&error);
    if(error) return false;
        
    if(rectangle->fillType == 'F'){
        rectangle->color = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
    }
    else if(rectangle->fillType == 'L'){
        rectangle->px1 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        rectangle->py1 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        rectangle->px2 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        rectangle->py2 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;

        rectangle->color = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
        rectangle->color2 = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
    }
    else {
        UHM_PRINTF("ParseRectangle: UNKNOWN FILL TYPE\n");
        return false;
    }

    return true;
}

void uhm_draw_rectangle(uhm_rectangle* rectangle,uint32_t width, uint32_t height, char* output_data, float gx, float gy){
    int32_t realX = (rectangle->x+gx)*width;
    int32_t realY = (rectangle->y+gy)*width;
    int32_t realWidth = rectangle->width*width;
    int32_t realHeight = rectangle->height*width;

    UHM_PRINTF("Drawing rectangle x: %d y: %d width: %d height: %d\n",realX,realY,realWidth,realHeight);

    for(int32_t i = realY; i < realY+realHeight; i++){
        if(i < 0 || i >= width) continue;
        for(int32_t j = realX; j < realX+realWidth; j++){
            if(j < 0 || j >= height) continue;

            if(rectangle->fillType == 'L'){
                ((uint32_t*)output_data)[i*width + j] = uhm_linearGetColor(i,j,realX,realY,realWidth,realHeight,rectangle->px1,rectangle->py1,rectangle->px2,rectangle->py2,rectangle->color,rectangle->color2);
            }else{
                ((uint32_t*)output_data)[i*width + j] = rectangle->color;
            }

        }
    }
}

typedef struct {
    float x,y,r,px1,py1,px2,py2;
    uint8_t fillType;
    uint32_t color,color2;
} uhm_circle;

bool uhm_parse_circle(uhm_circle* circle, char* data, uint32_t size, uint32_t* cursor){
    bool error = false;
    circle->x = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    circle->y = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    circle->r = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    circle->fillType = uhm_chop8(data,size,cursor,&error);
    if(error) return false;
        
    if(circle->fillType == 'F'){
        circle->color = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
    }
    else if(circle->fillType == 'L'){
        circle->px1 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        circle->py1 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        circle->px2 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        circle->py2 = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;

        circle->color = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
        circle->color2 = uhm_chop32(data,size,cursor,&error);
        if(error) return false;
    }
    else {
        UHM_PRINTF("ParseCircle: UNKNOWN FILL TYPE\n");
        return false;
    }

    return true;
}

void uhm_draw_circle(uhm_circle* circle, uint32_t width, uint32_t height, char* output_data, float gx, float gy){
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
                    ((uint32_t*)output_data)[i*width + j] = uhm_linearGetColor(i,j,realX - realR,realY - realR,realR*2,realR*2,circle->px1,circle->py1,circle->px2,circle->py2,circle->color,circle->color2);
                }else{
                    ((uint32_t*)output_data)[i*width + j] = circle->color;
                }
            }
        }
    }
}

typedef struct{
    uint8_t opcode;
    void* data;
} uhm_instruction;

bool uhm_parse_instruction(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction);
bool uhm_draw_instruction(uhm_instruction* instruction, uint32_t width, uint32_t height, char* output_data, float gx, float gy);

typedef struct {
    uhm_instruction *items;
    size_t           count;
    size_t           capacity;
} uhm_instructions;

typedef struct{
    float ox,oy;
    uint16_t rows, cols;
    uhm_instructions instructions;
} uhm_tiledPattern;

bool uhm_parse_tiledPattern(uhm_tiledPattern* tiledPattern, char* data, uint32_t size, uint32_t* cursor){
    bool error = false;
    tiledPattern->ox = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    tiledPattern->oy = uhm_chopf32(data,size,cursor,&error);
    if(error) return false;
    tiledPattern->rows = uhm_chop16(data,size,cursor,&error);
    if(error) return false;
    tiledPattern->cols = uhm_chop16(data,size,cursor,&error);
    if(error) return false;
    
    tiledPattern->instructions.capacity = 0;
    tiledPattern->instructions.count = 0;
    tiledPattern->instructions.items = 0;

    while(true){
        if(*cursor == size){
            UHM_PRINTF("end clause wasn't found\n");
            return false;
        }
        uhm_instruction instruction = {0};
        if(!uhm_parse_instruction(data,size,cursor,&instruction)){
            if(instruction.data) UHM_FREE(instruction.data);
            return false;
        }

        if(instruction.opcode == 'E') break;

        uhm_append(&tiledPattern->instructions,instruction);
    }

    return true;
}

void uhm_draw_tiledPattern(uhm_tiledPattern* tiledPattern, uint32_t width, uint32_t height, char* output_data, float gx, float gy){
    for(int i = 0; i < tiledPattern->rows; i++){
        for(int j = 0; j < tiledPattern->cols; j++){
            for(int index = 0; index < tiledPattern->instructions.count; index++){
                uhm_draw_instruction(&tiledPattern->instructions.items[index],width,height,output_data,tiledPattern->ox*j + gx,tiledPattern->oy*i + gy);
            }
        }
    }
}

bool uhm_parse_instruction(char* data, uint32_t size, uint32_t* cursor, uhm_instruction* instruction){
    bool error = false;
    uint8_t opcode = uhm_chop8(data,size,cursor,&error);
    if(error) return false;

    instruction->opcode = opcode;

    if(opcode == 'R'){
        uhm_rectangle rectangle;
        if(!uhm_parse_rectangle(&rectangle, data,size,cursor)) return false;
        instruction->data = UHM_MALLOC(sizeof(uhm_rectangle));
        *(uhm_rectangle*)(instruction->data) = rectangle;
        return true;
    }
    else if(opcode == 'C'){
        uhm_circle circle;
        if(!uhm_parse_circle(&circle, data,size,cursor)) return false;
        instruction->data = UHM_MALLOC(sizeof(uhm_circle));
        *(uhm_circle*)(instruction->data) = circle;
        return true;
    }
    else if(opcode == 'T'){
        uhm_tiledPattern tiledPattern;
        if(!uhm_parse_tiledPattern(&tiledPattern, data,size,cursor)) {
            if(tiledPattern.instructions.items != NULL) UHM_FREE(tiledPattern.instructions.items);
            return false;
        }
        instruction->data = UHM_MALLOC(sizeof(uhm_tiledPattern));
        *(uhm_tiledPattern*)(instruction->data) = tiledPattern;
        return true;
    }
    else if(opcode == 'E'){
        return true;
    }

    UHM_PRINTF("Unknown Opcode\n");
    return false;
};

bool uhm_draw_instruction(uhm_instruction* instruction, uint32_t width, uint32_t height, char* output_data, float gx, float gy){
         if(instruction->opcode == 'R') uhm_draw_rectangle((uhm_rectangle*)instruction->data,width,height,output_data, gx, gy);
    else if(instruction->opcode == 'C') uhm_draw_circle((uhm_circle*)instruction->data,width,height,output_data, gx, gy);
    else if(instruction->opcode == 'T') uhm_draw_tiledPattern((uhm_tiledPattern*)instruction->data,width,height,output_data, gx, gy);
    else{
        UHM_PRINTF("Unknown Opcode\n");
        return false; 
    }
    return true;
}

char* uhm_encode(char* data, uint32_t size, uint32_t width, uint32_t height){
    UHM_PRINTF("Got %u bytes\n", size);
    char* output_data = (char*)UHM_MALLOC(width*height*4);
    uint32_t cursor = 0;

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
    bool error = false;
    uint32_t backgroundColor = uhm_chop32(data,size,&cursor,&error);
    if(error) return NULL;
    for(int i = 0; i < width * height; i++) ((uint32_t*)output_data)[i] = backgroundColor;

    uhm_instruction instruction = {0};
    while(cursor < size){
        if(!uhm_parse_instruction(data,size,&cursor,&instruction)) {
            UHM_FREE(output_data);
            if(instruction.data) UHM_FREE(instruction.data);
            if(instruction.opcode == 'T') UHM_FREE(((uhm_tiledPattern*)instruction.data)->instructions.items);
            return NULL;
        }

        if(!uhm_draw_instruction(&instruction,width,height,output_data, 0, 0)) {
            UHM_FREE(output_data);
            if(instruction.data) UHM_FREE(instruction.data);
            if(instruction.opcode == 'T') UHM_FREE(((uhm_tiledPattern*)instruction.data)->instructions.items);
            return NULL;
        }

        if(instruction.data) UHM_FREE(instruction.data);
    }

    return output_data;
}

#endif

#endif