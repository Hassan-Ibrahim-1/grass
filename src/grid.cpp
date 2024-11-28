#include "grid.hpp"
#include "engine.hpp"
#include "utils.hpp"

void Grid::create_cells(uint ncells) {
    delete_cells();

    uint f1 = static_cast<uint>(glm::sqrt(ncells));
    while (ncells % f1 != 0) {
        f1--;
    }
    // f2 > f1
    uint f2 = ncells / f1; // along the width

    _rows = 0;
    _cols = 0;

    // NOTE: if you wanna change this back to the vertical grid
    // sub out ALL z's for y's

    // put the larger factor amount of cells on the larger side
    if (boundary.transform.scale.x > boundary.transform.scale.z) {
        _rows = f1 - 1;
        _cols = f2 - 1;
        /*_rows = f1;*/
        /*_cols = f2;*/
    }
    else {
        _rows = f2 - 1;
        _cols = f1 - 1;
        /*_rows = f2;*/
        /*_cols = f1;*/
    }

    update_direction_offsets();

    Transform cell_t = Transform();
    cell_t.position.y = boundary.transform.position.y;
    cell_t.rotation = boundary.transform.rotation;

    cell_t.scale.x = boundary.transform.scale.x / _cols;
    cell_t.scale.z = boundary.transform.scale.z / _rows;

    // make cell go to the upper left corner
    cell_t.position.x = (-0.5 * boundary.transform.scale.x) + boundary.transform.position.x;
    // + here to make the cell go to the right
    cell_t.position.x += (cell_t.scale.x / 2);

    // - here to make the cell go downwards - opposite of vao coordinate
    cell_t.position.z = (0.5 * boundary.transform.scale.z) + boundary.transform.position.z;
    cell_t.position.z -= (cell_t.scale.z);

    float original_xpos = cell_t.position.x;

    for (uint i = 0; i < _rows; i++) {
        for (uint j = 0; j < _cols; j++) {
            cells.push_back(new Cell(cell_t));
            cells.back()->set_fill(false);
            /*cells.back()->transform.rotation = boundary.transform.rotation;*/
            /*auto& texture = cells.back()->material.create_diffuse_texture();*/
            /*texture = Texture2D(_cat.ID, TextureType::DIFFUSE);*/
            cell_t.position.x += cell_t.scale.x;
            cells.back()->material.shader = &engine::get_renderer().shaders.basic_mesh;
        }
        cell_t.position.x = original_xpos;
        cell_t.position.z -= cell_t.scale.z;
    }
}

void Grid::add_to_scene() {
    for (auto& cell : cells) {
        _scene.add_primitive(cell);
    }
    /*_scene.add_primitive(&boundary);*/
}

void Grid::delete_cells() {
    for (uint i = 0; i < cells.size(); i++) {
        _scene.delete_game_object(cells[i]);
    }
    cells.clear();
}

void Grid::update_direction_offsets() {
    _direction_offsets[NORTH] = -_cols;
    _direction_offsets[EAST] = 1;
    _direction_offsets[WEST] = -1;
    _direction_offsets[SOUTH] = _cols;
    _direction_offsets[NORTH_EAST] = -_cols + 1;
    _direction_offsets[NORTH_WEST] = -_cols - 1;
    _direction_offsets[SOUTH_EAST] = _cols + 1;
    _direction_offsets[SOUTH_WEST] = _cols - 1;
}

int Grid::cell_north(uint cell_index) const {
    // still on the first row
    if (cell_index < _cols) return -1;

    uint index = cell_index + _direction_offsets[NORTH];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_south(uint cell_index) const {
    // if on the bottom row
    if (cell_index > _cols * (_rows -1)) return -1;

    uint index = cell_index + _direction_offsets[SOUTH];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_east(uint cell_index) const {
    // if on the right edge
    if ((cell_index % _cols) == (_cols - 1)) return -1;

    uint index = cell_index + _direction_offsets[EAST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_west(uint cell_index) const {
    // if on the left edge
    if ((cell_index % _cols) == 0) return -1;
    /*if (((cell_index + _cols) % _cols) == 0) return -1;*/

    uint index = cell_index + _direction_offsets[WEST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}

int Grid::cell_north_east(uint cell_index) const {
    if (cell_index < _cols) return -1;
    /*if ((cell_index % (_cols - 1)) == 0) return -1;*/
    if ((cell_index % _cols) == (_cols - 1)) return -1;

    uint index = cell_index + _direction_offsets[NORTH_EAST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_north_west(uint cell_index) const {
    if (cell_index < _cols) return -1;
    if ((cell_index % _cols) == 0) return -1;

    uint index = cell_index + _direction_offsets[NORTH_WEST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_south_east(uint cell_index) const {
    if (cell_index >= _cols * (_rows -1)) return -1;
    if ((cell_index % _cols) == (_cols - 1)) return -1;

    uint index = cell_index + _direction_offsets[SOUTH_EAST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}
int Grid::cell_south_west(uint cell_index) const {
    if (cell_index > _cols * (_rows -1)) return -1;
    if ((cell_index % _cols) == 0) return -1;

    uint index = cell_index + _direction_offsets[SOUTH_WEST];
    if (index < 0 || index >= cells.size()) {
        return -1;
    }
    return index;
}

std::vector<uint> Grid::get_neighbours(uint cell_index) {
    std::vector<uint> neighbours;

    if (cell_north(cell_index) != -1) {
        neighbours.push_back(cell_north(cell_index));
    }
    if (cell_south(cell_index) != -1) {
        neighbours.push_back(cell_south(cell_index));
    }
    if (cell_east(cell_index) != -1) {
        neighbours.push_back(cell_east(cell_index));
    }
    if (cell_west(cell_index) != -1) {
        neighbours.push_back(cell_west(cell_index));
    }
    if (cell_north_east(cell_index) != -1) {
        neighbours.push_back(cell_north_east(cell_index));
    }
    if (cell_north_west(cell_index) != -1) {
        neighbours.push_back(cell_north_west(cell_index));
    }
    if (cell_south_east(cell_index) != -1) {
        neighbours.push_back(cell_south_east(cell_index));
    }
    if (cell_south_west(cell_index) != -1) {
        neighbours.push_back(cell_south_west(cell_index));
    }

    /*LOG("neighbour count: %zu", neighbours.size());*/
    return neighbours;
}
std::vector<uint> Grid::get_neighbours(Cell* cell) {
    return get_neighbours(
        std::find(cells.begin(), cells.end(), cell) - cells.begin()
    );
}

std::optional<Cell*> Grid::find_cell(const glm::vec3& position) {
    for (auto c : cells) {
        if (utils::point_in_rect(
            *c,
            glm::vec2(position.x, position.z)
        )) {
            return c;
        }
    }
    return {};
}

std::vector<Cell*> Grid::find_all_cells(const Transform& transform) {
    float start_x = transform.position.x - (transform.scale.x / 2);
    float start_z = transform.position.z + (transform.scale.z / 2);
    // HACK: this fixes rect y being bs
    // Bottom right
    float end_x = transform.position.x + (transform.scale.x / 2);
    float end_z = transform.position.z - (transform.scale.z / 2);

    std::vector<Cell*> cells_in_transform;

    for (auto cell : cells) {
        const auto& pos = cell->transform.position;
        if ((pos.x > start_x && pos.x < end_x)
         && (pos.z < start_z && pos.z > end_z)) {
            cells_in_transform.emplace_back(cell);
        }
    }
    return cells_in_transform;
}

