#include "engine.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    uint ngrass = 1000;
    std::vector<GameObject*> grass;

    Cube& ground = *new Cube;
    PointLight& light = *new PointLight;

    float mult = 0.0220f;

    /*GameObject& grass = *new GameObject;*/
    
    GameObject& create_grass_blade();
    void update_grass();
    void create_random_grass();
    glm::vec3 random_point_on_ground();
};

