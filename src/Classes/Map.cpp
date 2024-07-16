
#include <entities/Map.h>
#include <entities/Screen.h>
#include <entities/Ball.h>
#include <entities/Particle.h>
#include <windows.h>
#include <EngineState.h>
#include <set>
#include <stdlib.h>
#include <time.h>

Map::Map() : Entity({0, 0})
{
    maxX = (currentScreenWidth - 180) / BLOCK_SIZE;
    maxY = (currentScreenHeight - 340) / BLOCK_SIZE;
    map = new int *[maxX];
    for (int i = 0; i < maxX; i++)
    {
        map[i] = new int[maxY];
        for (int j = 0; j < maxY; j++)
        {
            map[i][j] = (i == 20 && j > 10) ? 2 : ((i > 20 && j == 10 && i % 2 == 0) ? 2 : 0);
        }
    }

    for (int i = 0; i < maxX; i++)
    {
        map[i][3] = ((i / 3) % 2 == 1) ? -1 : 0;
    }
}

void Map::render()
{
    CV::color(WHITE);
    Vector2d start = {100, 220};
    Vector2d blockSize = Vector2d(BLOCK_SIZE, BLOCK_SIZE);
    int oldseed = rand();
    for (int x = 0; x < maxX; x++)
    {
        for (int y = 0; y < maxY; y++)
        {
            start.y += BLOCK_SIZE;
            if (map[x][y] != 0)
            {
                srand(map[x][y]);
                CV::color(Color::fromRGB((rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1));
                CV::rect(start, start + blockSize);
                CV::color(WHITE);
                if (map[x][y] > 0)
                    CV::textf(start.x + BLOCK_SIZE / 2 - ceil(log10(map[x][y] + 1)) * 3, start.y + BLOCK_SIZE / 2 - 5, "%d", map[x][y]);
                else if (map[x][y] == -2)
                {
                    CV::circleFill(start + (Vector2d){BLOCK_SIZE / 2 - 6, BLOCK_SIZE / 2 + 5}, 6, 10);
                    CV::circleFill(start + (Vector2d){BLOCK_SIZE / 2 + 6, BLOCK_SIZE / 2 + 5}, 6, 10);
                    CV::circleFill(start + (Vector2d){BLOCK_SIZE / 2, BLOCK_SIZE / 2 - 5}, 6, 10);
                }
                else if (map[x][y] == -3)
                {
                    srand(-1);
                    CV::color(Color::fromRGB((rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1));
                    CV::rect(start + (Vector2d){BLOCK_SIZE / 4, BLOCK_SIZE / 4}, start + blockSize - (Vector2d){BLOCK_SIZE / 4, BLOCK_SIZE / 4});
                }
            }
        }
        start.x += BLOCK_SIZE;
        start.y = 220;
    }
    srand(oldseed);
}
int orientation(Vector2d p, Vector2d q, Vector2d r)
{
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0.0)
        return 0;
    return (val > 0.0) ? 1 : 2;
}
bool onSegment(Vector2d p, Vector2d q, Vector2d r)
{
    return (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y));
}

// Função principal para verificar se duas linhas se interceptam
bool doIntersect(Vector2d p1, Vector2d q1, Vector2d p2, Vector2d q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return true;

    // Casos especiais
    // p1, q1 e p2 são colineares e p2 está na linha p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;

    // p1, q1 e q2 são colineares e q2 está na linha p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;

    // p2, q2 e p1 são colineares e p1 está na linha p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;

    // p2, q2 e q1 são colineares e q1 está na linha p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    // Nenhum dos casos acima
    return false;
}

