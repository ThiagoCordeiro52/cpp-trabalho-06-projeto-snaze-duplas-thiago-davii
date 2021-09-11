#include <iostream>

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
    m_body.pop_back();

    m_body.push_front(next_move(facing));
}

void Snake::enlarge(Direction facing) {
    m_body.push_front(next_move(facing));
}
// 
// void Snake::backup() {
//     m_backup.emplace(m_body);
// }
// 
// void Snake::reset_backup() {
//     if (not m_backup.has_value())
//         throw std::runtime_error("the body of the snake was not backed up");
//     m_body = m_backup.value();
//     m_backup.reset();
// }

const Position& Snake::head() const {
    return m_body.front();
}

const Position& Snake::tail() const {
    return m_body.back();
}

void Snake::reset(Position& pos) {
    m_body = {pos};
    // m_backup.reset();
    m_lives -= 1;
}

bool Snake::dead() const {
    return m_lives <= 0;
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

std::deque<Snake::Instruction> Level::find_path(PlayerType type) {
    switch (type) {
        case RANDOM: {
            std::deque<Snake::Instruction> instructions;

            // Creates copies of the snake and the map to avoid changing their values
            auto map_copy = level_map;
            auto snake_copy = snake;

            std::default_random_engine generator;

            while (true) {
                // Create a vector of directions in random order
                std::vector directions (Snake::directions, Snake::directions + 4);
                std::shuffle(directions.begin(), directions.end(), generator);

                // Iterate over the random order directions
                bool found_path {false};
                for (const auto& direction: directions) {
                    auto pos {snake_copy.next_move(direction)};
                    auto& tile {map_copy[pos.first][pos.second]};

                    // Check if it is the food or the path and adds it to the instructions vector
                    if (tile == FOOD) {
                        instructions.emplace_back(Snake::ENLARGE, direction);
                        return instructions;
                    }
                    else if (tile == PATH) {
                        found_path = true;
                        tile = SNAKE;
                        instructions.emplace_back(Snake::MOVE, direction);
                        map_copy[snake_copy.tail().first][snake_copy.tail().second] = PATH;
                        snake_copy.move(direction);
                    }
                }

                if (found_path)
                    continue;

                // If no path is found, move in a random direction
                instructions.emplace_back(Snake::MOVE, directions[0]);
                return instructions;
            }
        } break;
        case BACKTRACKING: {
            // Create a deque contains the possibles paths to food [X]
            std::deque<std::pair <std::deque<Snake::Instruction>, Snake>> paths;
            auto map_copy = level_map;

            // Remove current snake from map to avoid problems
            for (const auto& pos : snake)
                map_copy[pos.first][pos.second] = PATH;

            paths.emplace_back(std::deque<Snake::Instruction>{}, snake);

            // Iterate over the deque of paths [X]
            while (not paths.empty()) {
                auto& curr_path {paths.front()};

                // Add current snake to map
                for (const auto& pos: curr_path.second)
                    map_copy[pos.first][pos.second] = SNAKE;

                // Check all the positions next to the snake [X]
                for (const auto& direction: Snake::directions) {
                    auto pos {curr_path.second.next_move(direction)};
                    const auto& tile {map_copy[pos.first][pos.second]};

                    // If the position is the food, return the current path [X]
                    if (tile == FOOD) {
                        curr_path.first.emplace_back(Snake::ENLARGE, direction);
                        return curr_path.first;
                    }
                    // If the position is a path, create a copy of the snake and move it there [X]
                    else if (tile == PATH) {
                        auto new_path = curr_path;
                        new_path.first.emplace_back(Snake::MOVE, direction);
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
            return find_path(RANDOM);
        } break;
    }
    throw std::runtime_error("unexpect error");
}

