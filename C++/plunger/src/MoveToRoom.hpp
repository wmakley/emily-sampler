#ifndef MOVE_TO_ROOM_HPP
#define MOVE_TO_ROOM_HPP

#include <string>
#include "RoomId.hpp"
#include "Command.hpp"

class MoveToRoom: public Command {
private:
  const RoomId room_id_;
public:
  MoveToRoom(uint32_t room_id);
  ~MoveToRoom();

  void execute(IGame& game);
  const std::string desc(IGame& game);
};

#endif
