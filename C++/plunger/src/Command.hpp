#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include "IGame.hpp"

class Command
{
public:
  Command();
  virtual ~Command();

  virtual void execute(IGame &game) = 0;
  virtual const std::string desc(const IGame &game) const = 0;

  friend std::ostream &operator<<(std::ostream &os, const Room &room);
};

std::ostream &operator<<(std::ostream &os, const Room &room);

#endif
