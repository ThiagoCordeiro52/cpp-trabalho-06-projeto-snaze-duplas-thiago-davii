#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <utility>
#include <vector>
using std::cout;
using std::cin;
using std::endl;

/// This the main class that represents the snake.
class snake {
  public:
    using positions = std::vector<std::pair<int, int>>;
    unsigned int lifes = 5;

    positions body;
};
#endif