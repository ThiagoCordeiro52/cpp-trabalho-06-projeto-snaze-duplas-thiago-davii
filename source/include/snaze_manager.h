#ifndef SNAZE_MANAGER_H
#define SNAZE_MANAGER_H

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <random>
using std::default_random_engine;
using std::uniform_int_distribution;
#include <chrono>
using std::chrono::system_clock;

#include "level.h"

/// This the main class that represents the Snaze manager.
class SnazeManager {
    public:
        using number_type = unsigned int;

        enum GameState{
            START,          //!< Initial game state.
            WELCOME,        //!< Print welcome message.
            READING,        //!< Reading data files.
            CREATING_LEVEL, //!< Setting up the level
            THINKING,       //!< Deciding next move
            MOVING,         //!< Moving the snake
            CRASH,          //!< Handling snake crash
            QUIT_GAME,      //!< User choose to leave the game.
            END,            //!< Final game state.
        };
        
        // Constructor
        SnazeManager(int argc, char *argv[]);

        // methods
        bool has_ended() const { return m_state == END; }
        void process();
        void update();
        void render() const;
  
    private:
        void print_usage() const;
        void print_message() const;
        void print_summary() const;
        void print_map() const;
        void error(const std::string& error_message) const;

        GameState m_state {START};
        std::vector<Level> m_levels;
        number_type m_fps {24};
        number_type m_quant_food {10};
        number_type m_lives {5};
        std::string m_filename;
        // PlayerType player_type;
        number_type m_curr_level {0};
};
#endif
