#pragma once
#include "Texture.h"

class FrameBuffer
{
public:
    FrameBuffer(HWND winHandle, HDC deviceContext);
    FrameBuffer(HWND winHandle, HDC deviceContext, u32 width, u32 height);
    ~FrameBuffer();
    const u32 Width() const;
    const u32 Height() const;
    void Fill(u32 color);
    V2 NdcToPixels(V2 &ndcPos);
    inline u32 GetPixel(u32 x, u32 y);
    inline void SetPixel(u32 x, u32 y, u32 color);
    inline void SetPixel(V2 pos, u32 color);
    inline float GetDepth(u32 x, u32 y);
    void DrawTriangle(V3 &vertex0, V3 &vertex1, V3 &vertex2,
        V2 &modelUv0, V2 &modelUv1, V2 &modelUv2, M4 &transform, Texture &texture);
    void Render(u32 width = 0, u32 height = 0);

private:
    HWND wh;
    HDC dc;
    u32 mBgColor;
    u32 mWidth;
    u32 mHeight;
    u32* mBuffer;
    float* mDepthBuffer;
};
