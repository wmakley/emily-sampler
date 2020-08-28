#include "Room.hpp"

#include <iostream>

Room::Room(const std::string &name, const std::string &desc):
  name_(name), desc_(desc)
{
  // std::cout << *this << " constructor" << std::endl;
}

Room::Room(const Room& other):
  name_(other.name_), desc_(other.desc_)
{
}

Room::~Room() {
  // std::cout << *this << " destructor" << std::endl;
}

const std::string Room::name() const {
  return name_;
}

const std::string Room::desc() const {
  return desc_;
}

void Room::add_link(uint32_t room_id) {
  links.push_back(room_id);
}

std::ostream& operator<<(std::ostream& os, const Room& room)
{
  os << "Room(\"" << room.name() << "\")";
  return os;
}


Kitchen::Kitchen():
  Room("Kitchen", "It is very grungy, and there are crumbs all over the stove."),
  rat_killed_(false)
{}

Kitchen::Kitchen(bool rat_killed):
  Kitchen()
{
  rat_killed_ = rat_killed;
}

Kitchen::Kitchen(const Kitchen& other):
  Kitchen()
{
  rat_killed_ = other.rat_killed_;
}

void Kitchen::kill_rat() {
  if (rat_killed_) {
    return;
  }

  rat_killed_ = true;
}

const std::string Kitchen::desc() const {
  if (rat_killed_) {
    return Room::desc();
  } else {
    return Room::desc() + "\nThere is a large rat eating the crumbs.";
  }
}
