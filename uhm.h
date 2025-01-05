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

/*
    This function takes in data inside uhm's format size for this data also dimensions for output image and creates bitmap with 4 channels
*/
char* uhm_encode(char* data, uint32_t size, uint32_t width, uint32_t height);


#ifndef UHM_MALLOC
#define UHM_MALLOC(sz)        malloc(sz)
#define UHM_REALLOC(p,newsz)  realloc(p,newsz)
#define UHM_FREE(p)           free(p)
#endif

#ifndef UHM_NO_STDIO
#include <stdio.h>

#define UHM_PRINTF(x, ...) printf(x, __VA_ARGS__)
#else
#define UHM_PRINTF(x, ...)
#endif

#ifdef UHM_IMPLEMENTATION

#define IF_ERR if(error != NULL)

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
    if(*cursor + 2 > size){
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
    UHM_PRINTF("Peeked %c\n", peeked);
    if(error) return false;
    return peeked == expected;
}

bool parse_instruction(char* data, uint32_t size, uint32_t width, uint32_t height, uint32_t* cursor, char* output_data){
    bool error = false;
    uint8_t opcode = uhm_chop8(data,size,cursor,&error);
    if(error) return false;

    if(opcode == 'R'){
        float x = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        float y = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        float Rwidth = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        float Rheight = uhm_chopf32(data,size,cursor,&error);
        if(error) return false;
        uint8_t fillType = uhm_chop8(data,size,cursor,&error);
        if(error) return false;
        uint32_t color = uhm_chop32(data,size,cursor,&error);
        if(error) return false;

        int32_t realX = x*width;
        int32_t realY = y*width;
        int32_t realWidth = Rwidth*width;
        int32_t realHeight = Rheight*width;


        UHM_PRINTF("Drawing rectangle x: %d y: %d width: %d height: %d fillType: %c color: %x",realX,realY,realWidth,realHeight,fillType,color);

        for(int32_t i = realY; i < realY+realHeight; i++){
            if(i < 0 || i >= width) continue;
            for(int32_t j = realX; j < realX+realWidth; j++){
                if(j < 0 || j >= height) continue;
                ((uint32_t*)output_data)[i*width + j] = color;
            }
        }
        
    }else{
        return false;
    }

    return true;
}

char* uhm_encode(char* data, uint32_t size, uint32_t width, uint32_t height){
    UHM_PRINTF("%u\n", size);
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

    // setting image background color
    bool error = false;
    uint32_t backgroundColor = uhm_chop32(data,size,&cursor,&error);
    if(error) return NULL;
    for(int i = 0; i < width * height; i++) ((uint32_t*)output_data)[i] = backgroundColor;

    while(cursor < size){
        if(!parse_instruction(data,size,width,height,&cursor,output_data)) {
            UHM_FREE(output_data);
            return NULL;
        }
    }

    return output_data;
}

#endif

#endif