#ifndef __IMAGE_H__
#define __IMAGE_H__

/**
 * O BPMImage é a classe para renderizar imagens do tipo bitmap.
 * Ela faz o controle da rotação, e dentro dela ela possui outras entidades que ajudam a fazer a gestão dela mesma.
 */

#include <cstdint>
#include <entities/ClicableEntity.h>
#include <entities/Pin.h>
#include <Color.h>
#include <entities/Graph.h>

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
    Pin flipXPin, flipYPin;
    int state = 0;

    double rotation;
    double scale = 1;
    bool flipX = false, flipY = false;

    BMPImage() : ClicableEntity({0, 0}, {0, 0}),
                 movePin({0, 0}),
                 rotationPin({0, 0}),
                 deletePin({0, 0}),
                 flipXPin({0, 0}),
                 flipYPin({0, 0})
    {
        movePin.setColor(BLUE);
        movePin.setHoverColor(LIGTH_BLUE);
        deletePin.setColor(RED);
        deletePin.setHoverColor(Color::fromRGB(0xaa, 0x00, 0x00));
        rotation = 0;
        flipYPin.setColor(YELLOW);
        flipYPin.setHoverColor(LIGTH_YELLOW);
        flipXPin.setColor(YELLOW);
        flipXPin.setHoverColor(LIGTH_YELLOW);
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

    virtual void entityName(char *buffer)
    {
        sprintf(buffer, "BMPImage");
    };

    static BMPImage *readBMP(const char *filename);

    void toggle()
    {
        state += 1;
        state %= 5;
    }

    void plotHistogram(Graph *graph);
};

#endif