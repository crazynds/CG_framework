#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <entities/FocusedEntity.h>
#include <entities/Pin.h>
#include <vector>
#include <map>

#define MAX_X_GRAPH 512 * 3

class Graph : public FocusedEntity
{
private:
    Pin closePin;
    Vector2d boxSize;
    std::vector<std::pair<Color, std::map<int, int>>> lines;
    Entity *lastEntity = 0;

    int minX = 0, maxX = 0, minY = 0, maxY = 0;

public:
    Graph(Vector2d _position, Vector2d _box) : FocusedEntity(_position),
                                               closePin(_position + _box)
    {
        closePin.setColor(RED);
        boxSize = _box;
        closePin.setHoverColor(Color::fromRGB(0xaa, 0x00, 0x00));
    }

    void addLine(std::map<int, int> data, Color c);

    virtual int tick(EngineState *state, double delta);
    virtual void render();
};

#endif