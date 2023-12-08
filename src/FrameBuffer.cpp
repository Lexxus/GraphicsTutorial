#include <windows.h>
#include "main.h"
#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(HWND winHandle, HDC deviceContext)
{
    wh = winHandle;
    dc = deviceContext;

    RECT rect = {};

    Assert(GetClientRect(winHandle, &rect))

    mWidth = rect.right - rect.left;
    mHeight = rect.bottom - rect.top;
    mBuffer = new u32[mWidth * mHeight];
}

void FrameBuffer::Fill(f32 offset = 0.0f)
{
    for (u32 y = 0; y < mHeight; ++y)
        for (u32 x = 0; x < mWidth; ++x)
        {
            u32 i = y * mWidth + x;
            u8 r = (u8)(x - offset);
            u8 g = (u8)y;
            u8 b = 0;
            u8 alpha = 255;
            u32 pixel = ((u32)alpha << 24) | ((u32)r << 16) | ((u32)g << 8) | (u32)b;

            mBuffer[i] = pixel;
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

FrameBuffer::~FrameBuffer()
{
    delete[] mBuffer;
}

