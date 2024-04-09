#include <entities/Graph.h>
#include <cmath>
#include <entities/Image.h>

void Graph::addLine(std::map<int, int> data, Color c)
{
    this->maxX = std::max(this->maxX, data.rbegin()->first);
    for (auto it = data.begin(); it != data.end(); it++)
    {
        this->maxY = std::max(this->maxY, it->second);
    }
    this->minY = 0;
    this->minX = 0;
    lines.push_back({c, data});
}

int Graph::tick(EngineState *state, double delta)
{
    Entity *foc = state->getFocusedEntity();
    if (foc != nullptr && foc != this->lastEntity)
    {
        char buffer[256];
        foc->entityName(buffer);
        if (strcmp(buffer, "BMPImage") == 0)
        {
            lines.clear();
            BMPImage *img = (BMPImage *)(foc);
            minX = 0, maxX = 0, minY = 0, maxY = 0;
            img->plotHistogram(this);
            lastEntity = foc;
        }
    }
    this->closePin.tick(state, delta);
    if (this->closePin.isClicked())
        return TICK_KILL_MYSELF;
    return isFocused() ? TICK_TO_FRONT : TICK_OK;
}
void Graph::render()
{
    CV::color(WHITE);
    CV::rectFill(position, position + boxSize);
    CV::color(BLACK);
    CV::rect(position, position + boxSize);

    Vector2d startGraph = position + Vector2d(15, 20);
    Vector2d endGraph = position + boxSize - Vector2d(15, 15);

    CV::line(startGraph, {startGraph.x, endGraph.y});
    CV::line(startGraph, {endGraph.x, startGraph.y});
    CV::textf(startGraph + Vector2d(-10, -10), "0");
    CV::textf(startGraph + Vector2d(boxSize.x - 45, -18), "%d", maxX);
    CV::textf(startGraph + Vector2d(5, boxSize.y - 35), "%d", maxY);

    for (auto it = lines.begin(); it != lines.end(); it++)
    {
        Vector2d last = startGraph;
        CV::color(it->first);
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            double nx = it2->first * (endGraph.x - startGraph.x) / (maxX - minX) + startGraph.x;
            double ny = it2->second * (endGraph.y - startGraph.y) / (maxY - minY) + startGraph.y;
            CV::line(last, {nx, ny});
            last = {nx, ny};
        }
    }
    closePin.render();
}