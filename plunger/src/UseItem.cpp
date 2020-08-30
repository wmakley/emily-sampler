#include "UseItem.hpp"
#include "Item.hpp"

UseItem::UseItem(std::shared_ptr<Item> item) : item_(item) {}

void UseItem::execute(IGame &game)
{
  std::cout << ">> Use " << item_->name() << std::endl;

  if (item_->usable(game))
  {
    item_->use(game);
  }
  else
  {
    std::cout << "That doesn't seem to do anything here." << std::endl;
  }
}

const std::string UseItem::desc(IGame &)
{
  return "Use " + item_->name();
}
