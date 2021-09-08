#include "snaze_manager.h"

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
                // set player_type;
            } else if (arg2 == "backtracking" or arg2 == "b") {
                // set player_type;
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

        case READING: {
            number_type rows;
            number_type columns;
            std::ifstream file{m_filename};

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
            for (int i = 0; i < rows; i++) {
                std::string line;
                getline(file >> std::ws, line);
                for(int j = 0; j < columns; j++) {
                    Level::Tile tile;
                    switch (line[j]) {
                        case '#':
                            tile = Level::Tile::WALL;
                            break;
                        case '*':
                            tile = Level::Tile::SNAKE_HEAD;
                            break;
                        case '.':
                            tile = Level::Tile::INVISIBLE_WALL;
                            break;
                        default:
                            error("invalid character in " + m_filename);
                            break;
                    }
                    level.level_map[i].push_back(tile);
                }
            }
        } break;

        case CREATING_LEVEL:
            break;

        default:
            break;
    }
}

void SnazeManager::update() {
    switch (m_state) {
        case START:
            m_state = READING;
            break;

        case READING:
            m_state = CREATING_LEVEL;
            break;

        default:
            break;
    }
}

void SnazeManager::print_message() const {
    std::cout << "--> Welcome to the classic Snake Game <---\n";
    std::cout << "\t copyright IMD/UFRN 2021.\n";
    std::cout << "---------------------------------------------------\n";
}

void SnazeManager::print_summary() const {
    std::cout << " Levels loaded: " + m_levels.size() + " | Snake Lives: " + m_lives + " | Apples to eat: " + m_quant_food + "\n";
    std::cout << " Clear all levels to win the game. Good luck!!!"
    std::cout << "---------------------------------------------------\n";
}

void SnazeManager::render() const {
    switch (m_state) {
        case START:
            print_message();
            break;

        case READING:
            print_summary();
            break;

        default:
            break;
    }
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
/// teste
