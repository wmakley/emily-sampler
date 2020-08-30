#include "Player.hpp"

Player::Player() : room_id_(1)
{}

void Player::move_to(const RoomId room_id) {
  room_id_ = room_id;
}

RoomId Player::room_id() const {
  return room_id_;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
  os << "Player(current_room:" << player.room_id_ << ")";
  return os;
}
