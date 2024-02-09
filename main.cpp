#include "Game.hpp"
#include <SFML/Graphics.hpp>

int main() {

  Game g("config.txt");
  g.run();

  return 0;
}
