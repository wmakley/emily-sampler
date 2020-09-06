#include <iostream>
#include <string>
#include "Game.hpp"
#include "UI.hpp"

void bad_input(const std::string &input)
{
  std::cout << "Unrecognized input: '" << input << "'" << std::endl;
}

int main()
{
  Game game;
  UI ui;

  ui.start_game(game);

  bool done = false;
  std::string input;
  while (!done)
  {
    ui.rebuild_options(game);
    if (game.temp_flag(IGame::ROOM_CHANGED))
    {
      ui.describe_room(game);
    }
    if (game.temp_flag(IGame::ROOM_INVENTORY_CHANGED) || game.temp_flag(IGame::ROOM_CHANGED))
    {
      ui.list_room_objects(game);
    }
    ui.print_options(game);
    game.reset_temp_flags();

    std::cout << std::endl
              << ">> ";

    std::cin >> input;
    const char key = input[0];

    if (key == 'q' || key == 'Q')
    {
      done = true;
      std::cout << "Quitting!" << std::endl;
    }
    else if (key >= '0' && key <= '9')
    {
      try
      {
        const size_t number = std::stol(input);
        const size_t option_index = number - 1;
        ui.execute_option(option_index, game);
      }
      catch (std::invalid_argument)
      {
        bad_input(input);
      }
      catch (std::out_of_range)
      {
        std::cout << "Number too large or small: '" << input << "'" << std::endl;
      }
    }
    else
    {
      bad_input(input);
    }
  }

  return 0;
}
