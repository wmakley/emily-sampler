#ifndef USE_ITEM_HPP
#define USE_ITEM_HPP

#include <memory>
#include "Command.hpp"

class Item;

class UseItem : public Command
{
private:
  std::shared_ptr<Item> item_;

public:
  UseItem(std::shared_ptr<Item> item);
  void execute(IGame &game) override;
  const std::string desc(const IGame &game) const override;
};

#endif
