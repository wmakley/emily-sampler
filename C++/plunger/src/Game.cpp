#include <algorithm>
#include <iostream>
#include <string>
#include "Game.hpp"
#include "Item.hpp"
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
  current_room_ = rooms_[room_id];
}
