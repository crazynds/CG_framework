#ifndef __COLOR_H__
#define __COLOR_H__


class Color{
public:
    float r, g, b, a;

    Color(float r,float g,float b):r(r),g(g),b(b){
        this->a = 1;
    }
    Color(float r,float g,float b, float a):r(r),g(g),b(b),a(a){}
    Color()
    {
    }

    // Convert from 0-255 to 0-1
    static Color fromRGB(int r, int g, int b)
    {
        return Color::fromRGB(r,g,b,1);
    }
    static Color fromRGB(int r, int g, int b,int a){
        while (r > 255)
        {
            r >>= 1;
        }
        while (g > 255)
        {
            g >>= 1;
        }
        while (b > 255)
        {
            b >>= 1;
        }
        float rf = (r%256);
        float gf = (g%256);
        float bf = (b%256);
        float af = (a%256);
        return Color(rf/255,gf/255,bf/255,af/255);
    }

    bool operator==(const Color &c)
    {
        return this->r == c.r && this->b == c.b && this->g == c.g && this->a == c.a;
    }
    bool operator!=(const Color &c)
    {
        return !(this->operator==(c));
    }
};

const Color BLACK = Color::fromRGB(0,0,0);
const Color WHITE = Color::fromRGB(255,255,255);
const Color RED = Color::fromRGB(255,0,0);
const Color GREEN = Color::fromRGB(0,255,0);
const Color LIGTH_GREEN = Color::fromRGB(0x64,0xD1,0x3E);
const Color BLUE = Color::fromRGB(0,0,255);
const Color LIGTH_BLUE = Color::fromRGB(0x4D,0xC7,0xFD);
const Color YELLOW = Color::fromRGB(0xFA,0xFD,0);
const Color ORANGE = Color::fromRGB(0xF1,0xAB,0);
const Color BROWN = Color::fromRGB(0x76,0x39,0x31);
const Color GREY = Color::fromRGB(0x80,0x80,0x80);
const Color PINK = Color::fromRGB(0xFF,0x0,0xFF);



#endif