#include "graphics_math.h"

//
// NOTE: V2
//

V2 V2::operator=(V2 a)
{
    x = a.x;
    y = a.y;

    return *this;
}

V2 V2::operator+(V2 a)
{
    V2 result(x, y);

    result += a;

    return result;
}

V2 V2::operator+=(V2 a)
{
    x += a.x;
    y += a.y;

    return *this;
}

V2 V2::operator+(float n)
{
    V2 result(x, y);

    result += n;

    return result;
}

V2 V2::operator+=(float n)
{
    x += n;
    y += n;

    return *this;
}

V2 V2::operator-(V2 a)
{
    V2 result(x, y);

    result.x -= a.x;
    result.y -= a.y;

    return result;
}

V2 V2::operator-=(V2 a)
{
    x -= a.x;
    y -= a.y;

    return *this;
}

V2 V2::operator-(float n)
{
    V2 result(x, y);

    result -= n;

    return result;
}

V2 V2::operator-=(float n)
{
    x -= n;
    y -= n;

    return *this;
}


V2 V2::operator*(V2 a)
{
    V2 result(x, y);

    result *= a;

    return result;
}

V2 V2::operator*=(V2 a)
{
    x *= a.x;
    y *= a.y;

    return *this;
}

V2 V2::operator*(float n)
{
    V2 result(x, y);

    result *= n;

    return result;
}

V2 V2::operator*=(float n)
{
    x *= n;
    y *= n;

    return *this;
}

V2 operator*(float n, V2 a)
{
    return a * n;
}

V2 V2::operator/(float n)
{
    V2 result;

    result.x = x / n;
    result.y = y / n;

    return result;
}

float V2::Cross(V2 b)
{
    return x * b.y - y * b.x;
}

//
// NOTE: V3
//

V3 V3::operator=(V3 a)
{
    x = a.x;
    y = a.y;
    z = a.z;

    return *this;
}

V3 V3::operator+(V3 a)
{
    V3 result(x, y, z);

    result += a;

    return result;
}

V3 V3::operator+=(V3 a)
{
    x += a.x;
    y += a.y;
    z += a.z;

    return *this;
}
