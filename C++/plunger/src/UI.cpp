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

void UI::rebuild_options(const IGame &game)
{
  std::shared_ptr<const Room> room = game.current_room();
  const Player &player = game.player();

  options_.clear();

  for (const RoomId link : room->links)
  {
    options_.push_back(std::make_shared<MoveToRoom>(link));
  }

  for (auto item : room->inventory)
  {
    options_.push_back(std::make_shared<PickUpItem>(item));
  }

  for (auto item : player.inventory)
  {
    if (item->usable(game))
    {
      options_.push_back(std::make_shared<UseItem>(item));
    }
    options_.push_back(std::make_shared<DropItem>(item));
  }
}

void UI::describe_room(const IGame &game) const
{
  auto room = game.current_room();
  std::cout << "You are in the " << room->name() << "." << std::endl
            << room->desc(game) << std::endl;
}

void UI::list_room_objects(const IGame &game) const
{
  auto room = game.current_room();
  if (room->inventory.has_items())
  {
    std::cout << std::endl
              << "Objects:" << std::endl
              << std::endl;

    for (auto const &item : room->inventory)
    {
      std::cout << "  * " << item->name() << std::endl;
    }
    // std::cout << std::endl;
  }
}

void UI::print_options(const IGame &game) const
{
  std::cout << std::endl
            << "Options:" << std::endl
            << std::endl;

  int i = 1;
  for (auto const &command : options_)
  {
    std::cout << "  (" << i << "): " << command->desc(game) << std::endl;
    i++;
  }

  std::cout << "  (q): Quit Game" << std::endl;
}

void UI::execute_option(const size_t index, IGame &game) const
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
