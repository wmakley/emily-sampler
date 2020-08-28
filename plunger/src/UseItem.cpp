#include "UseItem.hpp"

UseItem::UseItem(std::shared_ptr<Item> item): item_(item) {}

void UseItem::execute(IGame& game) {
  std::cout << ">> Use " << item_->name() << std::endl;

  // TODO: don't hardcode this, would like to be able to call a virtual method
  // on the item, but item can't depend on Inventory. :(
  if (item_->name() == "Plunger") {
    Room *room = game.current_room();
    if (room->name() == "Kitchen") {
      Kitchen *kitchen = (Kitchen *)room;
      kitchen->kill_rat();

      Player *player = game.player();
      player->inventory.remove( item_ );
      player->inventory.add( std::make_shared<PlungerWithRat>() );
      std::cout << "You suction the rat in your plunger." << std::endl
        << "You are carrying a plunger with a rat in it." << std::endl;

      game.rebuild_options();
    }
    else if (room->name() == "Bathroom") {
      std::cout << "You plunge the toilet, even though it doesn't need it." << std::endl;
    }
    else {
      std::cout << "That doesn't seem to do anything here." << std::endl;
    }
  }
  else if (item_->name() == "Plunger with rat") {
    Room *room = game.current_room();
    if (room->name() == "Back deck") {
      Player *player = game.player();
      player->inventory.remove( item_ );
      player->inventory.add( std::make_shared<Plunger>() );
      room->inventory.add( std::make_shared<Item>("Sun-bathing rat") );
      std::cout << "You release the rat. It is now sun-bathing on the back deck." << std::endl
        << "You are now carrying an empty plunger." << std::endl;
      game.rebuild_options();
    } else {
      std::cout << "That doesn't seem to do anything here." << std::endl;
    }
  }
  else {
    std::cout << "That item cannot be used." << std::endl;
  }
}

const std::string UseItem::desc(IGame&) {
  return "Use " + item_->name();
}
