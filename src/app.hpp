#include "engine.hpp"

class App : public Application {
public:
    void init() override;
    void update() override;
    void cleanup() override;

    uint ngrass = 1;
    Mesh grass_mesh;
    uint instance_vbo = 0;
    uint instance_inverse_model_vbo = 0;
    // TODO: make this an array
    std::vector<Transform> grass_transforms;
    Color grass_color = Color(0, 255, 141);

    Cube& ground = *new Cube;
    PointLight& light = *new PointLight;

    float mult = 0.0220f;

    Shader grass_shader;

    /*GameObject& grass = *new GameObject;*/
    
    Transform& create_grass_blade();
    void update_grass();
    void render_grass();
    void create_random_grass();
    void init_instance_vbo();
    void send_transform_data();
    glm::vec3 random_point_on_ground();
};

