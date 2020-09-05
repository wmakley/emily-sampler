#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <unordered_map>
#include <vector>

#include "IGame.hpp"
#include "RoomId.hpp"
#include "Player.hpp"

class Command;
class Room;

class Game : public IGame
{
private:
  std::unordered_map<RoomId, std::shared_ptr<Room>> rooms_;
  Player player_;
  bool rat_removed_;

public:
  Game();
  ~Game();
  std::shared_ptr<Room> get_room(const RoomId id) override;
  std::shared_ptr<const Room> get_room(const RoomId id) const override;

  std::shared_ptr<Room> current_room() override;
  std::shared_ptr<const Room> current_room() const override;

  Player &player() override;
  const Player &player() const override;

  bool rat_removed() const override;
  void remove_rat() override;
};

#endif
