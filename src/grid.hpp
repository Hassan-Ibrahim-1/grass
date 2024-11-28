#pragma once

#include "common.hpp"
#include "engine.hpp"

struct Cell : public Rect {
    bool traversable = true;

    Cell(Transform transform) : Rect(transform) {}
};

class Grid {
public:
    // NOTE: boundary is never rendered becase it doesn't have to be
    Rect& boundary = *new Rect;
    // Probably doesn't have to be heap allocated
    std::vector<Cell*> cells;

    Grid() {
        /*_cat = Texture2D("textures/bebe.jpg", TextureType::DIFFUSE);*/
    }
    Grid(uint ncells) {
        create_cells(ncells);
    }
    ~Grid() {
    }

    void create_cells(uint ncells);
    void add_to_scene();
    void delete_cells();

    size_t cell_count() const {
        return cells.size();
    }

    // returns -1 if no cell exists
    int cell_north(uint cell_index) const;
    int cell_south(uint cell_index) const;
    int cell_east(uint cell_index) const;
    int cell_west(uint cell_index) const;
    int cell_north_east(uint cell_index) const;
    int cell_north_west(uint cell_index) const;
    int cell_south_east(uint cell_index) const;
    int cell_south_west(uint cell_index) const;

    std::vector<uint> get_neighbours(uint cell_index);
    std::vector<uint> get_neighbours(Cell* cell);
    // Finds cell based on a provided x and z
    std::optional<Cell*> find_cell(const glm::vec3& position);
    // finds all the cells that contain this transform 
    std::vector<Cell*> find_all_cells(const Transform& transform);

private:
    // probably not the best way to do this
    Scene& _scene = engine::get_scene();

    uint _cols = 0;
    uint _rows = 0;

    enum Direction {
        NORTH = 0,
        SOUTH,
        EAST,
        WEST,
        NORTH_EAST,
        NORTH_WEST,
        SOUTH_EAST,
        SOUTH_WEST
    };

    /*Texture2D _cat;*/

    std::array<int, 8> _direction_offsets;

    void update_direction_offsets();
};

