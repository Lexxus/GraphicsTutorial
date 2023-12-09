#pragma once

class FrameBuffer
{
public:
    FrameBuffer(HWND winHandle, HDC deviceContext);
    FrameBuffer(HWND winHandle, HDC deviceContext, u32 width, u32 height);
    ~FrameBuffer();
    const u32 Width() const;
    const u32 Height() const;
    void Fill(u32 color);
    void SetPixel(V2 pos, u32 color);
    void Render();

private:
    HWND wh;
    HDC dc;
    u32 mWidth;
    u32 mHeight;
    u32* mBuffer;
};
