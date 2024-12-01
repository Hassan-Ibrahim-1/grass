#include "engine.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    Mesh grass_mesh;
    uint instance_vbo = 0;
    // TODO: make this an array

    static constexpr uint ngrass = 100000;
    std::vector<glm::mat4> grass_mats;
    uint current_grass = 0;

    Color grass_color = Color(0, 255, 141);

    Cube& ground = *new Cube;
    PointLight& light = *new PointLight;

    float mult = 0.0220f;

    Shader grass_shader;

    /*GameObject& grass = *new GameObject;*/
    
    glm::mat4& create_grass_blade();
    void render_grass();
    void create_random_grass();
    void init_instance_vbo();
    glm::vec3 random_point_on_ground();
};

