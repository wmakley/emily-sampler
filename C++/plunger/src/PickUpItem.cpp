#include "PickUpItem.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Room.hpp"

PickUpItem::PickUpItem(Room *room, std::shared_ptr<Item> item)
    : room_(room),
      item_(item)
{
}

PickUpItem::~PickUpItem() {}

void PickUpItem::execute(IGame &game)
{
  room_->inventory.remove(item_);
  game.player()->inventory.add(item_);
  std::cout << "Picked up " << item_->name() << "." << std::endl
            << "You now have a " << item_->name() << "." << std::endl;
}

const std::string PickUpItem::desc(IGame &) const
{
  return std::string("Pick up ") + item_->name();
}
