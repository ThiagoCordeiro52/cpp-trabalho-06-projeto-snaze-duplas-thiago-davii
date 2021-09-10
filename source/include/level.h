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

using Position = std::pair<int, int>;
using Path = std::deque<Position>;

std::ostream& operator<<(std::ostream& os, Position pos);

enum PlayerType {
    RANDOM,
    BACKTRACKING,
};

class Snake {
    public:
        enum Direction {
            LEFT,
            RIGHT,
        };

        enum Orientation {
            NORTH,
            SOUTH,
            EAST,
            WEST,
        };

        enum Instruction {
            ROTATE_LEFT,
            ROTATE_RIGHT,
            MOVE,
            ENLARGE,
        };

        Snake() : m_body{}, m_backup{}, m_lives{0}, m_facing{EAST} {/* empty */}
        Snake(Position start, unsigned int lives) : m_body{start}, m_backup{}, m_lives{lives}, m_facing{EAST} {/* empty */}

        void rotate(Direction direction=LEFT);
        Position next_move() const;
        void move();
        void enlarge();
        void backup();
        void reset_backup();
        void reset(Position& pos);
        const Position& head() const;
        const Position& tail() const;
        bool dead() const;
        Path::iterator begin();
        Path::iterator end();
        // Path::const_iterator cbegin() const;
        // Path::const_iterator cend() const;

    private:
        struct Backup {
            Path body;
            Orientation facing;
        };
        Path m_body;
        std::optional<Backup> m_backup;
        unsigned int m_lives;
        Orientation m_facing;

        // friend class Level;
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
