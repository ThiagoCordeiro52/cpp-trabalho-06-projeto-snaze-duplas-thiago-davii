#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <utility>
#include <vector>
using std::cout;
using std::cin;
using std::endl;

/// This the main class that represents the snake.
class Snake {
  public:
    using position = std::vector<std::pair<int, int>>;

  private:
    position m_body;
    unsigned int m_lives;
};
#endif
