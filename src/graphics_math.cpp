#include "pch.h"
#include "main.h"
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

V2 V2::operator/(V2 a)
{
    V2 result(x, y);

    result /= a;

    return result;
}

V2 V2::operator/=(V2 a)
{
    x /= a.x;
    y /= a.y;

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

V2 V2::operator/(float n)
{
    V2 result(x, y);

    result /= n;

    return result;
}

V2 V2::operator/=(float n)
{
    x /= n;
    y /= n;

    return *this;
}

V2 operator*(float n, V2 a)
{
    return a * n;
}

float V2::Cross(V2 b)
{
    return x * b.y - y * b.x;
}

//
// NOTE: V3
//

V3 V3::Normalize()
{
    *this /= sqrtf(x * x + y * y + z * z);

    return *this;
}

V3 V3::operator=(V3 a)
{
    x = a.x;
    y = a.y;
    z = a.z;

    return *this;
}

V3 V3::operator-()
{
    V3 result(-x, -y, -z);

    return result;
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

V3 V3::operator-(V3 a)
{
    V3 result(x, y, z);

    result -= a;

    return result;
}

V3 V3::operator-=(V3 a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;

    return *this;
}

V3 V3::operator*(float a)
{
    V3 result(x, y, z);

    result *= a;

    return result;
}

V3 V3::operator*=(float a)
{
    x *= a;
    y *= a;
    z *= a;

    return *this;
}

V3 V3::operator/(float a)
{
    V3 result(x, y, z);

    result /= a;

    return result;
}

V3 V3::operator/=(float a)
{
    x /= a;
    y /= a;
    z /= a;

    return *this;
}


V3 operator*(float n, V3 a)
{
    return a * n;
}

//
// V4
//

V4::V4(V3 v3)
    :xyz{ v3 }
{
    w = 1.0f;
}

V4 V4::operator=(V4 a)
{
    x = a.x;
    y = a.y;
    z = a.z;
    w = a.w;

    return *this;
}

V4 V4::operator+(V4 a)
{
    V4 result(x, y, z, w);

    result += a;

    return result;
}

V4 V4::operator+=(V4 a)
{
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;

    return *this;
}

V4 V4::operator*(V4 a)
{
    V4 result(x, y, z, w);

    result *= a;

    return result;
}

V4 V4::operator*=(V4 a)
{
    x *= a.x;
    y *= a.y;
    z *= a.z;
    w *= a.w;

    return *this;
}

V4 V4::operator*(float n)
{
    V4 result(x, y, z, w);

    result *= n;

    return result;
}

V4 V4::operator*=(float n)
{
    x *= n;
    y *= n;
    z *= n;
    w *= n;

    return *this;
}

//
// M4
//

M4::M4(float a)
{
    Clear(a);
}

M4::M4(V4 a[4])
{
    v[0] = a[0];
    v[1] = a[1];
    v[2] = a[2];
    v[3] = a[3];
}

M4::M4(V3 a)
{
    Clear();
    v[0].x = 1.0f;
    v[1].y = 1.0f;
    v[2].z = 1.0f;
    v[3].xyz = a;
    v[3].w = 1.0f;
}

M4::M4(float x, float y, float z)
{
    Clear();
    v[0].x = x;
    v[1].y = y;
    v[2].z = z;
    v[3].w = 1.0f;
}

M4 M4::Identity()
{
    return M4(1.0f, 1.0f, 1.0f);
}


M4 M4::SetTranslation(V3 a)
{
    v[3].xyz = a;

    return *this;
}

M4 M4::Rotate(float x, float y, float z)
{
    float cosX = cosf(x);
    float sinX = sinf(x);
    float cosY = cosf(y);
    float sinY = sinf(y);
    float cosZ = cosf(z);
    float sinZ = sinf(z);
    M4 m = M4::Identity();
    m.v[1].y = cosX;
    m.v[1].z = sinX;
    m.v[2].y = -sinX;
    m.v[2].z = cosX;

    *this *= m;

    m.v[0].x = cosY;
    m.v[0].z = sinY;
    m.v[1].y = 1.0f;
    m.v[1].z = 0.0f;
    m.v[2].x = -sinY;
    m.v[2].y = 0.0f;
    m.v[2].z = cosY;

    *this *= m;

    m.v[0].x = cosZ;
    m.v[0].y = sinZ;
    m.v[0].z = 0.0f;
    m.v[1].x = -sinZ;
    m.v[1].y = cosZ;
    m.v[2].x = 0.0f;
    m.v[2].z = 1.0f;

    *this *= m;

    return *this;
}

// fov as degree
M4 M4::Perspective(float fov, float aspectRatio, float nearZ, float farZ)
{
    M4 m = { 0.0f };
    float fovRad = (fov / 360.0f) * 2.0f * PI;

    m.v[0].x = 1.0f / (aspectRatio * tan(fovRad * 0.5f));
    m.v[1].y = 1.0f / tan(fovRad * 0.5f);
    m.v[2].z = -farZ / (nearZ - farZ);
    m.v[3].z = (nearZ * farZ) / (nearZ - farZ);
    m.v[2].w = 1.0f;

    return m;
}


M4 M4::operator=(M4 a)
{
    for (int i = 0; i < 16; ++i)
        e[i] = a.e[i];

    return *this;
}

M4 M4::operator*(M4 m)
{
    M4 result = {};

    result.v[0] = *this * m.v[0];
    result.v[1] = *this * m.v[1];
    result.v[2] = *this * m.v[2];
    result.v[3] = *this * m.v[3];

    return result;
}

M4 M4::operator*=(M4 m)
{
    M4 a(v);

    v[0] = a * m.v[0];
    v[1] = a * m.v[1];
    v[2] = a * m.v[2];
    v[3] = a * m.v[3];

    return *this;
}

V4 M4::operator*(V4 a)
{
    V4 result = v[0] * a.x;
    result += v[1] * a.y;
    result += v[2] * a.z;
    result += v[3] * a.w;

    return result;
}

void M4::Clear(float a)
{
    for (int i = 0; i < 16; ++i)
        e[i] = a;
}
