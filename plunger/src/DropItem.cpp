#include "DropItem.hpp"

DropItem::DropItem(std::shared_ptr<Room> room, std::shared_ptr<Item> item):
  room_(room),
  item_(item)
{}

DropItem::~DropItem() {}

void DropItem::execute(IGame& game) {
  game.player()->inventory.remove( item_ );
  room_->inventory.add( item_ );
  game.rebuild_options();
  std::cout << "Dropped " << item_->name() << "." << std::endl;
  item_->dropped();
}

const std::string DropItem::desc(IGame&) {
  return "Drop " + item_->name();
}
