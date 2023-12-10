#include <windows.h>
#include "main.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext)
{
    wh = winHandle;
    dc = deviceContext;
    mBgColor = 0xFF000000;

    RECT rect = {};

    Assert(GetClientRect(winHandle, &rect));

    mWidth = rect.right - rect.left;
    mHeight = rect.bottom - rect.top;

    mBuffer = new u32[mWidth * mHeight];
}

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext, u32 width, u32 height)
{
    wh = winHandle;
    dc = deviceContext;
    mBgColor = 0xFF000000;

    mWidth = width;
    mHeight = height;

    mBuffer = new u32[mWidth * mHeight];
}

void FrameBuffer::Fill(u32 color = 0)
{
    if (color != 0)
        mBgColor = color;

    for (u32 y = 0; y < mHeight; ++y)
        for (u32 x = 0; x < mWidth; ++x)
        {
            SetPixel(x, y, color);
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

void FrameBuffer::DrawTriangle(V3* triPixels, u32 color)
{
    V2 pointA = ProjectPoint(triPixels[0]);
    V2 pointB = ProjectPoint(triPixels[1]);
    V2 pointC = ProjectPoint(triPixels[2]);

    V2 edge0 = pointB - pointA;
    V2 edge1 = pointC - pointB;
    V2 edge2 = pointA - pointC;

    bool isTopLeft0 = (edge0.x >= 0.0f && edge0.y > 0.0f) || (edge0.x > 0.0f && edge0.y == 0.0f);
    bool isTopLeft1 = (edge1.x >= 0.0f && edge1.y > 0.0f) || (edge1.x > 0.0f && edge1.y == 0.0f);
    bool isTopLeft2 = (edge2.x >= 0.0f && edge2.y > 0.0f) || (edge2.x > 0.0f && edge2.y == 0.0f);

    for (u32 y = 0; y < mHeight; ++y)
        for (u32 x = 0; x < mWidth; ++x)
            if (GetPixel(x, y) == mBgColor)
            {
                V2 p = V2(x, y) + 0.5f;

                V2 pEdge0 = p - pointA;
                V2 pEdge1 = p - pointB;
                V2 pEdge2 = p - pointC;

                float cross0 = pEdge0.Cross(edge0);
                float cross1 = pEdge1.Cross(edge1);
                float cross2 = pEdge2.Cross(edge2);

                if ((cross0 > 0.0f || (isTopLeft0 && cross0 == 0.0f)) &&
                    (cross1 > 0.0f || (isTopLeft1 && cross1 == 0.0f)) &&
                    (cross2 > 0.0f || (isTopLeft2 && cross2 == 0.0f)))
                {
                    SetPixel(x, y, color);
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
}

