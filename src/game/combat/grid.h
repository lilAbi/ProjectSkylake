#ifndef PROJECTSKYLAKE_GRID_H
#define PROJECTSKYLAKE_GRID_H

#include "game/core/types.h"

#include <vector>

struct GridTile {
    bool         m_walkable{true};
    std::int32_t m_movement_cost{1};
    std::int32_t m_elevation{};
    bool         m_blocks_sight{};
};

class Grid {
public:
    Grid(std::int32_t width, std::int32_t height, std::vector<GridTile>);
    bool                      contains(GridPosition) const;
    const GridTile*           tile(GridPosition) const;
    std::vector<GridPosition> neighbors(GridPosition) const;
    std::int32_t              width() const;
    std::int32_t              height() const;

private:
    std::int32_t          m_width{};
    std::int32_t          m_height{};
    std::vector<GridTile> m_tiles;
};

#endif //PROJECTSKYLAKE_GRID_H
