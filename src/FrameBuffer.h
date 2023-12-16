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
    V2 ProjectPoint(V3 &point);
    inline u32 GetPixel(u32 x, u32 y);
    inline void SetPixel(u32 x, u32 y, u32 color);
    inline void SetPixel(V2 pos, u32 color);
    inline float GetDepth(u32 x, u32 y);
    void DrawTriangle(V3 &vertex0, V3 &vertex1, V3 &vertex2,
        V3 &color0, V3 &color1, V3 &color2, M4 &transform);
    void Render();

private:
    HWND wh;
    HDC dc;
    u32 mBgColor;
    u32 mWidth;
    u32 mHeight;
    u32* mBuffer;
    float* mDepthBuffer;
};
