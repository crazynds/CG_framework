#include <entities/Engine.h>
#include <gl_canvas2d.h>
#include <EngineState.h>
#include <Matrix4.h>
#include <vector>

#define PERSPECTIVE_D 10

struct object
{
    vector<Vector3d> positions;
    Color color;
};

Engine::Engine() : Entity({0, 0})
{
}

int Engine::tick(EngineState *state, double delta)
{
    this->rotation += delta * speedEngine * this->pistoes;
    // D
    if (state->getKeyboardState(97))
        this->cameraRotation += delta * 2;
    // A
    if (state->getKeyboardState(100))
        this->cameraRotation -= delta * 2;

    // W
    if (state->getKeyboardState(119))
        this->speedEngine += delta * 3;
    // S
    if (state->getKeyboardState(115))
        this->speedEngine -= delta * 3;

    // Espace
    if (state->getKeyboardState(32))
    {
        this->onlyLines = !this->onlyLines;
        state->keyboardHandler(32, false);
    }
    // Q
    if (state->getKeyboardState(113))
    {
        this->only2d = !this->only2d;
        state->keyboardHandler(113, false);
    }
    // R
    if (state->getKeyboardState(114))
    {
        this->pistoes -= -1;
        if (this->pistoes > 6)
        {
            this->pistoes = 1;
        }
        state->keyboardHandler(114, false);
    }
    // T
    if (state->getKeyboardState(116))
    {
        this->exibe -= -1;
        if (this->exibe > 6)
        {
            this->exibe = 0;
        }
        state->keyboardHandler(116, false);
    }
    // P
    if (state->getKeyboardState(112))
    {
        this->perspective = !this->perspective;
        state->keyboardHandler(112, false);
    }

    if (state->getMouseWheel() == 1)
    {
        this->zoom *= 1.05;
    }
    else if (state->getMouseWheel() == -1)
    {
        this->zoom *= 0.95;
    }

    return TICK_OK;
}

vector<struct object> drawCog(double x, double y, double z, double tamanho, int dentes, double rotation)
{
    vector<Vector3d> positions;

    double angle = PI_2 / (dentes * 2);

    for (int i = 0; i < dentes * 2; i++)
    {
        if ((i & 1) == 0)
        {
            positions.push_back({cos(rotation + angle * (i - 0.20)) * 0.85, sin(rotation + angle * (i - 0.20)) * 0.85, 0});
            positions.push_back({cos(rotation + angle * (i + 0.20)) * 1.1, sin(rotation + angle * (i + 0.20)) * 1.1, 0});
        }
        else
        {
            positions.push_back({cos(rotation + angle * (i - 0.20)) * 1.1, sin(rotation + angle * (i - 0.20)) * 1.1, 0});
            positions.push_back({cos(rotation + angle * (i + 0.20)) * 0.85, sin(rotation + angle * (i + 0.15)) * 0.85, 0});
        }
    }
    CV::color(BLACK);
    Matrix4Plan<double> p;
    p.addScaleMatrix(tamanho, tamanho, tamanho);
    p.push();
    p.addTranslateMatrix(x, y, z);
    // perspective projection
    struct object obj1, obj2;
    obj1.color = GREY;
    obj2.color = GREY;
    vector<struct object> ret;
    ret.push_back(obj1);
    ret.push_back(obj2);

    for (unsigned int i = 0; i < positions.size(); i++)
    {
        obj1.positions.push_back(p.compute(positions[i]));
        obj2.positions.push_back(p.compute(positions[i]) + (Vector3d){0, 0, 10});
        struct object obj3;
        obj3.color = GREY;

        obj3.positions.push_back(p.compute(positions[i]));
        obj3.positions.push_back(p.compute(positions[i]) + (Vector3d){0, 0, 10});
        obj3.positions.push_back(p.compute(positions[(i + 1) % positions.size()]) + (Vector3d){0, 0, 10});
        obj3.positions.push_back(p.compute(positions[(i + 1) % positions.size()]));
        ret.push_back(obj3);
    }
    ret[0] = obj1;
    ret[1] = obj2;
    return ret;
}

vector<struct object> drawCilindro(Vector3d base, Vector3d rotation, double raio, double altura, Color cor)
{
    vector<struct object> cilindro;
    vector<Vector3d> detonador = {
        {-1, 0, 0},
        {1, 0, 0},
        {1, 1, 0},
        {-1, 1, 0},
    };
    Matrix4Plan<double> plan;
    plan.addScaleMatrix(raio, altura, raio);
    plan.push();
    plan.addRotateXMatrix(rotation.x);
    plan.addRotateYMatrix(rotation.y);
    plan.addRotateZMatrix(rotation.z);
    plan.push();
    plan.addTranslateMatrix(base);
    plan.push();
    const int rotations = 8;
    struct object cima, baixo;
    cima.color = cor;
    baixo.color = cor;
    for (unsigned int i = 0; i < rotations; i++)
    {
        struct object obj1;
        obj1.color = cor;
        Matrix4Plan<double> rotPlan;
        rotPlan.addRotateYMatrix(PI_2 / rotations * i);
        for (unsigned int j = 0; j < detonador.size(); j++)
        {
            auto p = plan.compute(rotPlan.compute(detonador[j]));
            if (j == 0)
                cima.positions.push_back(p);
            if (j == 2)
                baixo.positions.push_back(p);
            obj1.positions.push_back(p);
        }
        cilindro.push_back(obj1);
    }
    cilindro.push_back(cima);
    cilindro.push_back(baixo);
    return cilindro;
}

vector<struct object> drawCogMain(double rotation, double z)
{
    return drawCog(59, 49, z, 45, 12, rotation);
}

vector<struct object> drawMiniCog(double rotation, double z)
{
    return drawCog(0, 0, z, 30, 9, rotation);
}

