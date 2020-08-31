#include "Command.hpp"
#include <iostream>

Command::Command() {}
Command::~Command() {}

std::ostream &operator<<(std::ostream &os, const Command &)
{
  os << "Command()";
  return os;
}
