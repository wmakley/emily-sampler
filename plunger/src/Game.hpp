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

class Game : public IGame
{
private:
  std::unordered_map<RoomId, std::shared_ptr<Room>> rooms_;
  std::shared_ptr<Room> current_room_;
  Player player_;
  std::vector<std::shared_ptr<Command>> options_;

public:
  Game();
  ~Game();
  void start() override;
  Room *get_room(const RoomId id) override;
  Player *player() override;
  Room *current_room() override;
  void move_player_to(const RoomId room_id) override;
  void print_room_desc() override;
  void print_options() override;
  void rebuild_options() override;
};

#endif
