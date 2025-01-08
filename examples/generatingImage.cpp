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

void add_rectangle_linearGradient(std::vector<char>& vec, float x, float y, float w, float h, float px1, float py1, float px2, float py2, uint32_t color1, uint32_t color2){
    add_u8(vec,'R');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,w);
    add_f32(vec,h);
    add_u8(vec,'L');
    add_f32(vec,px1);
    add_f32(vec,py1);
    add_f32(vec,px2);
    add_f32(vec,py2);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_rectangle_circularGradient(std::vector<char>& vec, float x, float y, float w, float h, float cx, float cy, float radius, uint32_t color1, uint32_t color2){
    add_u8(vec,'R');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,w);
    add_f32(vec,h);
    add_u8(vec,'C');
    add_f32(vec,cx);
    add_f32(vec,cy);
    add_f32(vec,radius);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_circle_filled(std::vector<char>& vec, float x, float y, float r, uint32_t color){
    add_u8(vec,'C');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,r);
    add_u8(vec,'F');
    add_color(vec,color);
}

void add_circle_linearGradient(std::vector<char>& vec, float x, float y, float r, float px1, float py1, float px2, float py2, uint32_t color1, uint32_t color2){
    add_u8(vec,'C');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,r);
    add_u8(vec,'L');
    add_f32(vec,px1);
    add_f32(vec,py1);
    add_f32(vec,px2);
    add_f32(vec,py2);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_circle_circularGradient(std::vector<char>& vec, float x, float y, float r, float cx, float cy, float radius, uint32_t color1, uint32_t color2){
    add_u8(vec,'C');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,r);
    add_u8(vec,'C');
    add_f32(vec,cx);
    add_f32(vec,cy);
    add_f32(vec,radius);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_ellipse_filled(std::vector<char>& vec, float x, float y, float rw, float rh, uint32_t color){
    add_u8(vec,'E');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,rw);
    add_f32(vec,rh);
    add_u8(vec,'F');
    add_color(vec,color);
}

void add_ellipse_linearGradient(std::vector<char>& vec, float x, float y, float rw, float rh, float px1, float py1, float px2, float py2, uint32_t color1, uint32_t color2){
    add_u8(vec,'E');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,rw);
    add_f32(vec,rh);
    add_u8(vec,'L');
    add_f32(vec,px1);
    add_f32(vec,py1);
    add_f32(vec,px2);
    add_f32(vec,py2);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_ellipse_circularGradient(std::vector<char>& vec, float x, float y, float rw, float rh, float cx, float cy, float radius, uint32_t color1, uint32_t color2){
    add_u8(vec,'E');
    add_f32(vec,x);
    add_f32(vec,y);
    add_f32(vec,rw);
    add_f32(vec,rh);
    add_u8(vec,'C');
    add_f32(vec,cx);
    add_f32(vec,cy);
    add_f32(vec,radius);
    add_color(vec,color1);
    add_color(vec,color2);
}

void add_boilerplate(std::vector<char>& vec, uint32_t backgroundColor){
    add_u8(vec,'U');
    add_u8(vec,'H');
    add_u8(vec,'M');
    add_color(vec,backgroundColor);
}

void add_tiledPattern_startClause(std::vector<char>& vec,float gx, float gy, float ox, float oy, uint16_t rows, uint16_t cols){
    add_u8(vec,'T');
    add_f32(vec,gx);
    add_f32(vec,gy);
    add_f32(vec,ox);
    add_f32(vec,oy);
    add_u16(vec,rows);
    add_u16(vec,cols);
}

void add_definePattern_startClause(std::vector<char>& vec, uint16_t patternID){
    add_u8(vec, 'P');
    add_u8(vec, 'R');
    add_u16(vec, patternID);
}

void add_endClause(std::vector<char>& vec){
    add_u8(vec,']');
}

void add_rotateModifier(std::vector<char>& vec, float radians){
    add_u8(vec,'|');
    add_f32(vec,radians);
}

void add_scaleModifier(std::vector<char>& vec, float scale){
    add_u8(vec,'\\');
    add_f32(vec,scale);
}

void add_placePattern(std::vector<char>& vec, uint16_t patternID, float x, float y){
    add_u8(vec, 'P');
    add_u8(vec, 'P');
    add_u16(vec, patternID);
    add_f32(vec, x);
    add_f32(vec, y);
}

