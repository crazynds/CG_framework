#include <iostream>
#include <fstream>
#include <cmath>
#include <entities/Image.h>
#include <Color.h>
#include <gl_canvas2d.h>
#include <cmath>

int BMPImage::tick(EngineState *state, double delta)
{
    int s1 = movePin.tick(state, delta);
    if (movePin.isClicked())
    {
        setPosition(Position::from(movePin)
                        .getPosition());
        Vector2d rotationVec = Vector2d(cos(this->rotation), sin(this->rotation));
        Vector2d rotationVec2 = Vector2d(-sin(this->rotation), cos(this->rotation));
        Vector2d newPosition = position + rotationVec * boxSize.x;
        rotationPin.setPosition(newPosition);
        deletePin.setPosition(newPosition + rotationVec2 * boxSize.y);
        state->setFocusedEntity(this);
    }

    int s2 = rotationPin.tick(state, delta);
    if (rotationPin.isClicked())
    {
        Vector2d pos = rotationPin.getPosition() - position;
        this->rotation = atan2(pos.y, pos.x);

        Vector2d rotationVec = Vector2d(cos(this->rotation), sin(this->rotation));
        Vector2d newPosition = position + rotationVec * boxSize.x;
        rotationPin.setPosition(newPosition);

        Vector2d rotationVec2 = Vector2d(-sin(this->rotation), cos(this->rotation));
        deletePin.setPosition(newPosition + rotationVec2 * boxSize.y);

        state->setFocusedEntity(this);
    }
    deletePin.tick(state, delta);
    if (deletePin.isClicked())
    {
        return TICK_KILL_MYSELF;
    }

    int s3 = ClicableEntity::tick(state, delta);

    return std::max(std::max(s1, s2), s3);
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

void BMPImage::render()
{
    struct pixel current = {0, 0, 0};
    Vector2f baseX = Vector2f(cos(this->rotation), sin(this->rotation));
    Vector2f baseY = Vector2f(-sin(this->rotation), cos(this->rotation));

    Vector2f posRow = Vector2f(this->position.x, this->position.y);
    Vector2f posNextPixel = posRow;
    for (int y = 0; y < this->header.height; ++y)
    {
        struct pixel *row = this->pixels[y];
        //        posNextPixel = posRow;
        for (int x = 0; x < this->header.width; ++x)
        {
            struct pixel pixel = row[x];

            if (pixel.b != current.b || pixel.g != current.g || pixel.r != current.r)
            {
                current = pixel;
                CV::color(pixel.r, pixel.g, pixel.b);
            }
            CV::point(posNextPixel);
            posNextPixel += baseX;
        }
        posNextPixel -= baseX * this->header.width;
        posNextPixel += baseY;
        //        posRow += baseY;
    }

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

    uint8_t *pixels = new uint8_t[image->header.width * image->header.height * image->header.bpp / 8];
    file.seekg(image->header.dataOffset);
    file.read((char *)pixels, image->header.imageSize == 0 ? image->header.width * image->header.height * image->header.bpp / 8 : image->header.imageSize);
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
    return image;
}