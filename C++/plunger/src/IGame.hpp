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
  virtual void start() = 0;
  virtual Room *get_room(const RoomId id) = 0;
  virtual Player *player() = 0;
  virtual Room *current_room() = 0;
  virtual void move_player_to(const RoomId room_id) = 0;
  virtual void print_room_desc() = 0;
  virtual void print_options() = 0;
  virtual void rebuild_options() = 0;
};

#endif
