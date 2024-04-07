#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdint>
#include <entities/ClicableEntity.h>
#include <entities/Pin.h>
#include <Color.h>

#pragma pack(push, 1)
struct BMPHeader
{
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
struct pixel
{
    float b, g, r;
};

#pragma pack(pop)

class BMPImage : public ClicableEntity
{
private:
    BMPHeader header;
    struct pixel **pixels;

    Pin movePin;
    Pin rotationPin;
    Pin deletePin;

    double rotation;

    BMPImage() : ClicableEntity({0, 0}, {0, 0}),
                 movePin({0, 0}),
                 rotationPin({0, 0}),
                 deletePin({0, 0})
    {
        movePin.setColor(BLUE);
        movePin.setHoverColor(LIGTH_BLUE);
        deletePin.setColor(RED);
        deletePin.setHoverColor(Color::fromRGB(0xaa, 0x00, 0x00));
    }
    ~BMPImage()
    {
        for (int x = 0; x < header.height; ++x)
            delete[] pixels[x];
        delete[] pixels;
    }

public:
    virtual void onMouseEnter(){}; // Not mandatory to be implemented
    virtual void onMouseExit(){};  // Not mandatory to be implemented
    virtual void onClick(){};      // Not mandatory to be implemented
    virtual void onRelease(){};    // Not mandatory to be implemented

    virtual int tick(EngineState *state, double delta);
    virtual void render();

    virtual bool checkColision(int x, int y);

    static BMPImage *readBMP(const char *filename);
};

#endif