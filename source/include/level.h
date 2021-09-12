#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
using std::vector;
#include <deque>
using std::deque;
#include <set>
using std::set;
#include <unordered_map>
using std::unordered_multimap;
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

        Snake() : m_body{} {/* empty */}
        Snake(Position start) : m_body{start} {/* empty */}

        Position next_move(Direction facing) const;
        void move(Direction facing);
        void enlarge(Direction facing);
        void reset(Position& pos);
        const Position& head() const;
        const Position& tail() const;
        std::array <Position, 4>  possible_moves() const;
        Path::iterator begin();
        Path::iterator end();

    private:
        Path m_body;
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

/// Functor that generates a hash number for a given position.
struct KeyHash {
    std::size_t operator()( const Position& pos ) const;
};

#endif
