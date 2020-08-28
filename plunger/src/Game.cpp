#include <algorithm>
#include <iostream>
#include <string>
#include "Game.hpp"
#include "Item.hpp"
#include "MoveToRoom.hpp"
#include "PickUpItem.hpp"
#include "DropItem.hpp"
#include "UseItem.hpp"

static const RoomId
  KITCHEN = 1,
  BATHROOM = 2,
  LIVING_ROOM = 3,
  BACK_DECK = 4;

Game::Game(): current_room_(NULL) {
  rooms_[KITCHEN] = std::make_shared<Kitchen>();

  rooms_[BATHROOM] = std::make_shared<Room>("Bathroom", "It is reasonably clean." );
  rooms_[BATHROOM]->inventory.add( std::make_shared<Plunger>() );

  rooms_[LIVING_ROOM] = std::make_shared<Room>("Living Room", "You are standing near the coffee table.\nIt is cluttered.\nYou can see the kitchen, bathroom, and back door.");
  rooms_[BACK_DECK] = std::make_shared<Room>("Back deck", "You are standing just outside of the sliding door.\nIt is sunny and beautiful.");

  rooms_[KITCHEN]->add_link(BATHROOM);
  rooms_[KITCHEN]->add_link(LIVING_ROOM);
  rooms_[BATHROOM]->add_link(KITCHEN);
  rooms_[BATHROOM]->add_link(LIVING_ROOM);
  rooms_[LIVING_ROOM]->add_link(BATHROOM);
  rooms_[LIVING_ROOM]->add_link(KITCHEN);
  rooms_[LIVING_ROOM]->add_link(BACK_DECK);
  rooms_[BACK_DECK]->add_link(LIVING_ROOM);
}

Game::~Game() {}

void Game::start() {
  std::cout << "Game start!" << std::endl;

  move_player_to(KITCHEN);

  bool done = false;
  char key;
  while (!done) {
    print_options();

    std::cout << std::endl << ">> ";
    std::cin >> key;

    if (key == 'q' || key == 'Q') {
      done = true;
      std::cout << "Quitting!" << std::endl;

    } else if (key >= '0' && key <= '9') {
      const size_t option_index = (key - '0');

      if (option_index >= 1 && option_index <= options_.size()) {
        auto cmd = options_[option_index - 1];
        cmd->execute(*this);
      } else {
        std::cout << "Unrecognized option: " << option_index << std::endl;
      }
    } else {
      std::cout << "Unrecognized input: '" << key << "'" << std::endl;
    }
  }
}

std::shared_ptr<Room> Game::get_room(const RoomId room_id) {
  return rooms_[room_id];
}

Player* Game::player() {
  return &player_;
}

Room* Game::current_room() {
  return current_room_.get();
}

void Game::move_player_to(const RoomId room_id) {
  // std::cout << "move_player_to(" << room_id << ")" << std::endl;
  auto room = rooms_[room_id];
  current_room_ = room;
  player_.move_to(room_id);
  rebuild_options();
  print_room_desc();
}

void Game::rebuild_options() {
  options_.clear();

  std::for_each(current_room_->links.begin(), current_room_->links.end(), [this](const RoomId link) {
    this->options_.push_back( std::make_shared<MoveToRoom>(link) );
  });

  for (size_t i = 0; i < current_room_->inventory.size(); i++) {
    auto item = current_room_->inventory[i];
    options_.push_back( std::make_shared<PickUpItem>(current_room_, item) );
  }

  for (size_t i = 0; i < player_.inventory.size(); i++) {
    auto item = player_.inventory[i];
    if (item->usable()) {
      options_.push_back( std::make_shared<UseItem>(item) );
    }
    options_.push_back( std::make_shared<DropItem>(current_room_, item) );
  }
}

void Game::print_room_desc() {
  std::cout << "You are in the " << current_room_->name() << "." << std::endl
    << current_room_->desc() << std::endl << std::endl;
}

void Game::print_options() {
  std::cout << std::endl;

  if (current_room_->inventory.has_items()) {
    std::cout << "Objects:" << std::endl << std::endl;
    for (size_t i = 0; i < current_room_->inventory.size(); i++) {
      auto item = current_room_->inventory[i];
      std::cout << "  - " << item->name() << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << "Options:" << std::endl << std::endl;

  int i = 1;
  std::for_each(options_.begin(), options_.end(), [&i,this](std::shared_ptr<Command> command) {
    std::cout << "  (" << i << "): " << command->desc(*this) << std::endl;
    i++;
  });

  std::cout << "  (q): Quit Game" << std::endl;
}


// std::vector<Command> Room::get_options() const {
//   auto o = get_link_options();
//   return o;
// }

// std::vector<Command> Room::get_link_options() const {
//   std::vector<Command> vec( links.size() );
//   std::for_each(links.begin(), links.end(), [&vec](const uint32_t link) {
//     vec.push_back( MoveToRoom(link) );
//   });
//   return vec;
// }
