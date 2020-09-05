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
  Room *room_;
  std::shared_ptr<Item> item_;

public:
  PickUpItem(Room *room, std::shared_ptr<Item> item);
  ~PickUpItem();
  void execute(IGame &game) override;
  const std::string desc(IGame &game) const override;
};

#endif
