#include "MoveToRoom.hpp"

MoveToRoom::MoveToRoom(RoomId room_id): Command(), room_id_(room_id) {}
MoveToRoom::~MoveToRoom() {}

void MoveToRoom::execute(IGame& game) {
  game.move_player_to(room_id_);
}

const std::string MoveToRoom::desc(IGame& game) {
  return "Walk to " + game.get_room(room_id_)->name();
}
