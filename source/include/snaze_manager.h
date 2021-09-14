#ifndef SNAZE_MANAGER_H
#define SNAZE_MANAGER_H

#include <iostream>
using std::cout;
#include <string>
using std::string;
#include <fstream>
using std::ifstream;
#include <thread>
#include <queue>
using std::queue;
#include <iomanip>

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
            NEW_LEVEL,      //!< New level
            CRASH,          //!< Handling snake crash
            WIN_GAME,       //!< Handling the case where the player wins
            LOSE_GAME,      //!< Handling the case where the player loses
            END,            //!< Final game state.
        };
        
        // Constructor
        SnazeManager(int argc, char *argv[]);

        // Public methods

        /**
        *
        * @return true if state is END and false otherwise
        *
        */
        bool has_ended() const { return m_state == END; }
        
        /**
        * @brief performs game processing in each state
        *
        */
        void process();

        /**
        * @brief update the game in each state
        *
        */
        void update();

        /**
        * @brief sends information to standart output in each state
        *
        */
        void render() const;
  
    private:
        // Private methods
        /**
        * @brief print game usage options
        *
        */
        void print_usage() const;

        /**
        * @brief print welcome message
        *
        */
        void print_message() const;

        /**
        * @brief print the summary of the game
        *
        */
        void print_summary() const;

        /**
        * @brief print the map of the game
        *
        */
        void print_map() const;

        /**
         * @brief prints a error message and exits the program
         * 
         * @param error_message the error message 
         */
        void error(const std::string& error_message) const;

        // Attributes
        GameState m_state {START};
        std::queue<Level> m_levels;
        number_type m_fps {24};
        number_type m_quant_food {10};
        number_type m_total_lives {5};
        number_type m_remaining_lives;
        number_type m_score {0};
        std::string m_filename;
        PlayerType m_player_type {BACKTRACKING};
        Snake::Instruction m_next_instruction;
};
#endif