int main(){
    uint32_t width = 512;
    uint32_t height = 512;
    std::vector<char> uhm_tester = {};
    add_boilerplate(uhm_tester, 0xFF181818);

    
    // Example 1 - Pure Shapes
    add_rectangle_filled(uhm_tester,0.25,0.5,0.1,0.3,0xFFFF0000);
    add_circle_filled(uhm_tester,0.25,0.25,0.3,0xFF00FF00);
    add_rectangle_linearGradient(uhm_tester,0.75,0.75,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFF00FF,0xFF00FF00);
    add_rectangle_circularGradient(uhm_tester,0.0,0.75,0.2,0.1,0.5,0.5,0.5,0xFFFF00FF,0xFF00FF00);
    add_circle_linearGradient(
        uhm_tester,
        0.5,0.5,0.3,
        0.5,0.0,
        0.5,1.0,
        0xFFFF00FF,0xFF00FFFF
    );
    add_circle_circularGradient(
        uhm_tester,
        0.75,0.75,0.2,
        0.5,0.7,
        0.5,
        0xFFFF00FF,0xFF00FFFF
    );
    add_ellipse_linearGradient(
        uhm_tester,
        0.1,0.1,
        0.1,0.2,
        0.5,0.0,
        0.5,1.0,
        0xFFFF00FF,0xFF00FFFF
    );

    // Example 2 - Tiled Patterns
    add_tiledPattern_startClause(uhm_tester,0.0,0.0,0.5,0.5,2,2);
        add_tiledPattern_startClause(uhm_tester,0.0,0.0,0.25,0.25,2,2);
            add_rectangle_linearGradient(uhm_tester,0,0,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFF00FF,0xFF00FF00);
        add_endClause(uhm_tester);
        add_circle_linearGradient(
            uhm_tester,
            0.25/2+0.1/2,0.25/2+0.1/2,0.1,
            0.5,0.0,
            0.5,1.0,
            0xFFFF00FF,0xFF00FFFF
        );
    add_endClause(uhm_tester);

    // Example 3 - Patterns
    // add_definePattern_startClause(uhm_tester,69);
    //     add_rectangle_linearGradient(uhm_tester,0,0,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFFFF00,0xFF00FF00);
    //     add_circle_linearGradient(uhm_tester,0.05,0.05,0.05,0.5,0.0,0.5,1.0,0xFFFF00FF,0xFF00FFFF);
    // add_endClause(uhm_tester);
    // add_tiledPattern_startClause(uhm_tester,0.5,0.5,2,2);
    //     add_placePattern(uhm_tester,69,0.2,0.2);
    // add_endClause(uhm_tester);

    // Example 4 - Rotate Modifier
    add_definePattern_startClause(uhm_tester,69);
        add_circle_circularGradient(
            uhm_tester,
            0.0,0.0,
            0.1,
            0.5,0.5,
            0.5,
            0xFFFF00FF,0xFF00FF00
        );

        add_rotateModifier(uhm_tester,UHM_PI/4);
        add_rectangle_linearGradient(
            uhm_tester,
            0,-0.15,
            0.2,0.2,
            0.0,0.0,
            1.0,1.0,
            0xFFFF00FF,
            0xFFFFFF00
        );
    add_endClause(uhm_tester);

    add_rotateModifier(uhm_tester,-UHM_PI/2);
    add_placePattern(uhm_tester,69,0.25,0.25);

    add_rotateModifier(uhm_tester,UHM_PI/8);
    add_placePattern(uhm_tester,69,0.25,0.65);

    add_rotateModifier(uhm_tester,UHM_PI+UHM_PI/8);
    add_placePattern(uhm_tester,69,0.75,0.75);

    add_rotateModifier(uhm_tester,UHM_PI/4);
    add_ellipse_linearGradient(
        uhm_tester,
        0.75,0.5,
        0.1,0.2,
        0.5,0.0,
        0.5,1.0,
        0xFFFF00FF,0xFF00FFFF
    );

    add_rotateModifier(uhm_tester,UHM_PI/8);
    add_circle_linearGradient(
        uhm_tester,
        0.5,0.5,
        0.1,
        0.5,0.0,
        0.5,1.0,
        0xFFFF00FF,0xFF00FFFF
    );

    add_rotateModifier(uhm_tester,UHM_PI/8);
    add_tiledPattern_startClause(uhm_tester,0,0,0.25,0.25,2,2);
        add_rectangle_linearGradient(uhm_tester,0.1/2,0.1/2,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFF00FF,0xFF00FF00);
    add_endClause(uhm_tester);


    // Example 5 - Scale Modifier
    add_scaleModifier(uhm_tester, 1.0);
    add_ellipse_filled(uhm_tester,0.5,0.5,0.25/2, 0.2,0xFFFF0000);

    // add_definePattern_startClause(uhm_tester,69);
    //     add_circle_circularGradient(
    //         uhm_tester,
    //         0.0,0.0,
    //         0.1,
    //         0.5,0.5,
    //         0.5,
    //         0xFFFF00FF,0xFF00FF00
    //     );

    //     add_rotateModifier(uhm_tester,UHM_PI/4);
    //     add_rectangle_linearGradient(
    //         uhm_tester,
    //         0,-0.15,
    //         0.2,0.2,
    //         0.0,0.0,
    //         1.0,1.0,
    //         0xFFFF00FF,
    //         0xFFFFFF00
    //     );
    // add_endClause(uhm_tester);

    add_scaleModifier(uhm_tester, 2);
    add_placePattern(uhm_tester,69,0.25,0.25);

    add_rotateModifier(uhm_tester,UHM_PI/3);
    add_scaleModifier(uhm_tester, 0.75);
    add_placePattern(uhm_tester,69,0.25,0.65);

    add_rotateModifier(uhm_tester,UHM_PI+UHM_PI/8);
    add_scaleModifier(uhm_tester, 0.5);
    add_placePattern(uhm_tester,69,0.75,0.75);


    add_rotateModifier(uhm_tester,UHM_PI/8);
    add_tiledPattern_startClause(uhm_tester,0.5,0.5,0.25,0.25,2,2);
        add_rectangle_linearGradient(uhm_tester,0.1/2,0.1/2,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFF00FF,0xFF00FF00);
    add_endClause(uhm_tester);

    add_rotateModifier(uhm_tester,UHM_PI/8);
    add_scaleModifier(uhm_tester, 0.5);
    add_tiledPattern_startClause(uhm_tester,0.5,0.5,0.25,0.25,2,2);
        add_rectangle_linearGradient(uhm_tester,0.1/2,0.1/2,0.1,0.1,0.0,0.0,1.0,1.0,0xFFFF00FF,0xFF00FF00);
    add_endClause(uhm_tester);

    char* data = uhm_encode(uhm_tester.data(),uhm_tester.size(),width,height);
    if(data == nullptr){
        printf("an error occured! couldn't generate image\n");
        return 1;
    }
    stbi_write_png("output.png",width,height,4,data,width*4);

    return 0;
}