vector<struct object> drawPiston(double rotation, double z)
{
    Vector3d base = {-50, 90, z};
    Vector3d rot = Matrix4<double>::rotationZ(rotation) * (Vector3d){20, 0, base.z};
    auto rotacaoBase = rot - base;
    Matrix4Plan<double> plan1;
    plan1.addScaleMatrix(5, 50, 5);
    plan1.push();
    plan1.addRotateZMatrix(atan2(-rotacaoBase.x, rotacaoBase.y));
    plan1.push();
    plan1.addTranslateMatrix(base);
    plan1.push();

    // CV::color(RED);
    // CV::circleFill({base.x, base.y}, 5, 12);
    // CV::circleFill({rot.x, rot.y}, 5, 12);
    // CV::circleFill({0, 0}, 5, 12);

    vector<struct object> vet1;
    auto camara = drawCilindro(base, {0, 0, atan2(-rotacaoBase.x, rotacaoBase.y)}, 10, 50, RED);
    vet1.insert(vet1.end(), camara.begin(), camara.end());

    Vector3d ponta = plan1.compute({0, 1, 0});
    auto tamanho = (ponta - rot).size();

    auto pistao = drawCilindro(ponta, {0, 0, atan2(-rotacaoBase.x, rotacaoBase.y)}, 5, tamanho, BLUE);
    vet1.insert(vet1.end(), pistao.begin(), pistao.end());

    return vet1;
}

vector<struct object> drawVirabreqin(double rotation, double z, int pistoes)
{
    vector<struct object> objects;
    auto v1 = drawCilindro({0, 0, -52}, {PI / 2, rotation, 0}, 3, 51, GREEN);
    objects.insert(objects.end(), v1.begin(), v1.end());

    Vector3d base = {-50, 90, z + 60};

    for (int x = 0; x < pistoes; x++)
    {
        double angle = x * PI + PI / 2;
        double spacing = x * 60;
        Vector3d rot = Matrix4<double>::rotationZ(-rotation - angle + PI / 2) * (Vector3d){20, 0, base.z};
        auto v2 = drawCilindro(rot + (Vector3d){0, 0, -15 + spacing}, {PI / 2, 0, 0}, 2.5, 30, GREEN);
        objects.insert(objects.end(), v2.begin(), v2.end());

        auto v3 = drawCilindro({0, 0, -3 + spacing}, {0, 0, -rotation - angle}, 2.5, 20, GREEN);
        objects.insert(objects.end(), v3.begin(), v3.end());

        auto v4 = drawCilindro({0, 0, 23 + spacing}, {0, 0, -rotation - angle}, 2.5, 20, GREEN);
        objects.insert(objects.end(), v4.begin(), v4.end());
        if (x < pistoes - 1)
        {
            auto v5 = drawCilindro({0, 0, 21 + spacing}, {PI / 2, rotation, 0}, 3, 37, GREEN);
            objects.insert(objects.end(), v5.begin(), v5.end());
        }
    }

    return objects;
}

void Engine::render()
{
    CV::translate(screenWidth / 2, screenHeight / 2);
    vector<struct object> objects;
    const int pistoes = this->pistoes;
    if (this->exibe == 0 || this->exibe == 1 || this->exibe == 3)
    {
        auto v1 = drawCogMain(this->rotation * 3 / 4, -50);
        objects.insert(objects.end(), v1.begin(), v1.end());
    }
    if (this->exibe == 0 || this->exibe == 2 || this->exibe == 3)
    {
        auto v2 = drawMiniCog(-this->rotation + 0.2, -50);
        objects.insert(objects.end(), v2.begin(), v2.end());
    }
    if (this->exibe == 0 || this->exibe == 4 || this->exibe == 6)
    {
        auto v5 = drawVirabreqin(this->rotation, -50, pistoes);
        objects.insert(objects.end(), v5.begin(), v5.end());
    }
    if (this->exibe == 0 || this->exibe == 5 || this->exibe == 6)
    {
        for (int x = 0; x < pistoes; x++)
        {
            auto v3 = drawPiston(-this->rotation + (x & 1 ? PI : 0), 10 + x * 60);
            objects.insert(objects.end(), v3.begin(), v3.end());
        }
    }

    Matrix4Plan<double> rotation;
    if (!this->only2d)
        rotation.addRotateYMatrix(this->cameraRotation);

    rotation.addScaleMatrix(this->zoom, this->zoom, this->zoom);
    rotation.push();
    rotation.addTranslateMatrix({0, -30, 0});
    if (this->perspective && !this->only2d)
    {
        rotation.addTranslateMatrix({0, 0, 300});
        rotation.push();
        rotation.addMultMatrix(Matrix4<double>::projection(250));
    }
    for (auto obj : objects)
    {
        CV::color(obj.color);
        Vector2d arr[obj.positions.size()];
        for (unsigned int i = 0; i < obj.positions.size() + 1; i++)
        {
            auto p1 = rotation.compute(obj.positions[i % obj.positions.size()]);
            arr[i] = {p1.x, p1.y};
        }
        if (this->onlyLines)
            CV::polygon(arr, obj.positions.size());
        else
            CV::polygonFill(arr, obj.positions.size());
    }
    CV::color(BLACK);

    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 15}, "ESPACE para colorir");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 30}, "A & D para rotacionar");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 45}, "W & S para acelerar/desacelerar");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 60}, "Q para alternar 2d/3d");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 75}, "R para adicionar mais vrun vruns");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 90}, "Rodinha do mouse para zoom");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 105}, "P para alterar modo perspectiva");
    CV::textf({-screenWidth / 2.0 + 10, screenHeight / 2.0 - 120}, "T para alterar oq vai ser exibido");
}