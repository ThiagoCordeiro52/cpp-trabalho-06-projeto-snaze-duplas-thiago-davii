#include "snaze_manager.h"

std::ostream& operator<<(std::ostream& os, const Snake::Instruction& instruction) {
    switch (instruction) {
        case Snake::MOVE:
            os << "MOVE";
            break;
        case Snake::ROTATE_LEFT:
            os << "ROTATE_LEFT";
            break;
        case Snake::ROTATE_RIGHT:
            os << "ROTATE_RIGHT";
            break;
        case Snake::ENLARGE:
            os << "ENLARGE";
            break;
    }

    return os;
}
// 
// std::ostream& operator<<(std::ostream& os, const std::deque<Snake::Instruction>& instructions) {
//     for (const auto& instruction: instructions) {
//         std::cout << instruction << ' ';
//     }
// 
//     return os;
// }

//====================[ SNAZE MANAGER METHODS ]========================//
std::string to_lowercase(const std::string& str) {
    auto clone = str;
    for (auto& c : clone) {
        c = ::tolower(c);
    }
    return clone;
}

SnazeManager::SnazeManager(int argc, char *argv[]) {
    bool filename_set {false};
    for (int i {1}; i < argc; i++) {
        std::string arg {to_lowercase(argv[i])};
        if (arg == "-h" or arg == "--help") {
            print_usage();
            exit(0);
        } else if (arg == "-f" or arg == "--fps") {
            if (++i >= argc)
                error("argument of " + arg + " missing");

            try {
                m_fps = static_cast<number_type>(std::stoul(argv[i]));
            }
            catch (const std::invalid_argument& e) {
                error("the argument of the " + arg + " is not an integer");
            }
            catch (const std::out_of_range& e) {
                error("the argument of " + arg + " is out of range");
            }

            if (m_fps < 1 || m_fps > 24)
                error("the argument of " + arg + " is out of range");
        } else if (arg == "-l" or arg == "--lives") {
            if (++i >= argc)
                error("argument of " + arg + " missing");

            try {
                m_lives = static_cast<number_type>(std::stoul(argv[i]));
            }
            catch (const std::invalid_argument& e) {
                error("the argument of the " + arg + " is not an integer");
            }
            catch (const std::out_of_range& e) {
                error("the argument of " + arg + " is out of range");
            }

            if (m_lives < 1 || m_lives > 50)
                error("the argument of " + arg + " is out of range");
        } else if (arg == "-nf" or arg == "--food") {
            if (++i >= argc)
                error("argument of " + arg + " missing");

            try {
                m_quant_food = static_cast<number_type>(std::stoul(argv[i]));
            }
            catch (const std::invalid_argument& e) {
                error("the argument of the " + arg + " is not an integer");
            }
            catch (const std::out_of_range& e) {
                error("the argument of " + arg + " is out of range");
            }

            if (m_quant_food < 1 || m_quant_food > 100)
                error("the argument of " + arg + " is out of range");
        } else if (arg == "-p" or arg == "--playertype") {
            if (++i >= argc)
                error("argument of " + arg + " missing");

            std::string arg2 {argv[i]};

            if (arg2 == "random" or arg2 == "r") {
                m_player_type = RANDOM;
            } else if (arg2 == "backtracking" or arg2 == "b") {
                m_player_type = BACKTRACKING;
            } else {
                error("invalid argument \"" + arg2 + "\" given to " + arg);
            }
        } else {
            if (filename_set)
                error("unknown option \"" + arg + "\"");

            filename_set = true;
            m_filename = arg;
        }
    }
    if (!filename_set)
        error("the path to the input file must be passed as argument");
}

