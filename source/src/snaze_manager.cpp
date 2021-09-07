#include "snaze_manager.h"

//====================[ SNAZE MANAGER METHODS ]========================//
std::string to_lowercase(const std::string& str) {
    auto clone = str;
    for (auto& c : clone) {
        c = ::tolower(c);
    };
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

            // if (m_fps < 1 || m_fps > 15)
            //     error("the argument of " + arg + " is out of range");
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

            // if (m_lives < 1 || m_lives > 15)
            //     error("the argument of " + arg + " is out of range");
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

            // if (m_quant_food < 1 || m_quant_food > 15)
            //     error("the argument of " + arg + " is out of range");
        } else if (arg == "-p" or arg == "--playertype") {
            if (++i >= argc)
                error("argument of " + arg + " missing");

            std::string arg2 {argv[i]};

            if (arg2 == "random") {
                // set player_type;
            } else if (arg2 == "backtracking") {
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

void SnazeManager::update() {
    switch (m_state) {
        case GameState::START:
            m_state = GameState::READING;
            break;

        case GameState::READING:
            break;

        default:
            break;
    }
}

void SnazeManager::print_message() const {
  std::cout << "--> Welcome to the classic Snake Game <---\n";
  std::cout << "\t copyright IMD/UFRN 2020.\n";
  std::cout << "---------------------------------------------------\n\n";
}

void SnazeManager::process() {
    switch (m_state) {
        case GameState::START:
            break;

        case GameState::READING:
            break;

        default:
            break;
    }
}

void SnazeManager::render() const {
    switch (m_state) {
        case GameState::START:
            print_message();
            break;

        case GameState::READING:
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
              << "\t\t-lx \t(or --lives)      \t<num>  \tNumber of lives the snake shall have. Default = 5.\n"
              << "\t\t-nf \t(or --food)       \t<num>  \tNumber of food pellets for the entire simulation. Default = 10.\n"
              << "\t\t-p  \t(or --playertype) \t<type> \tType of snake intelligence: random, backtracking. Default = backtracking\n";
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
