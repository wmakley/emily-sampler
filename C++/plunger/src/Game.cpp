#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include "Game.hpp"
#include "Item.hpp"
#include "Room.hpp"

Game::Game()
    : flags_({{IGame::RAT_REMOVED, false}})
{
  rooms_[KITCHEN] = std::make_shared<Kitchen>(std::initializer_list<RoomId>{
      BATHROOM,
      LIVING_ROOM});

  rooms_[BATHROOM] = std::make_shared<Room>(
      BATHROOM, "Bathroom", "It is reasonably clean.",
      std::initializer_list<RoomId>{
          KITCHEN,
          LIVING_ROOM},
      std::initializer_list<std::shared_ptr<Item>>{
          std::make_shared<Plunger>()});

  rooms_[LIVING_ROOM] = std::make_shared<Room>(
      LIVING_ROOM, "Living Room", "You are standing near the coffee table.\nIt is cluttered.\nYou can see the kitchen, bathroom, and back door.",
      std::initializer_list<RoomId>{
          BACK_DECK,
          BATHROOM,
          KITCHEN,
      });

  rooms_[BACK_DECK] = std::make_shared<Room>(
      BACK_DECK, "Back deck", "You are standing just outside of the sliding door.\nIt is sunny and beautiful.",
      std::initializer_list<RoomId>{
          LIVING_ROOM});
}

Game::~Game() {}

std::shared_ptr<Room> Game::get_room(const RoomId room_id)
{
  return rooms_[room_id];
}

std::shared_ptr<const Room> Game::get_room(const RoomId room_id) const
{
  return std::const_pointer_cast<const Room>(rooms_.at(room_id));
}

std::shared_ptr<Room> Game::current_room()
{
  return get_room(player_.room_id());
}

std::shared_ptr<const Room> Game::current_room() const
{
  return get_room(player_.room_id());
}

Player &Game::player()
{
  return player_;
}

const Player &Game::player() const
{
  return player_;
}

bool Game::flag(IGame::Flag key) const
{
  try
  {
    return flags_.at(key);
  }
  catch (std::out_of_range)
  {
    return false;
  }
}

void Game::set_flag(IGame::Flag key, bool value)
{
  flags_[key] = value;
}

bool Game::temp_flag(IGame::TempFlag key) const
{
  try
  {
    return temp_flags_.at(key);
  }
  catch (std::out_of_range)
  {
    return false;
  }
}

void Game::set_temp_flag(IGame::TempFlag key)
{
  temp_flags_[key] = true;
}

void Game::reset_temp_flags()
{
  temp_flags_.clear();
}
