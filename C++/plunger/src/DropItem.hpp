#ifndef DROP_ITEM_HPP
#define DROP_ITEM_HPP

#include <memory>
#include <string>
#include "Command.hpp"
#include "IGame.hpp"

class Room;
class Item;

class DropItem : public Command
{
private:
  std::shared_ptr<Room> room_;
  std::shared_ptr<Item> item_;

public:
  DropItem(std::shared_ptr<Room> room, std::shared_ptr<Item> item);
  ~DropItem();
  void execute(IGame &game) override;
  const std::string desc(IGame &game) const override;
};

#endif
