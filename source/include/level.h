#ifndef LEVEL_H
#define LEVEL_H

// #include <iostream>
#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <utility>
using std::pair;
#include <optional>
using std::optional;
#include <stdexcept>
using std::runtime_error;
#include <array>
using std::array;
#include <algorithm>
using std::shuffle;
#include <random>
using std::default_random_engine;
using std::uniform_int_distribution;

using Position = std::pair<int, int>;
using Path = std::deque<Position>;

enum PlayerType {
    RANDOM,
    BACKTRACKING,
};

class Snake {
    public:
        enum Direction {
            NORTH,
            SOUTH,
            EAST,
            WEST,
        };

        constexpr static Direction directions [4] = {NORTH, SOUTH, EAST, WEST};

        enum Command {
            MOVE,
            ENLARGE,
        };

        using Instruction = std::pair<Command, Direction>;

        // Snake() : m_body{}, m_backup{}, m_lives{0} {/* empty */}
        // Snake(Position start, unsigned int lives) : m_body{start}, m_backup{}, m_lives{lives} {/* empty */}

        Snake() : m_body{}, m_lives{0} {/* empty */}
        Snake(Position start, unsigned int lives) : m_body{start}, m_lives{lives} {/* empty */}

        Position next_move(Direction facing) const;
        void move(Direction facing);
        void enlarge(Direction facing);
        // void backup();
        // void reset_backup();
        void reset(Position& pos);
        const Position& head() const;
        const Position& tail() const;
        bool dead() const;
        std::array <Position, 4>  possible_moves() const;
        Path::iterator begin();
        Path::iterator end();

    private:
        Path m_body;
        // std::optional<Path> m_backup;
        unsigned int m_lives;
};

struct Level {
    enum Tile {
        PATH,
        WALL,
        INVISIBLE_WALL,
        // SNAKE_HEAD,
        // SNAKE_TAIL,
        SNAKE,
        FOOD,
        // CRASH,
    };

    using Map = std::vector<std::vector<Tile>>;
    std::deque<Snake::Instruction> find_path(PlayerType type);

    Snake snake;
    Map level_map;
    Position initial_pos;
    unsigned int quant_food;
};

#endif
