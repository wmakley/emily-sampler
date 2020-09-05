#include "MoveToRoom.hpp"
#include "Room.hpp"
#include "Player.hpp"

MoveToRoom::MoveToRoom(RoomId room_id)
    : Command(),
      room_id_(room_id)
{
}

MoveToRoom::~MoveToRoom() {}

void MoveToRoom::execute(IGame &game)
{
  // std::cout << "move_player_to(" << room_id << ")" << std::endl;
  game.set_current_room(room_id_);
  game.player()->move_to(room_id_);
  game.rebuild_options();
  game.print_room_desc();
}

const std::string MoveToRoom::desc(IGame &game) const
{
  return "Walk to " + game.get_room(room_id_)->name();
}
