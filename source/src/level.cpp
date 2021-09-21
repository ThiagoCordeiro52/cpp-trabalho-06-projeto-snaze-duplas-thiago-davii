#include "level.h"

Position Snake::next_move(Direction facing) const {
    auto& head {m_body.front()};

    Position pos;
    switch (facing) {
        case NORTH:
            pos = std::make_pair(head.first - 1, head.second);
            break;
        case SOUTH:
            pos = std::make_pair(head.first + 1, head.second);
            break;
        case EAST:
            pos = std::make_pair(head.first, head.second + 1);
            break;
        case WEST:
            pos = std::make_pair(head.first, head.second - 1);
            break;
    }

    return pos;
}

void Snake::move(Direction facing) {
    m_last_direction = facing;
    m_body.pop_back();

    m_body.push_front(next_move(facing));
}

void Snake::enlarge(Direction facing) {
    m_last_direction = facing;
    m_body.push_front(next_move(facing));
}

const Position& Snake::head() const {
    return m_body.front();
}

const Position& Snake::tail() const {
    return m_body.back();
}

const Snake::Direction& Snake::last_direction() const {
    return m_last_direction;
};

void Snake::reset(Position& pos) {
    m_body = {pos};
}

Path::iterator Snake::begin() {
    return m_body.begin();
}

Path::iterator Snake::end() {
    return m_body.end();
}

std::array <Position, 4> Snake::possible_moves() const {
    const auto & head = m_body.front(); 
    return {
        std::make_pair(head.first, head.second + 1), 
        std::make_pair(head.first, head.second - 1),
        std::make_pair(head.first + 1, head.second),
        std::make_pair(head.first - 1, head.second),
    };
}

// std::deque<Snake::Instruction> Level::find_path(PlayerType type) {
Snake::Instruction Level::next_instruction(PlayerType type) {
    switch (type) {
        case RANDOM: {
            std::default_random_engine generator;
            // sets the current time as the seed for generator 
            generator.seed(system_clock::now().time_since_epoch().count());

            // Create a vector of directions in random order
            std::vector directions (Snake::directions, Snake::directions + 4);
            std::shuffle(directions.begin(), directions.end(), generator);

            // Iterate over the random order directions
            for (const auto& direction: directions) {
                auto pos {snake.next_move(direction)};
                const auto& tile {level_map[pos.first][pos.second]};

                // Check if it is the food or the path and moves the snake in that direction
                if (tile == FOOD)
                    return std::make_pair(Snake::ENLARGE, direction);

                else if (tile == PATH)
                    return std::make_pair(Snake::MOVE, direction);
            }

            // If no path or food is found, move in a random direction
            return std::make_pair(Snake::MOVE, directions[0]);
        } break;
        case BACKTRACKING: {
            // if there is still instructions stored, return the first one 
            if (not instructions.empty()) {
                const auto& next {instructions.front()};
                instructions.pop_front();
                return next;
            }
            // Create a deque contains the possibles paths to food [X]
            std::deque paths {std::make_pair(std::deque<Snake::Instruction>{}, snake)};

            // Create a copy of the map to keep the original one intact
            auto map_copy = level_map;

            // Remove snake from map to avoid problems
            for (const auto& pos: snake)
                map_copy[pos.first][pos.second] = PATH;

            // Stores pairs of positions representing where the head of the snake was and were she moved to
            std::unordered_multiset<std::pair<Position, Position>, KeyHash> used_pos;

            // Iterate over the deque of paths [X]
            while (not paths.empty()) {
                auto& curr_path {paths.front()};

                // Add current snake to map
                for (const auto& pos: curr_path.second)
                    map_copy[pos.first][pos.second] = SNAKE_TAIL;

                // Check all the positions next to the snake [X]
                for (const auto& direction: Snake::directions) {
                    auto pos {curr_path.second.next_move(direction)};
                    const auto& tile {map_copy[pos.first][pos.second]};

                    std::pair value {curr_path.second.head(), pos};

                    // If the position is the food, return the current path [X]
                    if (tile == FOOD) {
                        curr_path.first.emplace_back(Snake::ENLARGE, direction);
                        instructions = curr_path.first;
                        return next_instruction(type);
                    }
                    // If the position is a path, create a copy of the snake and move it there [X]
                    // Also check if this moviment happend before and does not repeat it if so
                    else if (tile == PATH and used_pos.find(value) == used_pos.end()) {
                        used_pos.insert(value);
                        auto new_path = curr_path;
                        // Add the instruction to move in this direction
                        new_path.first.emplace_back(Snake::MOVE, direction);
                        // Move the snake
                        new_path.second.move(direction);

                        // Add new path no the end of the paths vector[X]
                        paths.push_back(new_path);
                    }
                }

                // Remove current snake from map, to avoid problems with other snakes
                for (const auto& pos: curr_path.second)
                    map_copy[pos.first][pos.second] = PATH;

                // Removes the current path from the deque [X]
                paths.pop_front();
            };

            // If no food is found in any of the paths, the snake will walk randonly
            return next_instruction(RANDOM);
        } break;
    }
    throw std::runtime_error("unexpect error");
}

std::size_t KeyHash::operator()( const std::pair<Position, Position>& positions ) const {
    return std::hash<int>()(positions.first.first
                            xor positions.first.second
                            xor positions.second.first
                            xor positions.second.second);
}

