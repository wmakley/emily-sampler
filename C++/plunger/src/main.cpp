#include "Game.hpp"
#include "MoveToRoom.hpp"
#include <iostream>
#include <exception>

int main()
{
  Game game;

  std::cout << "Game start!" << std::endl;
  MoveToRoom(KITCHEN).execute(game);

  bool done = false;
  char key;
  while (!done)
  {
    game.print_options();

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
      const size_t option_index = (key - '0');

      try
      {
        game.execute_option(option_index);
      }
      catch (std::out_of_range e)
      {
        std::cout << "Unrecognized option: " << option_index << std::endl;
      }
    }
    else
    {
      std::cout << "Unrecognized input: '" << key << "'" << std::endl;
    }
  }

  return 0;
}
