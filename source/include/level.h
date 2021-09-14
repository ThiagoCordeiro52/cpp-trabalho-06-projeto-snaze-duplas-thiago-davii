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
#include <chrono>
using std::chrono::system_clock;
using std::chrono::milliseconds;

using Position = std::pair<int, int>;
using Path = std::deque<Position>;

// Type of player that chooses the directions of the snake
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

        /**
        * @brief Calculates which position the snake will move to next
        *
        * @param facing direction where the snake is looking
        *
        * @return a pair with the coordinates of the next position the snake will move
        */
        Position next_move(Direction facing) const;

        /**
        * @brief moves the snake to the next position it is looking at
        *
        * @param facing direction where the snake is looking at
        *
        */
        void move(Direction facing);

        /**
        * @brief increases snake size by one position
        *
        * @param facing direction where the snake is looking at
        *
        */
        void enlarge(Direction facing);

        /**
        * @brief resets the snake's size if it hits a wall or your body
        *
        * @param pos a pair with the coordinates where the snake will be after the body is reset
        *
        */
        void reset(Position& pos);

        /**
        *
        * @return a pair with snake head positions
        */
        const Position& head() const;

        /**
        *
        * @return a pair with the last position of the snake's tail
        *
        */
        const Position& tail() const;


        /**
        *
        * @return the last direction the snake moved
        *
        */
        const Direction& last_direction() const;

        /**
        *
        * @return an array with the four possible directions the snake can move
        *
        */
        std::array <Position, 4>  possible_moves() const;

        /**
        *
        * @return an iterator to the snake's first body position
        *
        */
        Path::iterator begin();

        /**
        *
        * @return an iterator to the snake's last body position
        *
        */
        Path::iterator end();

    private:
        Path m_body;
        Direction m_last_direction {Direction::NORTH};
};

struct Level {
    enum Tile {
        PATH,
        WALL,
        INVISIBLE_WALL,
        SNAKE_HEAD,
        SNAKE_TAIL,
        FOOD,
        CRASH,
    };

    using Map = std::vector<std::vector<Tile>>;
    /**
    * @brief Calculates the next move of snake using a Player of type type 
    *
    * @param type type of player
    *
    * @return the next instruction the snake must follow
    */
    Snake::Instruction next_instruction(PlayerType type);

    Snake snake;
    Map level_map;
    Position initial_pos;
    unsigned int quant_food;
    std::deque<Snake::Instruction> instructions;
};

/// Functor that generates a hash number for a given position.
struct KeyHash {
    std::size_t operator()( const Position& pos ) const;
};

#endif
