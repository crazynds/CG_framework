#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <vector>
#include <entities/Image.h>
#include <Color.h>
#include <gl_canvas2d.h>
#include <cmath>

int BMPImage::tick(EngineState *state, double delta)
{
    bool pinClicked = false, pinSingle;
    int s1 = movePin.tick(state, delta);
    if (movePin.isClicked())
    {
        setPosition(Position::from(movePin)
                        .getPosition());
        state->setFocusedEntity(this);
        pinClicked = true;
    }

    int s2 = rotationPin.tick(state, delta);
    if (rotationPin.isClicked())
    {
        Vector2d pos = rotationPin.getPosition() - position;
        this->rotation = atan2(pos.y, pos.x);
        state->setFocusedEntity(this);

        pinClicked = true;
    }

    deletePin.tick(state, delta);
    if (deletePin.isClicked())
    {
        pinClicked = true;
        return TICK_KILL_MYSELF;
    }

    pinSingle = !flipXPin.isClicked();
    int s4 = flipXPin.tick(state, delta);
    if (pinSingle && flipXPin.isClicked())
    {
        flipX = !flipX;
        state->setFocusedEntity(this);
        pinClicked = true;
    }
    pinClicked |= flipXPin.isClicked();

    pinSingle = !flipYPin.isClicked();
    int s5 = flipYPin.tick(state, delta);
    if (pinSingle && flipYPin.isClicked())
    {
        flipY = !flipY;
        state->setFocusedEntity(this);
        pinClicked = true;
    }
    pinClicked |= flipYPin.isClicked();

    if (pinClicked)
    {
        Vector2d rotationVec = Vector2d(cos(this->rotation), sin(this->rotation));
        Vector2d rotationVec2 = Vector2d(-sin(this->rotation), cos(this->rotation));

        rotationPin.setPosition(position + rotationVec * boxSize.x);
        deletePin.setPosition(position + rotationVec * boxSize.x + rotationVec2 * boxSize.y);
        flipXPin.setPosition(position + rotationVec * (boxSize.x / 2));
        flipYPin.setPosition(position + rotationVec2 * (boxSize.y / 2));
    }

    int s3 = ClicableEntity::tick(state, delta);
    std::vector<int> vec = {s1, s2, s3, s4, s5};
    return *std::max_element(vec.begin(), vec.end());
}

bool BMPImage::checkColision(int x, int y)
{
    Vector2d current(x, y);
    current -= this->position;

    Vector2d baseX = Vector2d(cos(this->rotation), sin(this->rotation));
    Vector2d baseY = Vector2d(-sin(this->rotation), cos(this->rotation));

    double xd = current.x * baseY.x + current.y * baseX.x;
    double yd = current.x * baseY.y + current.y * baseX.y;

    return ClicableEntity::checkColision(xd + this->position.x, yd + this->position.y);
}

void BMPImage::drawImage(Vector2f baseX, Vector2f baseY)
{
    struct pixel current = {0, 0, 0};

    Vector2f posRow = Vector2f(this->position.x, this->position.y);
    Vector2f posNextPixel = posRow;
    for (int y = 0; y < this->header.height; ++y)
    {
        struct pixel *row = this->pixels[flipY ? this->header.height - 1 - y : y];
        posNextPixel = posRow;
        for (int x = 0; x < this->header.width; ++x)
        {
            struct pixel pixel = row[flipX ? this->header.width - 1 - x : x];

            switch (state)
            {
            case 4:
                if (pixel.b != current.b || pixel.g != current.g || pixel.r != current.r)
                {
                    current = pixel;
                    float lum = 0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b;
                    CV::color(lum, lum, lum);
                }
                break;
            case 3:

                if (pixel.b != current.b || pixel.g != current.g || pixel.r != current.r)
                {
                    current = pixel;
                    CV::color(1 - pixel.g, 1, 1 - pixel.g);
                }
                break;
            case 2:

                if (pixel.r != current.r)
                {
                    current = pixel;
                    CV::color(1, 1 - pixel.r, 1 - pixel.r);
                }
                break;
            case 1:

                if (pixel.b != current.b)
                {
                    current = pixel;
                    CV::color(1 - pixel.b, 1 - pixel.b, 1);
                }
                break;
            case 0:
            default:
                if (pixel.b != current.b || pixel.g != current.g || pixel.r != current.r)
                {
                    current = pixel;
                    CV::color(pixel.r, pixel.g, pixel.b);
                }
                break;
            }
            CV::point(posNextPixel);
            posNextPixel += baseX;
        }
        posRow += baseY;
    }
}

