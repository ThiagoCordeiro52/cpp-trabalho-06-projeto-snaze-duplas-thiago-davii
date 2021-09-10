#include "level.h"

// std::ostream& operator<<(std::ostream& os, Position pos) {
//     os << '(' << pos.first << ", " << pos.second << ')'; 
// 
//     return os;
// }
// 
// std::ostream& operator<<(std::ostream& os, Path path_) {
//     for (const auto& pos: path_) {
//         os << pos << " => "; 
//     }
// 
//     return os;
// }
// 
// std::ostream& operator<<(std::ostream& os, Snake::Orientation orientation) {
//     switch (orientation) {
//         case Snake::NORTH:
//             os << "NORTH";
//             break;
//         case Snake::SOUTH:
//             os << "SOUTH";
//             break;
//         case Snake::EAST:
//             os << "EAST";
//             break;
//         case Snake::WEST:
//             os << "WEST";
//             break;
//     }
// 
//     return os;
// }

void Snake::rotate(Direction direction) {
    if (direction == LEFT) {
        if (m_facing == NORTH)
            m_facing = WEST;
        else if (m_facing == SOUTH)
            m_facing = EAST;
        else if (m_facing == EAST)
            m_facing = NORTH;
        else if (m_facing == WEST)
            m_facing = SOUTH;
    } else if (direction == RIGHT) {
        if (m_facing == NORTH)
            m_facing = EAST;
        else if (m_facing == SOUTH)
            m_facing = WEST;
        else if (m_facing == EAST)
            m_facing = SOUTH;
        else if (m_facing == WEST)
            m_facing = NORTH;
    }
}

Position Snake::next_move() const {
    auto& head {m_body.front()};

    Position pos;
    switch (m_facing) {
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

void Snake::move() {
    m_body.pop_back();

    m_body.push_front(next_move());
}

void Snake::enlarge() {
    m_body.push_front(next_move());
}

void Snake::backup() {
    m_backup.emplace(Backup{m_body, m_facing});
}

void Snake::reset_backup() {
    if (not m_backup.has_value())
        throw std::runtime_error("the body of the snake was not backed up");
    m_body = m_backup.value().body;
    m_facing = m_backup.value().facing;
    m_backup.reset();
}

const Position& Snake::head() const {
    return m_body.front();
}

const Position& Snake::tail() const {
    return m_body.back();
}

void Snake::reset(Position& pos) {
    // m_body.clear();
    // m_body.push_front(pos);
    m_body = {pos};
    m_backup.reset();
    m_facing = EAST;
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
// 
// Path::const_iterator Snake::cbegin() const {
//     return m_body.cbegin();
// }
// 
// Path::const_iterator Snake::cend() const {
//     return m_body.cend();
// }

std::deque<Snake::Instruction> Level::find_path(PlayerType type) {
    switch (type) {
        case RANDOM: {
            std::deque<Snake::Instruction> instructions;
            auto map_copy = level_map;
            snake.backup();

            while (true) {
                bool to_continue {false};
                Position pos;
                // auto dir {static_cast<Snake::Direction>(rand() % 2)};
                
                for (int i {0}; i < (rand() % 4); i++) {
                    instructions.push_back(Snake::ROTATE_LEFT);
                    snake.rotate();
                }
                for (int i {0}; i < 4; i++) {
                    pos = snake.next_move();

                    auto& tile = map_copy[pos.first][pos.second];
                    if (tile == FOOD) {
                        instructions.push_back(Snake::ENLARGE);
                        snake.reset_backup();
                        return instructions;
                    }
                    else if (tile == PATH) {
                        tile = SNAKE;
                        to_continue = true;
                        break;
                    }

                    snake.rotate();
                    instructions.push_back(Snake::ROTATE_LEFT);
                    // snake.rotate(dir);
                    // instructions.push_back(static_cast<Snake::Instruction>(dir));
                }

                // found_path.push_back(pos);
                instructions.push_back(Snake::MOVE);
                auto& to_remove {snake.tail()};
                snake.move();
                map_copy[to_remove.first][to_remove.second] = PATH;

                if (to_continue)
                    continue;

                snake.reset_backup();
                return instructions;
            }
        } break;
        case BACKTRACKING:
            return {};
    }
}

