#include <cmath>
#include <windows.h>
#include "main.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext, u32 width, u32 height)
    :wh{ winHandle }, dc{ deviceContext }, mWidth{ width }, mHeight{ height }
{
    mBgColor = 0xFF000000;

    mBuffer = new u32[mWidth * mHeight];
    mDepthBuffer = new float[mWidth * mHeight];
}

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext)
    :wh{ winHandle }, dc{ deviceContext }
{
    RECT rect = {};

    Assert(GetClientRect(winHandle, &rect));

    mWidth = rect.right - rect.left;
    mHeight = rect.bottom - rect.top;

    mBgColor = 0xFF000000;

    mBuffer = new u32[mWidth * mHeight];
    mDepthBuffer = new float[mWidth * mHeight];
}

void FrameBuffer::Fill(u32 color = 0)
{
    if (color != 0)
        mBgColor = color;

    for (u32 y = 0; y < mHeight; ++y)
        for (u32 x = 0; x < mWidth; ++x)
        {
            u32 i = y * mWidth + x;
            mBuffer[i] = color;
            mDepthBuffer[i] = FLT_MAX;
        }
}

V2 FrameBuffer::ProjectPoint(V3 &point)
{
    V2 result = point.xy / point.z;

    result = 0.5f * (result + 1.0f) * V2(mWidth, mHeight);

    return result;
}

u32 FrameBuffer::GetPixel(u32 x, u32 y)
{
    u32 i = y * mWidth + x;

    return mBuffer[i];
}

void FrameBuffer::SetPixel(u32 x, u32 y, u32 color)
{
    u32 i = y * mWidth + x;

    mBuffer[i] = color;
}

void FrameBuffer::SetPixel(V2 pos, u32 color)
{
    u32 i = u32(pos.y) * mWidth + (pos.x);

    mBuffer[i] = color;
}

float FrameBuffer::GetDepth(u32 x, u32 y)
{
    u32 i = y * mWidth + x;

    return mDepthBuffer[i];
}

void FrameBuffer::DrawTriangle(V3* triPixels, V3* colors)
{
    // pojecting 3D points of triangle's corners into 2D points
    V2 pointA = ProjectPoint(triPixels[0]);
    V2 pointB = ProjectPoint(triPixels[1]);
    V2 pointC = ProjectPoint(triPixels[2]);

    // calculating min/max 2D coordnates of the triangle points
    u32 minX = (u32)min(max(min(min(pointA.x, pointB.x), pointC.x), 0), mWidth - 1);
    u32 minY = (u32)min(max(min(min(pointA.y, pointB.y), pointC.y), 0), mHeight - 1);
    u32 maxX = (u32)max(min(round(max(max(pointA.x, pointB.x), pointC.x)), mWidth - 1), 0);
    u32 maxY = (u32)max(min(round(max(max(pointA.y, pointB.y), pointC.y)), mHeight - 1), 0);

    // get vectors of edges
    V2 edge0 = pointB - pointA;
    V2 edge1 = pointC - pointB;
    V2 edge2 = pointA - pointC;

    // check if an edge's vector points from left to right
    bool isTopLeft0 = (edge0.x >= 0.0f && edge0.y > 0.0f) || (edge0.x > 0.0f && edge0.y == 0.0f);
    bool isTopLeft1 = (edge1.x >= 0.0f && edge1.y > 0.0f) || (edge1.x > 0.0f && edge1.y == 0.0f);
    bool isTopLeft2 = (edge2.x >= 0.0f && edge2.y > 0.0f) || (edge2.x > 0.0f && edge2.y == 0.0f);

    // calculate bary-centric value as (B - A)x(C - A)
    float baryCentricDiv = edge0.Cross(pointC - pointA);
    constexpr u32 alpha = 0xFF << 24;

    for (u32 y = minY; y < maxY; ++y)
        for (u32 x = minX; x < maxX; ++x)
        {
            V2 p = V2(x, y) + 0.5f;

            // get the vectors from the triangle's corners to the point
            V2 pEdge0 = p - pointA;
            V2 pEdge1 = p - pointB;
            V2 pEdge2 = p - pointC;

            float cross0 = pEdge0.Cross(edge0);
            float cross1 = pEdge1.Cross(edge1);
            float cross2 = pEdge2.Cross(edge2);

            // check if the point is inside or on an edge of the triangle
            if ((cross0 > 0.0f || (isTopLeft0 && cross0 == 0.0f)) &&
                (cross1 > 0.0f || (isTopLeft1 && cross1 == 0.0f)) &&
                (cross2 > 0.0f || (isTopLeft2 && cross2 == 0.0f)))
            {
                float t0 = -cross1 / baryCentricDiv;
                float t1 = -cross2 / baryCentricDiv;
                float t2 = -cross0 / baryCentricDiv;

                // perspective z-interpolation
                float depth = 1.0f / (t0 / triPixels[0].z + t1 / triPixels[1].z + t2 / triPixels[2].z);

                u32 i = y * mWidth + x;

                if (depth < mDepthBuffer[i])
                {
                    // get interpolated color based on the colors at the triangle's corners
                    V3 color3d = (t0 * colors[0] + t1 * colors[1] + t2 * colors[2]) * 255.0f;
                    u32 color = alpha | ((u32)color3d.r << 16) | ((u32)color3d.g << 8) | (u32)color3d.b;

                    mBuffer[i] = color;
                    mDepthBuffer[i] = depth;
                }
            }
        }
}

void FrameBuffer::Render()
{
    RECT rect = {};

    Assert(GetClientRect(wh, &rect));

    u32 clientWidth = rect.right - rect.left;
    u32 clientHeight = rect.bottom - rect.top;

    BITMAPINFO BitmapInfo = {};
    BitmapInfo.bmiHeader.biSize = sizeof(tagBITMAPINFOHEADER);
    BitmapInfo.bmiHeader.biWidth = mWidth;
    BitmapInfo.bmiHeader.biHeight = mHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    Assert(StretchDIBits(dc, 0, 0, clientWidth, clientHeight, 0, 0, mWidth, mHeight, mBuffer, &BitmapInfo, DIB_RGB_COLORS, SRCCOPY));
}

const u32 FrameBuffer::Width() const
{
    return mWidth;
}

const u32 FrameBuffer::Height() const
{
    return mHeight;
}

FrameBuffer::~FrameBuffer()
{
    delete[] mBuffer;
    delete[] mDepthBuffer;
}