// Função para calcular a nova trajetória da bolinha após colisão
Vector2d reflect(Vector2d velocity, Vector2d normal)
{
    // Fórmula da reflexão: R = V - 2 * (V . N) * N
    return velocity - (normal * 2) * velocity.dot(normal);
}
Vector2d Map::checkColision(EngineState *state, Vector2d ballPos, Vector2d ballVelocity, bool draw)
{
    CV::color(RED);
    int x = ceil(ballPos.x);
    int y = ceil(ballPos.y);
    x -= 100;
    y -= 220;
    x /= BLOCK_SIZE;
    y /= BLOCK_SIZE;
    y -= 1;
    std::set<std::pair<int, int>> colisoes;

    int times = 0;

    bool colided = false;

    if ((x < 0 && x >= maxX && y < 0 && y >= maxY && map[x][y] != 0) || draw)
    {
        // Estou dentro do quadrado ;-; alg me salve!!

        Vector2d first = Vector2d(x * BLOCK_SIZE + 100.0, (y + 1) * BLOCK_SIZE + 220.0);
        first += {BLOCK_SIZE / 2, BLOCK_SIZE / 2};
        if (draw)
            CV::circleFill(first, 2, 4);
        else
        {
            double vel = ballVelocity.size();
            ballVelocity = ballPos - first;
            ballVelocity.normalize();
            ballVelocity *= vel;
        }
    }
    Vector2d colisions[][2] = {
        {{0.08, 0}, {0.92, 0}},
        {{0.08, 1}, {0.92, 1}},
        {{0, 0.08}, {0, 0.92}},
        {{1, 0.08}, {1, 0.92}},
        {{0, 0.08}, {0.08, 0}},
        {{0, 0.92}, {0.08, 1}},
        {{0.92, 0}, {1, 0.08}},
        {{0.92, 1}, {1, 0.92}},
    };
    Vector2d shift = {100, 220};

    do
    {
        colided = false;
        std::pair<Vector2d, Vector2d> checks[] = {
            {{100.0, 100.0}, {100.0, currentScreenHeight - 100.0}},
            {{100.0, currentScreenHeight - 100.0}, {currentScreenWidth - 100.0, currentScreenHeight - 100.0}},
            {{currentScreenWidth - 100.0, 100.0}, {currentScreenWidth - 100.0, currentScreenHeight - 100.0}},
        };
        for (std::pair<Vector2d, Vector2d> check : checks)
        {

            // Verificar interseção
            if (doIntersect(check.first, check.second, ballPos, ballPos + ballVelocity))
            {
                Vector2d line = check.second - check.first;
                Vector2d normal = Vector2d(-line.y, line.x); // Normal da linha
                normal.normalize();
                ballVelocity = reflect(ballVelocity, normal);
            }
        }
        for (int xb = -1; xb <= 1; xb++)
        {
            for (int yb = -1; yb <= 1; yb++)
            {
                if (colided)
                    break;
                int xa = xb + x;
                int ya = yb + y;
                if (xa < 0 || xa >= maxX || ya < 0 || ya >= maxY || map[xa][ya] == 0)
                    continue;

                for (auto colision : colisions)
                {
                    Vector2d first = shift + Vector2d(xa, (ya + 1)) * BLOCK_SIZE + colision[0] * BLOCK_SIZE;
                    Vector2d second = shift + Vector2d(xa, (ya + 1)) * BLOCK_SIZE + colision[1] * BLOCK_SIZE;
                    if (draw)
                    {
                        CV::line(first, second);
                    }
                    else
                    {
                        if (doIntersect(first, second, ballPos, ballPos + ballVelocity))
                        {
                            Vector2d line = second - first;
                            Vector2d normal = Vector2d(-line.y, line.x);
                            normal.normalize();
                            ballVelocity = reflect(ballVelocity, normal);
                            if (map[xa][ya] > 0 || map[xa][ya] < -1)
                                colisoes.insert(std::make_pair(xa, ya));

                            colided = true;
                            break;
                        }
                    }
                }
                if (colided)
                    break;
            }
            if (colided)
                break;
        }
        if (colided == true)
        {
            times += 1;
            if (times > 3)
            {
                break;
            }
        }
    } while (colided);

    if (draw)
        return {0, 0};

    for (std::pair<int, int> check : colisoes)
    {
        switch (map[check.first][check.second])
        {
        case -2:
        { // create 2-4 balls
            map[check.first][check.second] = 0;
            Screen *screen = (Screen *)state->getMainEntity();
            if (screen == nullptr)
                continue;
            Vector2d mov = {0, 20};
            for (int x = 0; x < 2 + (rand() % 4); x++)
            {
                double rotation = 2 * PI * ((rand() % 256) / 256.0);
                Matrix3<double> rot = Matrix3<double>::rotationZ(rotation);
                Vector2d aux = rot * mov;
                Ball *b = new Ball();
                b->moving = true;
                b->setPosition(ballPos);
                b->moviment = aux * (rand() % 20 + 20);
                screen->addEntity(b);
            }
        }
        break;
        case -3:
        { // Create a stone
            map[check.first][check.second] = -1;
        }
        break;
        default:
        {

            map[check.first][check.second] -= 1;
            if (map[check.first][check.second] != 0)
                continue;
            Screen *screen = (Screen *)state->getMainEntity();
            if (screen == nullptr)
                continue;
            Vector2d mov = {0, 17};

            for (int x = 0; x < 13; x++)
            {
                double rotation = 2 * PI * ((rand() % 256) / 256.0);
                Matrix3<double> rot = Matrix3<double>::rotationZ(rotation);
                Vector2d aux = rot * mov;
                screen->addEntity(
                    new Particle(
                        ballPos, aux * (rand() % 10 + 5),
                        Color::fromRGB((rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1, (rand() % 17) * 16 - 1),
                        0.3 + 0.01 * (rand() % 50)));
            }
        }
        }
    }

    return ballVelocity;
}

bool Map::addLayer()
{
    for (int i = 0; i < maxX; i++)
    {
        if (map[i][1] > 0)
            map[i][1] = 0;
        if (map[i][0] > 0)
            map[i][0] = 0;
    }

    // desce um nivel todos os blocos do mapa atual
    for (int i = 0; i < maxX; i++)
    {
        for (int j = 0; j < maxY - 1; j++)
        {
            if ((map[i][j + 1] != -1) && map[i][j] != -1)
                map[i][j] = map[i][j + 1];
        }
    }

    switch (rand() % 12)
    {
    case 0: // dont add anything
        for (int i = 0; i < maxX; i++)
        {
            map[i][maxY - 1] = 0;
        }
        break;
    case 7: // clone last line in top
    case 6:
        break;
    case 1: // add a randoms blocks
    case 8: // add a randoms blocks
    case 9: // add a randoms blocks
        for (int i = 0; i < maxX; i++)
        {
            if (rand() % 12 == 0)
                map[i][maxY - 1] = -2;
            else
                map[i][maxY - 1] = rand() % 4;
        }
        break;
    case 2:  // add a failed line
    case 10: // add a failed line
        for (int i = 0; i < maxX; i++)
        {
            if (rand() % 50 == 0)
                map[i][maxY - 1] = (i % 2 == 0 || i % 3 == 0) ? -3 : 0;
            else
                map[i][maxY - 1] = (i % 2 == 0 || i % 3 == 0) ? (rand() % 8 + 3) : 0;
        }
        break;
    case 3: // failed line base 13
    case 4: // 3x more probability to happen
    case 5:
    case 11: // add a failed line
        for (int i = 0; i < maxX; i++)
        {
            if (rand() % 12 == 0)
                map[i][maxY - 1] = (i % 7 == 0) ? -2 : 0;
            else
                map[i][maxY - 1] = (i % 7 == 0) ? (rand() % 4) : 0;
        }
        break;
    }

    return true;
}
