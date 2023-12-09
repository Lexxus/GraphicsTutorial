#pragma once
#if !defined(GRAPHICS_MATH_H)

class V2
{
public:
    V2() :x{ 0 }, y{ 0 } {}
    V2(float x, float y) :x{ x }, y{ y } {}
    V2(float a) :x{ a }, y{ a } {}
    V2(unsigned int x, unsigned int y) :x{ (float)x }, y{ (float)y } {}

    V2 operator=(V2);
    V2 operator+(V2);
    V2 operator+=(V2);
    V2 operator+(float);
    V2 operator+=(float);
    V2 operator*(V2);
    V2 operator*=(V2);
    V2 operator*(float);
    V2 operator*=(float);
    V2 operator/(float);

    union
    {
        struct
        {
            float x;
            float y;
        };

        float e[2];
    };
};

V2 operator*(float n, V2 a);


class V3
{
public:
    V3() :x{ 0 }, y{ 0 }, z{ 0 } {}
    V3(float x, float y, float z) :x{x}, y{y}, z{z} {}

    V3 operator+(V3);

    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };

        struct
        {
            V2 xy;
            float _ignored;
        };

        float e[3];
    };
};

#define GRAPHICS_MATH_H
#endif
