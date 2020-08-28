#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "IGame.hpp"

class Command {
public:
  virtual void execute(IGame& game) = 0;
  virtual const std::string desc(IGame& game) = 0;
  Command();
  virtual ~Command();
};

#endif
