#include "Item.hpp"
#include "Game.hpp"
#include "Room.hpp"

uint32_t Item::NEXT_ID = 1;

Item::Item(const std::string &name)
    : name_(name),
      id(Item::NEXT_ID)

{
  NEXT_ID++;
}

Item::Item(const Item &other)
    : name_(other.name_),
      id(other.id)
{
  // std::cout << *this << ": copy constructor" << std::endl;
}

Item::Item(Item &&other)
    : name_(std::move(other.name_)),
      id(std::move(other.id))
{
  // std::cout << *this << ": move constructor" << std::endl;
}

Item::~Item()
{
  // std::cout << *this << ": destructor" << std::endl;
}

const std::string Item::name() const
{
  return name_;
}

bool Item::usable(const IGame &) const
{
  return false;
}

void Item::use(IGame &)
{
}

std::ostream &operator<<(std::ostream &os, const Item &item)
{
  os << "Item(" << item.id << ", \"" << item.name_ << "\")";
  return os;
}

Plunger::Plunger()
    : Item("Plunger")
{
}

bool Plunger::usable(const IGame &game) const
{
  std::shared_ptr<const Room> room = game.current_room();
  bool result = false;
  switch (room->id())
  {
  case BATHROOM:
    result = true;
    break;
  case KITCHEN:
    result = !game.flag(IGame::RAT_REMOVED);
    break;
  }

  return result;
}

void Plunger::use(IGame &game)
{
  std::shared_ptr<Room> room = game.current_room();
  if (room->id() == KITCHEN)
  {
    if (game.flag(IGame::RAT_REMOVED))
    {
      throw "rat already removed";
    }

    game.set_flag(IGame::RAT_REMOVED, true);

    Player &player = game.player();
    player.inventory.remove(this);
    player.inventory.add(std::make_shared<PlungerWithRat>());
    std::cout << "You suction the rat in your plunger." << std::endl
              << "You are carrying a plunger with a rat in it." << std::endl;

    game.set_temp_flag(IGame::PLAYER_INVENTORY_CHANGED);
  }
  else if (room->id() == BATHROOM)
  {
    std::cout << "You plunge the toilet, even though it doesn't need it." << std::endl;
  }
}

PlungerWithRat::PlungerWithRat()
    : Item("Plunger with rat")
{
}

bool PlungerWithRat::usable(const IGame &game) const
{
  return game.current_room()->id() == BACK_DECK;
}

void PlungerWithRat::use(IGame &game)
{
  std::shared_ptr<Room> room = game.current_room();
  if (room->id() == BACK_DECK)
  {
    Player &player = game.player();
    player.inventory.remove(this);
    std::shared_ptr<Item> sun_bathing_rat = std::make_shared<Item>("Sun-bathing rat");
    room->inventory.add(sun_bathing_rat);
    std::cout << "You release the rat. It is now sun-bathing on the back deck." << std::endl
              << "You no longer have a " << this->name() << "." << std::endl;

    std::shared_ptr<Plunger> plunger = std::make_shared<Plunger>();
    player.inventory.add(plunger);
    std::cout << "You now have a " << plunger->name() << "." << std::endl;

    game.set_temp_flag(IGame::PLAYER_INVENTORY_CHANGED);
    game.set_temp_flag(IGame::ROOM_INVENTORY_CHANGED);
  }
  else
  {
    throw "cannot use plunger in this room";
  }
}
