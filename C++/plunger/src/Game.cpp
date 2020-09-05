#include <algorithm>
#include <iostream>
#include <string>
#include "Game.hpp"
#include "Item.hpp"
#include "MoveToRoom.hpp"
#include "PickUpItem.hpp"
#include "DropItem.hpp"
#include "UseItem.hpp"
#include "Room.hpp"

Game::Game() : current_room_(NULL)
{
  rooms_[KITCHEN] = std::make_shared<Kitchen>();

  rooms_[BATHROOM] = std::make_shared<Room>(BATHROOM, "Bathroom", "It is reasonably clean.");
  rooms_[BATHROOM]->inventory.add(std::make_shared<Plunger>());

  rooms_[LIVING_ROOM] = std::make_shared<Room>(LIVING_ROOM, "Living Room", "You are standing near the coffee table.\nIt is cluttered.\nYou can see the kitchen, bathroom, and back door.");
  rooms_[BACK_DECK] = std::make_shared<Room>(BACK_DECK, "Back deck", "You are standing just outside of the sliding door.\nIt is sunny and beautiful.");

  rooms_[KITCHEN]->add_link(BATHROOM);
  rooms_[KITCHEN]->add_link(LIVING_ROOM);
  rooms_[BATHROOM]->add_link(KITCHEN);
  rooms_[BATHROOM]->add_link(LIVING_ROOM);
  rooms_[LIVING_ROOM]->add_link(BATHROOM);
  rooms_[LIVING_ROOM]->add_link(KITCHEN);
  rooms_[LIVING_ROOM]->add_link(BACK_DECK);
  rooms_[BACK_DECK]->add_link(LIVING_ROOM);
}

Game::~Game() {}

Room *Game::get_room(const RoomId room_id)
{
  return rooms_[room_id].get();
}

Player *Game::player()
{
  return &player_;
}

Room *Game::current_room()
{
  return current_room_.get();
}

void Game::set_current_room(const RoomId room_id)
{
  std::shared_ptr<Room> room = rooms_[room_id];
  current_room_ = room;
}

void Game::rebuild_options()
{
  options_.clear();

  for (const RoomId link : current_room_->links)
  {
    options_.push_back(std::make_shared<MoveToRoom>(link));
  }

  for (auto item : current_room_->inventory)
  {
    options_.push_back(std::make_shared<PickUpItem>(current_room_, item));
  }

  for (auto item : player_.inventory)
  {
    if (item->usable(*this))
    {
      options_.push_back(std::make_shared<UseItem>(item));
    }
    options_.push_back(std::make_shared<DropItem>(current_room_, item));
  }
}

void Game::execute_option(const size_t index)
{
  std::shared_ptr<Command> option = options_.at(index);
  option->execute(*this);
}

void Game::print_room_desc()
{
  std::cout << "You are in the " << current_room_->name() << "." << std::endl
            << current_room_->desc() << std::endl
            << std::endl;
}

void Game::print_options()
{
  std::cout << std::endl;

  if (current_room_->inventory.has_items())
  {
    std::cout << "Objects:" << std::endl
              << std::endl;

    for (auto const &item : current_room_->inventory)
    {
      std::cout << "  * " << item->name() << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << "Options:" << std::endl
            << std::endl;

  int i = 1;
  for (auto const &command : options_)
  {
    std::cout << "  (" << i << "): " << command->desc(*this) << std::endl;
    i++;
  }

  std::cout << "  (q): Quit Game" << std::endl;
}
