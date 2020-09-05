#ifndef UI_HPP
#define UI_HPP

#include <memory>
#include <vector>
#include "IGame.hpp"

class Command;
class Room;

class UI
{
private:
  std::vector<std::shared_ptr<Command>> options_;

public:
  void start_game(IGame &game) const;
  void rebuild_options(IGame &game);
  void print_options(IGame &game);
  void execute_option(const size_t index, IGame &game);
};

#endif