void SnazeManager::process() {
    switch (m_state) {
        case START:
            break;

        case WELCOME:
            break;

        case READING: {
            std::ifstream file{m_filename};
            while (not file.eof()) {
                number_type rows;
                number_type columns;

                if (file.fail()) {
                    error("could not open file \"" + m_filename + "\"");
                }

                std::string rows_str, columns_str;

                if (not getline(file >> std::ws, rows_str, ' '))
                    error("could not read rows number from file \"" + m_filename + "\"");

                try {
                    rows = static_cast<number_type>(std::stoul(rows_str));
                }
                catch (const std::invalid_argument& e) {
                    error("the number of the rows is not an integer");
                }
                catch (const std::out_of_range& e) {
                    error("the number of the rows is out of range");
                }

                if (not getline(file >> std::ws, columns_str))
                    error("could not read columns number from file \"" + m_filename + "\"");

                try {
                    columns = static_cast<number_type>(std::stoul(columns_str));
                }
                catch (const std::invalid_argument& e) {
                    error("the number of the columns is not an integer");
                }
                catch (const std::out_of_range& e) {
                    error("the number of the columns is out of range");
                }

                Level level;
                level.level_map.reserve(rows + 2);
                level.level_map.emplace_back(columns + 2, Level::INVISIBLE_WALL);
                for (int i = 1; i <= rows; i++) {
                    std::string line_str;
                    getline(file >> std::ws, line_str);

                    std::vector<Level::Tile> line {Level::INVISIBLE_WALL};
                    line.reserve(columns + 2);

                    for (int j = 1; j <= columns; j++) {
                        Level::Tile tile;
                        switch (line_str[j - 1]) {
                            case ' ':
                                tile = Level::Tile::PATH;
                                break;
                            case '#':
                                tile = Level::Tile::WALL;
                                break;
                            case '*':
                                level.snake = Snake{std::make_pair(i, j), m_lives};
                                tile = Level::Tile::SNAKE;
                                break;
                            case '.':
                                tile = Level::Tile::INVISIBLE_WALL;
                                break;
                            default:
                                error("invalid character \"" + std::string{line_str[j - 1]} + "\" in " + m_filename);
                                break;
                        }
                        line.push_back(tile);
                    }
                    line.push_back(Level::INVISIBLE_WALL);
                    level.level_map.push_back(line);
                }
                level.level_map.emplace_back(columns + 2, Level::INVISIBLE_WALL);
                m_levels.push(level);

                file >> std::ws;
            }
         } break;

        case CREATING_LEVEL: {
            auto& level {m_levels.front()};
            std::default_random_engine generator;
            generator.seed(system_clock::now().time_since_epoch().count());

            using rand_in_range = std::uniform_int_distribution<int>;

            int row, column;
            do {
                row = rand_in_range(1, level.level_map.size() - 2)(generator);
                column = rand_in_range(1, level.level_map[0].size() - 2)(generator);
            } while (level.level_map[row][column] != Level::PATH);
            level.level_map[row][column] = Level::FOOD;
        } break;

        case THINKING: {
            auto& level {m_levels.front()};
            m_instructions = level.find_path(m_player_type);
        } break;

        default:
            break;
    }
}

void SnazeManager::update() {
    switch (m_state) {
        case START:
            m_state = WELCOME;
            break;

        case WELCOME:
            m_state = READING;
            break;

        case READING:
            m_state = CREATING_LEVEL;
            break;

        case CREATING_LEVEL:
            m_state = THINKING;
            break;

        case THINKING:
            m_state = MOVING;
            break;

        case MOVING: {
            if (m_instructions.empty()) {
                std::cout << "end of instructions\n";
                m_state = END;
                break;
            }
            auto& next_instruction {m_instructions.front()};
            m_instructions.pop_front();
            auto& curr_level {m_levels.front()};

            std::cout << next_instruction << '\n';

            switch (next_instruction) {
                case Snake::MOVE: {
                    auto removed {curr_level.snake.tail()};

                    curr_level.snake.move();

                    auto added {curr_level.snake.head()};

                    curr_level.level_map[removed.first][removed.second] = Level::PATH;

                    auto& added_tile = curr_level.level_map[added.first][added.second];
                    if (added_tile == Level::PATH)
                        added_tile = Level::SNAKE;
                    else
                        m_state = LOSE_GAME;
                } break;
                case Snake::ENLARGE: {
                    curr_level.snake.enlarge();

                    auto added {curr_level.snake.head()};
                    curr_level.level_map[added.first][added.second] = Level::SNAKE;

                    m_state = CREATING_LEVEL;
                } break;
                case Snake::ROTATE_LEFT:
                    curr_level.snake.rotate(Snake::LEFT);
                    break;
                case Snake::ROTATE_RIGHT:
                    curr_level.snake.rotate(Snake::RIGHT);
                    break;
            }

        } break;

        case LOSE_GAME:
            m_state = END;
            break;

        case WIN_GAME:
            m_state = END;
            break;

        default:
            break;
    }
}

