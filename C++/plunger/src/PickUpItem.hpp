#ifndef PICK_UP_ITEM_HPP
#define PICK_UP_ITEM_HPP

#include <memory>
#include <string>
#include "Command.hpp"
#include "IGame.hpp"

class Room;
class Item;

class PickUpItem : public Command
{
private:
  std::shared_ptr<Item> item_;

public:
  PickUpItem(std::shared_ptr<Item> item);
  ~PickUpItem();
  void execute(IGame &game) override;
  const std::string desc(const IGame &game) const override;
};

#endif
