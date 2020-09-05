#ifndef IGAME_HPP
#define IGAME_HPP

#include <memory>
#include "RoomId.hpp"

class Player;
class Item;
class Room;

const RoomId
    KITCHEN = 1,
    BATHROOM = 2,
    LIVING_ROOM = 3,
    BACK_DECK = 4;

class IGame
{
public:
  virtual ~IGame() {}

  virtual std::shared_ptr<Room> get_room(const RoomId id) = 0;
  virtual std::shared_ptr<const Room> get_room(const RoomId id) const = 0;
  virtual std::shared_ptr<Room> current_room() = 0;
  virtual std::shared_ptr<const Room> current_room() const = 0;

  virtual Player &player() = 0;
  virtual const Player &player() const = 0;
};

#endif
