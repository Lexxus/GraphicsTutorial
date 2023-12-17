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
    V2 operator-(V2);
    V2 operator-=(V2);
    V2 operator-(float);
    V2 operator-=(float);
    V2 operator*(V2);
    V2 operator*=(V2);
    V2 operator*(float);
    V2 operator*=(float);
    V2 operator/(float);
    V2 operator/=(float);

    float Cross(V2);

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

    V3 operator=(V3);
    V3 operator-();
    V3 operator+(V3);
    V3 operator+=(V3);
    V3 operator-(V3);
    V3 operator-=(V3);
    V3 operator*(float);
    V3 operator*=(float);
    V3 operator/(float);
    V3 operator/=(float);

    V3 Normalize();

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

        struct
        {
            float r;
            float g;
            float b;
        };

        float e[3];
    };
};

V3 operator*(float, V3);


class V4
{
public:
    V4() :x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
    V4(float x, float y, float z, float w) :x{ x }, y{ y }, z{ z }, w{ w } {}
    V4(V3);

    V4 operator=(V4);
    V4 operator+(V4);
    V4 operator+=(V4);
    V4 operator*(V4);
    V4 operator*=(V4);
    V4 operator*(float);
    V4 operator*=(float);

    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };

        struct
        {
            V3 xyz;
            float _ignored;
        };

        struct
        {
            V2 xy;
            V2 x2y2;
        };

        struct
        {
            float r;
            float g;
            float b;
            float a;
        };

        float e[4];
    };
};


class M4
{
public:
    M4() {};
    // set all elements to a value
    M4(float);
    M4(V4 a[4]);
    // set Translation matrix
    M4(V3);
    // set Scale matrix
    M4(float x, float y, float z);

    static M4 Identity();

    void Clear(float val = 0.0f);
    M4 SetTranslation(V3);
    M4 Rotate(float x, float y, float z);

    M4 operator=(M4);
    M4 operator*(M4);
    M4 operator*=(M4);
    V4 operator*(V4);

    union
    {
        V4 v[4];
        float e[16];
    };
};

#define GRAPHICS_MATH_H
#endif
