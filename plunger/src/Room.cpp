#include "Room.hpp"

#include <iostream>

Room::Room(const RoomId id, const std::string &name, const std::string &desc)
    : id_(id), name_(name), desc_(desc)
{
  // std::cout << *this << " constructor" << std::endl;
}

Room::Room(const Room &other)
    : id_(other.id_), name_(other.name_), desc_(other.desc_)
{
}

Room::~Room()
{
  // std::cout << *this << " destructor" << std::endl;
}

RoomId Room::id() const
{
  return id_;
}

const std::string Room::name() const
{
  return name_;
}

const std::string Room::desc() const
{
  return desc_;
}

void Room::add_link(RoomId room_id)
{
  links.push_back(room_id);
}

std::ostream &operator<<(std::ostream &os, const Room &room)
{
  os << "Room(\"" << room.name() << "\")";
  return os;
}

Kitchen::Kitchen()
    : Room(KITCHEN, "Kitchen", "It is very grungy, and there are crumbs all over the stove."),
      rat_killed_(false)
{
}

Kitchen::Kitchen(bool rat_killed) : Kitchen()
{
  rat_killed_ = rat_killed;
}

Kitchen::Kitchen(const Kitchen &other) : Kitchen()
{
  rat_killed_ = other.rat_killed_;
}

void Kitchen::kill_rat()
{
  if (rat_killed_)
  {
    return;
  }

  rat_killed_ = true;
}

bool Kitchen::rat_killed() const
{
  return rat_killed_;
}

const std::string Kitchen::desc() const
{
  if (rat_killed_)
  {
    return Room::desc();
  }
  else
  {
    return Room::desc() + "\nThere is a large rat eating the crumbs.";
  }
}
