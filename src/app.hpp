#include "engine.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    Cube& ground = *new Cube;
    PointLight& light = *new PointLight;

    GameObject& grass = *new GameObject;
};

