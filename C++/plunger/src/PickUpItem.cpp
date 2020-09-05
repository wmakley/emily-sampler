#include "PickUpItem.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Room.hpp"

PickUpItem::PickUpItem(std::shared_ptr<Item> item)
    : item_(item)
{
}

PickUpItem::~PickUpItem() {}

void PickUpItem::execute(IGame &game)
{
  game.current_room()->inventory.remove(item_);
  game.player().inventory.add(item_);
  std::cout << "Picked up " << item_->name() << "." << std::endl
            << "You now have a " << item_->name() << "." << std::endl;
}

const std::string PickUpItem::desc(const IGame &) const
{
  return std::string("Pick up ") + item_->name();
}
