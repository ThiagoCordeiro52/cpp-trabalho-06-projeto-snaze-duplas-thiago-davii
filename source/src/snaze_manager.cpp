#include "snaze_manager.h"

//====================[ SNAZE MANAGER METHODS ]========================//
void SnazeManager::print_message(void) {
  std::cout << "--> Welcome to the classic Snake Game <---\n";
  std::cout << "\t copyright IMD/UFRN 2020.\n";
  std::cout << "---------------------------------------------------\n\n";
}

void SnazeManager::update(void) {
  switch (m_state)
  {
  case GameState::START:
    m_state = GameState::READING;
    break;
  
  case GameState::READING:
  break;
  
  default:
    break;
  }
}

void SnazeManager::process_events(void) {
  switch (m_state)
  {
  case GameState::START:
    break;
  
  case GameState::READING:
  break;
  
  default:
    break;
  }
}

void SnazeManager::render(void) {
  switch (m_state)
  {
  case GameState::START:
    print_message();
    break;
  
  case GameState::READING:
  break;
  
  default:
    break;
  }
}
