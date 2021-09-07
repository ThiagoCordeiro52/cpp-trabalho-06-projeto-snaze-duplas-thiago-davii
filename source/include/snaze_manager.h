#ifndef SNAZE_MANAGER_H
#define SNAZE_MANAGER_H

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

/// This the main class that represents the Snaze manager.
class SnazeManager {
  public:
      enum GameState{
          START = 0,              //!< Initial game state.
          READING,                //!< Reading data files.
          QUIT_GAME,              //!< User choose to leave the game.
          INVALID_INPUT,          //!< Readed invalid datas.
          END,                    //!< Final game state.
      };

      // methods
      void initialize( int argc, char *argv[] );
      bool quit_game(void){ return game_state == END; }
      void process_events(void);
      void update(void);
      void render(void);
  
  private:
      GameState game_state;
};

#endif
