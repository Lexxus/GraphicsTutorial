#pragma once

class FrameBuffer
{
public:
    FrameBuffer(HWND winHandle, HDC deviceContext);
    void Fill(f32 offset);
    void Render();
    ~FrameBuffer();

private:
    HWND wh;
    HDC dc;
    u32 mWidth;
    u32 mHeight;
    u32* mBuffer;
};
