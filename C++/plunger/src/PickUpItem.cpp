#include "PickUpItem.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Room.hpp"

PickUpItem::PickUpItem(std::shared_ptr<Room> room, std::shared_ptr<Item> item)
    : room_(room),
      item_(item)
{
}

PickUpItem::~PickUpItem() {}

void PickUpItem::execute(IGame &game)
{
  room_->inventory.remove(item_);
  game.player()->inventory.add(item_);
  game.rebuild_options();
  std::cout << "Picked up " << item_->name() << "." << std::endl;
  item_->picked_up(game);
}

const std::string PickUpItem::desc(IGame &) const
{
  return std::string("Pick up ") + item_->name();
}
