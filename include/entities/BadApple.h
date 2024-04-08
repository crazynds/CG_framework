#ifndef __BAD_APPLE_H__
#define __BAD_APPLE_H__

#include <entities/Image.h>
#include <iostream>
#include <fstream>
#include <cstdint>

#define BAD_APPLE_FRAMES 6562
#define IMAGE_SIZE 320
#define IMAGE_SPEED 30.1

class BadApple : public BMPImage
{
private:
    uint8_t **frames;

    int frame = 0, max_frames = BAD_APPLE_FRAMES;
    int ignorePixels;
    double deltaSum = 0;

public:
    BadApple() : BMPImage()
    {
        frames = new uint8_t *[BAD_APPLE_FRAMES];
        char buffer[256];
        pixels = nullptr;
        for (int x = 0; x < BAD_APPLE_FRAMES; x++)
        {
            sprintf(buffer, "./image_sequence2/bad_apple_%03d.bmp", x + 1);
            std::ifstream image_file(buffer, std::ios::binary);

            if (!image_file)
            {
                exit(1);
            }

            image_file.read((char *)(&this->header), sizeof(BMPHeader));
            if (this->header.signature != 0x4D42)
            {

                exit(1);
            }

            // Saltar para o início do bloco de dados da imagem (offset 54 bytes)
            frames[x] = new uint8_t[this->header.imageSize];
            image_file.seekg(this->header.dataOffset);
            image_file.read((char *)frames[x], this->header.imageSize);
            image_file.close();
            printf("Reading frame %d\n", x + 1);
        }
        boxSize.x = header.width;
        boxSize.y = header.height;
        if (this->header.imageSize != 0)
        {
            this->header.width = (this->header.imageSize * 8) / this->header.height;
        }
        ignorePixels = this->header.width - boxSize.x;
        this->header.width = boxSize.x;
        position = CENTER;
        Vector2d rotationVec = Vector2d(cos(this->rotation), sin(this->rotation));
        Vector2d rotationVec2 = Vector2d(-sin(this->rotation), cos(this->rotation));

        movePin.setPosition(position);
        rotationPin.setPosition(position + rotationVec * boxSize.x);
        deletePin.setPosition(position + rotationVec * boxSize.x + rotationVec2 * boxSize.y);
        flipXPin.setPosition(position + rotationVec * (boxSize.x / 2));
        flipYPin.setPosition(position + rotationVec2 * (boxSize.y / 2));
        const char *command = "start bad_apple.wav";

        int status = system(command);
    }

    virtual int tick(EngineState *state, double delta)
    {
        const double offset = 1;
        deltaSum += delta;

        if (deltaSum < offset)
            frame = 0;
        else
            frame = (int)((deltaSum - offset) * IMAGE_SPEED);
        frame %= max_frames;
        return BMPImage::tick(state, delta);
    }
    virtual void drawImage(Vector2f baseX, Vector2f baseY)
    {
        int color = 1;
        CV::color(color, color, color);
        Vector2f posRow = Vector2f(this->position.x, this->position.y);
        Vector2f posNextPixel = posRow;
        int pixel = header.width + ignorePixels;
        printf("Frame: %d\n", frame);
        for (int y = 0; y < header.height - 2; y++)
        {
            posNextPixel = posRow;
            for (int x = 0; x < header.width; x++)
            {
                int byte_index = (pixel) / 8;
                int bit_index = 7 - (pixel % 8);

                int pixel_value = (frames[frame][byte_index] >> bit_index) & 1;
                if (pixel_value != color)
                {
                    color = pixel_value;
                    CV::color(color, color, color);
                }
                CV::point(posNextPixel);
                posNextPixel += baseX;
                pixel++;
            }
            pixel += ignorePixels;
            posNextPixel += baseY;
            posRow += baseY;
        }
    }
};

#endif