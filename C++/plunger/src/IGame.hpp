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
  enum Flag
  {
    RAT_REMOVED
  };

  enum TempFlag
  {
    ROOM_CHANGED,
    ROOM_INVENTORY_CHANGED,
    PLAYER_INVENTORY_CHANGED,
  };

  virtual ~IGame() {}

  virtual std::shared_ptr<Room> get_room(const RoomId id) = 0;
  virtual std::shared_ptr<const Room> get_room(const RoomId id) const = 0;
  virtual std::shared_ptr<Room> current_room() = 0;
  virtual std::shared_ptr<const Room> current_room() const = 0;

  virtual Player &player() = 0;
  virtual const Player &player() const = 0;

  virtual bool flag(Flag) const = 0;
  virtual void set_flag(Flag, bool) = 0;

  virtual bool temp_flag(TempFlag) const = 0;
  virtual void set_temp_flag(TempFlag) = 0;
  virtual void reset_temp_flags() = 0;
};

#endif
