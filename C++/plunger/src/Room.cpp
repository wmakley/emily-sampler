#include "Room.hpp"

Room::Room(const RoomId id, const std::string &name, const std::string &desc)
    : id_(id), name_(name), desc_(desc)
{
  // std::cout << *this << " constructor" << std::endl;
}

Room::Room(const RoomId id, const std::string &name, const std::string &desc,
           std::initializer_list<RoomId> _links)
    : id_(id), name_(name), desc_(desc),
      links(_links)
{
}

Room::Room(const RoomId id, const std::string &name, const std::string &desc,
           std::initializer_list<RoomId> _links,
           std::initializer_list<std::shared_ptr<Item>> items)
    : id_(id), name_(name), desc_(desc),
      inventory(items),
      links(_links)
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

const std::string Room::desc(const IGame &) const
{
  return desc_;
}

void Room::add_link(RoomId room_id)
{
  links.insert(room_id);
}

std::ostream &operator<<(std::ostream &os, const Room &room)
{
  os << "Room(\"" << room.name() << "\")";
  return os;
}

Kitchen::Kitchen()
    : Room(KITCHEN, "Kitchen", "It is very grungy, and there are crumbs all over the stove.")
{
}

Kitchen::Kitchen(std::initializer_list<RoomId> _links)
    : Room(KITCHEN, "Kitchen", "It is very grungy, and there are crumbs all over the stove.", _links)
{
}

const std::string Kitchen::desc(const IGame &game) const
{
  if (game.flag(IGame::RAT_REMOVED))
  {
    return Room::desc(game);
  }
  else
  {
    return Room::desc(game) + "\nThere is a large rat eating the crumbs.";
  }
}
