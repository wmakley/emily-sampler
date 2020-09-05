#include <iostream>
#include <exception>
#include <memory>
#include "Game.hpp"
#include "UI.hpp"

int main()
{
  Game game;
  UI ui;

  ui.start_game(game);

  bool done = false;
  char key;
  while (!done)
  {
    ui.rebuild_options(game);
    ui.print_options(game);

    std::cout << std::endl
              << ">> ";
    std::cin >> key;

    if (key == 'q' || key == 'Q')
    {
      done = true;
      std::cout << "Quitting!" << std::endl;
    }
    else if (key >= '0' && key <= '9')
    {
      const size_t option_index = (key - '0') - 1;
      ui.execute_option(option_index, game);
    }
    else
    {
      std::cout << "Unrecognized input: '" << key << "'" << std::endl;
    }
  }

  return 0;
}