void SnazeManager::render() const {
    switch (m_state) {
        case START:
            break;

        case WELCOME:
            print_message();
            break;

        case READING:
            print_summary();
            break;

        case CREATING_LEVEL:
            break;

        case THINKING:
            break;

        case MOVING:
            std::this_thread::sleep_for(milliseconds{1000/m_fps});
            print_map();
            break;

        case LOSE_GAME:
            std::cout << "You lost!\n";
            break;

        case WIN_GAME:
            std::cout << "You won!\n";
            break;

        default:
            break;
    }
}

void SnazeManager::error(const std::string& error_message) const {
    switch (m_state) {
        case START:
            std::cerr << ">>> Error handling arguments: " << error_message << std::endl;
            print_usage();
            break;
        case READING:
            std::cerr << ">>> Error reading file: " << error_message << std::endl;
            break;
        default:
            std::cerr << ">>> Error during execution: " << error_message << std::endl;
            break;
    }
    exit(1);
}

void SnazeManager::print_usage() const {
    std::cout << "Usage: snaze [<options>] <input_level_file>\n"
              << "\tGame simulation options:\n"
              << "\t\t-h  \t(or --help)       \t       \tPrint this help text.\n"
              << "\t\t-f  \t(or --fps)        \t<num>  \tNumber of frames (board) presented per second.\n"
              << "\t\t    \t                  \t       \tValid range is [1, 24]. Default = 24.\n"
              << "\t\t-lx \t(or --lives)      \t<num>  \tNumber of lives the snake shall have.\n"
              << "\t\t    \t                  \t       \tValid range is [1, 50]. Default = 5.\n"
              << "\t\t-nf \t(or --food)       \t<num>  \tNumber of food pellets for the entire simulation.\n"
              << "\t\t    \t                  \t       \tValid range is [1, 100]. Default = 10.\n"
              << "\t\t-p  \t(or --playertype) \t<type> \tType of snake intelligence.\n"
              << "\t\t    \t                  \t       \tValid types are random, backtracking. Default = backtracking.\n";
}

void SnazeManager::print_message() const {
    std::cout << "--> Welcome to the classic Snake Game <---\n";
    std::cout << "\t copyright IMD/UFRN 2021.\n\n";
}

void SnazeManager::print_summary() const {
    std::cout << "--------------------------------------------------------\n";
    std::cout << " Levels loaded: " << m_levels.size() << " | Snake Lives: " << m_lives << " | Apples to eat: " << m_quant_food << '\n';
    std::cout << " Clear all levels to win the game. Good luck!!!\n";
    std::cout << "--------------------------------------------------------\n";
}

void SnazeManager::print_map() const {
    const auto& level{m_levels.front()};

    for (const auto& line: level.level_map) {
        for (const auto& tile: line) {
            switch (tile) {
                case Level::PATH:
                    std::cout << ' ';
                    break;
                case Level::WALL:
                    std::cout << '#';
                    break;
                case Level::INVISIBLE_WALL:
                    std::cout << ' ';
                    break;
                // case Level::SNAKE_HEAD:
                //     std::cout << '>';
                //     break;
                case Level::SNAKE:
                    std::cout << '*';
                    break;
                case Level::FOOD:
                    std::cout << '@';
                    break;
            }
        }
        std::cout << '\n';
    }
}
/// teste
