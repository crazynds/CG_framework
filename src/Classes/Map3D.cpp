#include <entities/Map3D.h>
#include <iostream>
#include <random>
#include <Matrix3.h>
#define M_PI 3.14159265358979323846
#include <math.h>

double calculateHeight(int x, int y)
{
    double height = std::sin(x * 0.01) * std::cos(y * 0.01);
    return height;
}
float generateRandomFloat(float max)
{
    float x = (float)rand() / ((float)RAND_MAX / max);
    return x;
}

GLfloat ctrlPoints[4][4][3] = {
    {{0, 0, 0}, {1 / 3.0, 0, 0}, {2 / 3.0, 0, 0}, {1, 0, 0}},
    {{0, 1 / 3.0, 0}, {1 / 3.0, 1 / 3.0, 0}, {2 / 3.0, 1 / 3.0, 0}, {1, 1 / 3.0, 0}},
    {{0, 2 / 3.0, 0}, {1 / 3.0, 2 / 3.0, 0}, {2 / 3.0, 2 / 3.0, 0}, {1, 2 / 3.0, 0}},
    {{0, 1, 0}, {1 / 3.0, 1, 0}, {2 / 3.0, 1, 0}, {1, 1, 0}}};
const int CHUNK_SIZE = 32;
const float CHUNK_SPACING = 1;
const int MAP_SIZE = 64;

GLfloat light_0_difuse[] = {1, 1, 1};  // luz branca
GLfloat light_0_ambient[] = {1, 1, 1}; // branco
float mapaAlturas[MAP_SIZE * 3 + 1][MAP_SIZE * 3 + 1];

Map3D::Map3D() : Entity({MAP_SIZE / 2, MAP_SIZE / 2})
{
    this->map = new struct chunk *[MAP_SIZE];
    int height = MAP_SIZE * 3 + 1;
    int width = MAP_SIZE * 3 + 1;
    this->speed = 1;
    this->angle = M_PI / 2;
    this->angleInclination = 0;

    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_difuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_0_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            mapaAlturas[i][j] = generateRandomFloat(0.5);
            // mapaAlturas[i][j] = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 3 == 0 && i > 0 && i < height - 1 && j % 3 == 0 && j > 0 && j < width - 1)
            {
                mapaAlturas[i][j] = (mapaAlturas[i][j] + mapaAlturas[i - 1][j - 1] + mapaAlturas[i - 1][j + 1] + mapaAlturas[i + 1][j - 1] + mapaAlturas[i + 1][j + 1]) / 5;
            }
            else if (i % 3 == 0 && i > 0 && i < height - 1)
            {
                mapaAlturas[i][j] = (mapaAlturas[i][j] + mapaAlturas[i - 1][j] + mapaAlturas[i + 1][j]) / 3;
            }
            else if (j % 3 == 0 && j > 0 && j < width - 1)
            {
                mapaAlturas[i][j] = (mapaAlturas[i][j] + mapaAlturas[i][j - 1] + mapaAlturas[i][j + 1]) / 3;
            }
        }
    }
    for (int i = 0; i < MAP_SIZE; i++)
    {
        this->map[i] = new struct chunk[MAP_SIZE];
        for (int j = 0; j < MAP_SIZE; j++)
        {
            memcpy(this->map[i][j].ctrlPoints, ctrlPoints, sizeof(ctrlPoints));
            // this->map[i][j].ctrlPoints[1][1][2] = generateRandomFloat();
            // this->map[i][j].ctrlPoints[1][2][2] = generateRandomFloat();
            // this->map[i][j].ctrlPoints[2][1][2] = generateRandomFloat();
            // this->map[i][j].ctrlPoints[2][2][2] = generateRandomFloat();
            int idY = i * 3;
            int idX = j * 3;
            for (int x = 0; x < 4; x++)
            {
                for (int y = 0; y < 4; y++)
                {
                    this->map[i][j].ctrlPoints[x][y][2] = mapaAlturas[x + idX][y + idY];
                }
            }
        }
    }
}

