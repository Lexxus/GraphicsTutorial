#pragma once

class Texture
{
public:
    Texture(unsigned int w, unsigned int h);
    ~Texture();

    unsigned int width();
    unsigned int height();
    unsigned int getTexel(unsigned int x, unsigned int y);
    void setTexel(unsigned int x, unsigned int y, unsigned int color);

private:
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int* mTexels;
};
