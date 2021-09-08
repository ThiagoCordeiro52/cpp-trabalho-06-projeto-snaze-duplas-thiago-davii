#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
using std::vector;
#include <utility>
using std::pair;

struct Snake {
    using position = std::vector<std::pair<int, int>>;

    position body;
    unsigned int lives;
};

struct Level {
    enum Tile {
        PATH,
        WALL,
        INVISIBLE_WALL,
        SNAKE_HEAD,
        SNAKE_TAIL,
        FOOD,
    };

    using Map = std::vector<std::vector<Tile>>;

    // Snake snake;
    Map level_map;
};

#endif
