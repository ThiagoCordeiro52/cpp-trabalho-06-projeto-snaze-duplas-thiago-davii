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

/// This the main class that represents the Snaze manager.
class SnazeManager {
    public:
        using number_type = unsigned int;

        enum GameState{
            START,          //!< Initial game state.
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
        void error(const std::string& error_message) const;

        GameState m_state {START};
        // std::vector<Level> levels;
        number_type m_fps;
        number_type m_quant_food {10};
        number_type m_lives {5};
        std::string m_filename;
        // PlayerType player_type;
};
#endif
