#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "IGame.hpp"
#include "RoomId.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Command.hpp"

class Game: public IGame {
private:
  std::unordered_map<RoomId, std::shared_ptr<Room> > rooms_;
  std::shared_ptr<Room> current_room_;
  Player player_;
  std::vector<std::shared_ptr<Command> > options_;

public:
  Game();
  ~Game();
  void start();
  std::shared_ptr<Room> get_room(const RoomId id);
  Player *player();
  Room *current_room();
  void move_player_to(const RoomId room_id);
  void print_room_desc();
  void print_options();
  void rebuild_options();
};

#endif
