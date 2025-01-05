#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define UHM_IMPLEMENTATION
#include <uhm.h>

#include <stdio.h>
#include <vector>
#include <stdint.h>

void add_u8(std::vector<char>& vec, char val){
    vec.push_back(val);
}

void add_u16(std::vector<char>& vec, uint16_t val){
    uint16_t p1 = (val >> 0 * 8);
    uint16_t p2 = (val >> 1 * 8);

    vec.push_back((char)p1);
    vec.push_back((char)p2);
}

void add_u32(std::vector<char>& vec, uint32_t val){
    uint32_t p1 = (val >> 0 * 8);
    uint32_t p2 = (val >> 1 * 8);
    uint32_t p3 = (val >> 2 * 8);
    uint32_t p4 = (val >> 3 * 8);

    vec.push_back((char)p1);
    vec.push_back((char)p2);
    vec.push_back((char)p3);
    vec.push_back((char)p4);
}

void add_u64(std::vector<char>& vec, uint64_t val){
    uint32_t p1 = (val >> 0 * 8);
    uint32_t p2 = (val >> 1 * 8);
    uint32_t p3 = (val >> 2 * 8);
    uint32_t p4 = (val >> 3 * 8);
    uint32_t p5 = (val >> 4 * 8);
    uint32_t p6 = (val >> 5 * 8);
    uint32_t p7 = (val >> 6 * 8);
    uint32_t p8 = (val >> 7 * 8);

    vec.push_back((char)p1);
    vec.push_back((char)p2);
    vec.push_back((char)p3);
    vec.push_back((char)p4);
    vec.push_back((char)p5);
    vec.push_back((char)p6);
    vec.push_back((char)p7);
    vec.push_back((char)p8);
}

void add_f32(std::vector<char>& vec, float val){
    uint32_t p1 = (*((uint32_t*)&val) >> 0 * 8);
    uint32_t p2 = (*((uint32_t*)&val) >> 1 * 8);
    uint32_t p3 = (*((uint32_t*)&val) >> 2 * 8);
    uint32_t p4 = (*((uint32_t*)&val) >> 3 * 8);

    vec.push_back(p1);
    vec.push_back(p2);
    vec.push_back(p3);
    vec.push_back(p4);
}

void add_f64(std::vector<char>& vec, double val){
    uint32_t p1 = (*((uint64_t*)&val) >> 0 * 8);
    uint32_t p2 = (*((uint64_t*)&val) >> 1 * 8);
    uint32_t p3 = (*((uint64_t*)&val) >> 2 * 8);
    uint32_t p4 = (*((uint64_t*)&val) >> 3 * 8);
    uint32_t p5 = (*((uint64_t*)&val) >> 4 * 8);
    uint32_t p6 = (*((uint64_t*)&val) >> 5 * 8);
    uint32_t p7 = (*((uint64_t*)&val) >> 6 * 8);
    uint32_t p8 = (*((uint64_t*)&val) >> 7 * 8);

    vec.push_back((char)p1);
    vec.push_back((char)p2);
    vec.push_back((char)p3);
    vec.push_back((char)p4);
    vec.push_back((char)p5);
    vec.push_back((char)p6);
    vec.push_back((char)p7);
    vec.push_back((char)p8);
}

void add_color(std::vector<char>& vec, uint32_t val){
    uint32_t R = (val >> 0 * 8);
    uint32_t G = (val >> 1 * 8);
    uint32_t B = (val >> 2 * 8);
    uint32_t A = (val >> 3 * 8);

    vec.push_back((char)B);
    vec.push_back((char)G);
    vec.push_back((char)R);
    vec.push_back((char)A);
}

void add_rectangle_filled(std::vector<char>& vec, float x, float y, float w, float h, uint32_t color){
    add_u8(vec,'R');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,w);
    add_f32(vec,h);
    add_u8(vec,'F');
    add_color(vec,color);
}

void add_boilerplate(std::vector<char>& vec, uint32_t backgroundColor){
    add_u8(vec,'U');
    add_u8(vec,'H');
    add_u8(vec,'M');
    add_color(vec,backgroundColor);
}

int main(){

    std::vector<char> uhm_tester = {};
    add_boilerplate(uhm_tester, 0xFF181818);
    add_rectangle_filled(uhm_tester,0.25,0.5,0.1,0.3,0xFFFF0000);

    add_rectangle_filled(uhm_tester,0.75,0.1,0.25,0.25,0xFF00FF00);

    char* data = uhm_encode(uhm_tester.data(),uhm_tester.size(),512,512);
    stbi_write_png("output.png",512,512,4,data,512*4);

    return 0;
}