void BMPImage::render()
{
    Vector2f baseX = Vector2f(cos(this->rotation), sin(this->rotation));
    Vector2f baseY = Vector2f(-sin(this->rotation), cos(this->rotation));

    this->drawImage(baseX, baseY);

    if (this->isFocused())
    {
        Vector2d distX = Vector2d(baseX.x, baseX.y) * this->header.width;
        Vector2d distY = Vector2d(baseY.x, baseY.y) * this->header.height;
        CV::color(BLUE);
        CV::line(this->position, this->position + distX);
        CV::line(this->position + distX + distY, this->position + distX);
        CV::line(this->position, this->position + distY);
        CV::line(this->position + distY, this->position + distY + distX);
        movePin.render();
        rotationPin.render();
        deletePin.render();
        flipXPin.render();
        flipYPin.render();
    }
}

BMPImage *BMPImage::readBMP(const char *filename)
{
    BMPImage *image = new BMPImage();
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        delete image;
        return nullptr;
    }

    file.read((char *)(&image->header), sizeof(BMPHeader));

    if (image->header.signature != 0x4D42)
    {
        std::cerr << "Not a valid BMP file: " << filename << std::endl;
        delete image;
        return nullptr;
    }
    int oldWidth = image->header.width;
    if (image->header.imageSize != 0)
    {
        image->header.width = (image->header.imageSize / (image->header.bpp / 8)) / image->header.height;
    }
    uint8_t *pixels = new uint8_t[image->header.width * image->header.height * image->header.bpp / 8];
    file.seekg(image->header.dataOffset);
    file.read((char *)pixels, image->header.width * image->header.height * image->header.bpp / 8);
    image->pixels = new struct pixel *[image->header.height];

    for (int y = 0; y < image->header.height; ++y)
    {
        image->pixels[y] = new struct pixel[image->header.width];
        for (int x = 0; x < image->header.width; ++x)
        {
            int index = y * image->header.width + x;
            int pixelIndex = index * 3;

            // BGR to RGB
            int r = pixels[pixelIndex + 2];
            int g = pixels[pixelIndex + 1];
            int b = pixels[pixelIndex];
            Color c = Color::fromRGB(r, g, b);
            image->pixels[y][x].r = c.r;
            image->pixels[y][x].g = c.g;
            image->pixels[y][x].b = c.b;
        }
    }
    delete[] pixels;
    image->header.width = oldWidth;
    BMPHeader &header = image->header;
    image->setBoxSize({(double)header.width, (double)header.height});
    image->setPosition(Position::from(CENTER)
                           .toBottom(image->getBoxSize().y / 2)
                           .toLeft(image->getBoxSize().x / 2)
                           .getPosition());

    image->movePin.setPosition(Position::from(image)
                                   .getPosition());
    image->rotationPin.setPosition(Position::from(image)
                                       .toRight(header.width)
                                       .getPosition());
    image->deletePin.setPosition(Position::from(image)
                                     .toRight(header.width)
                                     .toTop(header.height)
                                     .getPosition());

    image->flipXPin.setPosition(Position::from(image)
                                    .toRight(header.width / 2)
                                    .getPosition());
    image->flipYPin.setPosition(Position::from(image)
                                    .toTop(header.height / 2)
                                    .getPosition());
    return image;
}