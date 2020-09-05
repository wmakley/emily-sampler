#include "MoveToRoom.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Item.hpp"

MoveToRoom::MoveToRoom(RoomId room_id)
    : Command(),
      room_id_(room_id)
{
}

MoveToRoom::~MoveToRoom() {}

void MoveToRoom::execute(IGame &game)
{
  // std::cout << "move_player_to(" << room_id_ << ")" << std::endl;
  std::shared_ptr<Room> room = game.get_room(room_id_);
  game.player().move_to(room->id());

  std::cout << "You are in the " << room->name() << "." << std::endl
            << room->desc(game) << std::endl;

  if (room->inventory.has_items())
  {
    std::cout << std::endl
              << "Objects:" << std::endl
              << std::endl;

    for (auto const &item : room->inventory)
    {
      std::cout << "  * " << item->name() << std::endl;
    }
    // std::cout << std::endl;
  }
}

const std::string MoveToRoom::desc(const IGame &game) const
{
  return "Walk to " + game.get_room(room_id_)->name();
}
