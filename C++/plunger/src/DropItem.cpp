#include "DropItem.hpp"
#include "Item.hpp"
#include "Player.hpp"
#include "Room.hpp"

DropItem::DropItem(std::shared_ptr<Item> item)
    : item_(item)
{
}

DropItem::DropItem(const DropItem &other)
    : item_(other.item_)
{
}

DropItem::~DropItem() {}

void DropItem::execute(IGame &game)
{
  game.player().inventory.remove(item_);
  game.current_room()->inventory.add(item_);
  std::cout << "Dropped " << item_->name() << "." << std::endl
            << "You no longer have a " << item_->name() << "." << std::endl;

  game.set_temp_flag(IGame::PLAYER_INVENTORY_CHANGED);
  game.set_temp_flag(IGame::ROOM_INVENTORY_CHANGED);
}

const std::string DropItem::desc(const IGame &) const
{
  return "Drop " + item_->name();
}
