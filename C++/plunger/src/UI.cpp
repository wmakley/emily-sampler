#include <exception>
#include <iostream>
#include "UI.hpp"
#include "Item.hpp"
#include "Command.hpp"
#include "MoveToRoom.hpp"
#include "PickUpItem.hpp"
#include "DropItem.hpp"
#include "UseItem.hpp"
#include "Player.hpp"
#include "Room.hpp"

void UI::start_game(IGame &game) const
{
  std::cout << "Game start!" << std::endl;
  MoveToRoom(KITCHEN).execute(game);
}

void UI::rebuild_options(IGame &game)
{
  Room *room = game.current_room();
  Player *player = game.player();

  options_.clear();

  for (const RoomId link : room->links)
  {
    options_.push_back(std::make_shared<MoveToRoom>(link));
  }

  for (auto item : room->inventory)
  {
    options_.push_back(std::make_shared<PickUpItem>(room, item));
  }

  for (auto item : player->inventory)
  {
    if (item->usable(game))
    {
      options_.push_back(std::make_shared<UseItem>(item));
    }
    options_.push_back(std::make_shared<DropItem>(room, item));
  }
}

void UI::print_options(IGame &game)
{
  std::cout << "Options:" << std::endl
            << std::endl;

  int i = 1;
  for (auto const &command : options_)
  {
    std::cout << "  (" << i << "): " << command->desc(game) << std::endl;
    i++;
  }

  std::cout << "  (q): Quit Game" << std::endl;
}

void UI::execute_option(const size_t index, IGame &game)
{
  // std::cout << "execute_option(" << index << ")" << std::endl;
  try
  {
    std::shared_ptr<Command> option = options_.at(index);
    option->execute(game);
  }
  catch (std::out_of_range e)
  {
    std::cout << "Unrecognized option: " << (index + 1) << std::endl;
  }
}
