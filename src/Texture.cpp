#include "Texture.h"

Texture::Texture(unsigned int w, unsigned int h)
{
    mWidth = w;
    mHeight = h;
    mTexels = new unsigned int[w * h];
}

Texture::~Texture()
{
    delete mTexels;
}

unsigned int Texture::width()
{
    return mWidth;
}

unsigned int Texture::height()
{
    return mHeight;
}

unsigned int Texture::getTexel(unsigned int x, unsigned int y)
{
    if (x >= mWidth || y >= mHeight) return 0u;
    return mTexels[y * mHeight + x];
}

void Texture::setTexel(unsigned int x, unsigned int y, unsigned int color)
{
    if (x < mWidth && y < mHeight)
    {
        mTexels[y * mHeight + x] = color;
    }
}
