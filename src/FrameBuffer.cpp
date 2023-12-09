#include <windows.h>
#include "main.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext)
{
    wh = winHandle;
    dc = deviceContext;

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

    mWidth = width;
    mHeight = height;

    mBuffer = new u32[mWidth * mHeight];
}

void FrameBuffer::Fill(u32 color = 0)
{
    for (u32 y = 0; y < mHeight; ++y)
        for (u32 x = 0; x < mWidth; ++x)
        {
            u32 i = y * mWidth + x;

            mBuffer[i] = color;
        }
}

void FrameBuffer::SetPixel(V2 pos, u32 color)
{
    u32 i = u32(pos.y) * mWidth + (pos.x);

    mBuffer[i] = color;
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

