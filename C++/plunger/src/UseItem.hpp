#ifndef USE_ITEM_HPP
#define USE_ITEM_HPP

#include <memory>
#include "Command.hpp"
#include "Item.hpp"

class UseItem: public Command {
private:
  std::shared_ptr<Item> item_;

public:
  UseItem(std::shared_ptr<Item> item);
  void execute(IGame& game);
  const std::string desc(IGame& game);
};

#endif
