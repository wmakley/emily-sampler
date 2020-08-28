#ifndef PICK_UP_ITEM_HPP
#define PICK_UP_ITEM_HPP

#include <memory>
#include <string>
#include "Command.hpp"
#include "Room.hpp"
#include "IGame.hpp"

class PickUpItem: public Command {
private:
  std::shared_ptr<Room> room_;
  std::shared_ptr<Item> item_;

public:
  PickUpItem(std::shared_ptr<Room> room, std::shared_ptr<Item> item);
  ~PickUpItem();
  void execute(IGame& game);
  const std::string desc(IGame& game);
};

#endif
