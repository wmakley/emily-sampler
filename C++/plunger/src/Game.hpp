#ifndef GAME_HPP
#define GAME_HPP

#include <memory>
#include <unordered_map>

#include "IGame.hpp"
#include "RoomId.hpp"
#include "Player.hpp"

class Command;
class Room;

class Game : public IGame
{
private:
  std::unordered_map<RoomId, std::shared_ptr<Room>> rooms_;
  std::unordered_map<IGame::Flag, bool> flags_;
  std::unordered_map<IGame::TempFlag, bool> temp_flags_;
  Player player_;

public:
  Game();
  ~Game();
  std::shared_ptr<Room> get_room(const RoomId id) override;
  std::shared_ptr<const Room> get_room(const RoomId id) const override;

  std::shared_ptr<Room> current_room() override;
  std::shared_ptr<const Room> current_room() const override;

  Player &player() override;
  const Player &player() const override;

  bool flag(IGame::Flag) const override;
  void set_flag(IGame::Flag, bool) override;

  bool temp_flag(IGame::TempFlag) const override;
  void set_temp_flag(IGame::TempFlag) override;
  void reset_temp_flags() override;
};

#endif
