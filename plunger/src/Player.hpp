#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "Inventory.hpp"
#include "RoomId.hpp"

class Player {
private:
  RoomId room_id_;

public:
  Inventory inventory;

  Player();

  void move_to(const RoomId room_id);
  RoomId room_id() const;

  friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

std::ostream& operator<<(std::ostream& os, const Player& player);

#endif