int Map3D::tick(EngineState *state, double delta)
{

    auto frente = Matrix3<double>::rotationZ(this->angle) * (Vector2d){this->speed, 0};
    this->position += frente * delta;

    if (state->getKeyboardState('d'))
    {
        if (this->angleInclination < 0.4)
        {
            this->angleInclination += delta / 4;
            this->angle -= delta / 16 * this->speed;
        }
        else
        {
            this->angleInclination += delta / 16;
            this->angle -= delta / 10 * this->speed;
        }
    }
    else if (state->getKeyboardState('a'))
    {
        if (this->angleInclination > -0.4)
        {
            this->angleInclination -= delta / 4;
            this->angle += delta / 16 * this->speed;
        }
        else
        {
            this->angleInclination -= delta / 16;
            this->angle += delta / 10 * this->speed;
        }
    }
    else
    {
        this->angleInclination *= 1 - delta;
    }

    if (state->getKeyboardState('w'))
    {
        this->speed += delta;
    }
    if (state->getKeyboardState('s'))
    {
        this->speed -= delta;
        if (this->speed < 0.6)
            this->speed = 0.6;
    }

    if (state->getKeyboardState('e'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else if (state->getKeyboardState('q'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // if (state->getKeyboardState(' ')) // Espace
    // {
    //     rz = state->getMousePosition().x;
    //     rx = state->getMousePosition().y;
    // }
    float abertura = 20.0;
    float znear = 1;
    float zfar = 23;
    float aspect = 1;
    gluPerspective(abertura, aspect, znear, zfar);

    glMatrixMode(GL_MODELVIEW);

    Vector3d cima = {0, 0, 1};

    cima = Matrix3<double>::rotationX(this->angleInclination) * cima;
    cima = Matrix3<double>::rotationZ(this->angle) * cima;

    glLoadIdentity();
    if (state->getKeyboardState('g'))
    {
        gluLookAt(this->position.x - (frente * 12).x, this->position.y - (frente * 12).y, 2, // from. Posicao onde a camera esta posicionada
                  this->position.x, this->position.y, 1.1,                                   // to. Posicao absoluta onde a camera esta vendo
                  cima.x, cima.y, cima.z);                                                   // up. Vetor Up.
    }
    else
        gluLookAt(this->position.x, this->position.y, 1.0,                                   // from. Posicao onde a camera esta posicionada
                  this->position.x + (frente * 4).x, this->position.y + (frente * 4).y, 0.9, // to. Posicao absoluta onde a camera esta vendo
                  cima.x, cima.y, cima.z);                                                   // up. Vetor Up.

    // glRotatef((GLfloat)rz, 0.0f, 1.0f, 0.0f);
    // glRotatef((GLfloat)rx, 1.0f, 0.0f, 0.0f);

    return TICK_OK;
}

void normalize(GLfloat *v)
{
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (d == 0.0)
        return;
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}
void drawTriangle(GLfloat *a, GLfloat *b, GLfloat *c)
{
    glBegin(GL_TRIANGLES);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glEnd();
}
void subdivide(GLfloat *a, GLfloat *b, GLfloat *c, int depth)
{
    if (depth == 0)
    {
        drawTriangle(a, b, c);
        return;
    }

    GLfloat v1[3], v2[3], v3[3];
    for (int i = 0; i < 3; i++)
    {
        v1[i] = (a[i] + b[i]) / 2.0;
        v2[i] = (a[i] + c[i]) / 2.0;
        v3[i] = (b[i] + c[i]) / 2.0;
    }
    normalize(v1);
    normalize(v2);
    normalize(v3);

    subdivide(a, v1, v2, depth - 1);
    subdivide(b, v1, v3, depth - 1);
    subdivide(c, v2, v3, depth - 1);
    subdivide(v1, v2, v3, depth - 1);
}

void drawTetrahedron()
{
    GLfloat v[4][3] = {
        {0.1, 0.1, 0.1},
        {-0.1, -0.1, 0.1},
        {-0.1, 0.1, -0.1},
        {0.1, -0.1, -0.1}};
    subdivide(v[0], v[1], v[2], 3);
    subdivide(v[0], v[1], v[3], 3);
    subdivide(v[0], v[2], v[3], 3);
    subdivide(v[1], v[2], v[3], 3);
}

void desenhaArvore(int x, int y)
{
    glPushMatrix();
    glTranslatef(x / 3.0, y / 3.0, mapaAlturas[y][x] - 0.02);
    gluCylinder(gluNewQuadric(), 0.02, 0.01, 0.2, 20, 3);
    glScaled(0.07, 0.07, 0.07);
    glTranslatef(0, 0, 3);
    drawTetrahedron();
    glPopMatrix();
}

void Map3D::render()
{

    auto frente = Matrix3<double>::rotationZ(this->angle) * (Vector2d){this->speed, 0};
    GLfloat luz[] = {MAP_SIZE / 2, MAP_SIZE / 2, 6, 1};
    // GLfloat direcao[] = {frente.x * 3, frente.y * 3, -0.3, 1};
    // glLightfv(GL_LIGHT0, GL_POSITION, luz);
    // glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direcao);

    glLightfv(GL_LIGHT0, GL_POSITION, luz);

    glPushMatrix();
    glScalef(2, 2, 2);

    glColor3f(1, 1, 0);

    for (int x = 0; x < MAP_SIZE; ++x)
    {
        for (int y = 0; y < MAP_SIZE; ++y)
        {
            auto dist = pow(abs(this->position.x - (x + 0.5) * 2), 2) + pow(abs(this->position.y - (y + 0.5) * 2), 2);

            if (dist > 16 * 16)
                continue;

            if (dist > 3 * 3)
            {
                Vector2d pos = (Vector2d){(x + 0.5) * 2, (y + 0.5) * 2} - this->position;
                if (abs(pos.angleWith(frente) > M_PI / 180 * 90))
                    continue;
            }

            int idY = y * 3;
            int idX = x * 3;
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (i % 3 == 0 && i + idX > 0 && j % 3 == 0 && j + idY > 0)
                        desenhaArvore(i + idX, j + idY);
                }
            }
            // Repetir a luz pq senão buga
            glLightfv(GL_LIGHT0, GL_POSITION, luz);

            glPushMatrix();
            glMap2f(GL_MAP2_VERTEX_3,
                    0, 1, 3, 4,
                    0, 1, 12, 4,
                    &this->map[x][y].ctrlPoints[0][0][0]);
            glMapGrid2f(CHUNK_SIZE, 0, 1, CHUNK_SIZE, 0, 1);

            glTranslatef(x * CHUNK_SPACING, y * CHUNK_SPACING, 0.0f);

            GLfloat dx = 1.0 / CHUNK_SIZE, dy = 1.0 / CHUNK_SIZE;
            for (int i = 0; i < CHUNK_SIZE; ++i)
            {
                glBegin(GL_TRIANGLE_STRIP);
                for (int j = 0; j < CHUNK_SIZE; j++)
                {
                    // Primeiro triângulo
                    glEvalCoord2f(i * dx, j * dy);
                    glEvalCoord2f((i + 1) * dx, j * dy);
                }
                glEvalCoord2f(i * dx, CHUNK_SIZE * dy);
                glEvalCoord2f((i + 1) * dx, CHUNK_SIZE * dy);
                glEnd();
            }
            glPopMatrix();
        }
    }
    glPopMatrix();
}