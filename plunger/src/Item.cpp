#include "Item.hpp"
#include "Game.hpp"

uint32_t Item::NEXT_ID = 1;

Item::Item(const std::string &name) : name_(name), id(Item::NEXT_ID)
{
  NEXT_ID++;
}

Item::Item(const Item &other)
    : name_(other.name_), id(other.id)
{
  // std::cout << *this << ": copy constructor" << std::endl;
}

Item::Item(Item &&other)
    : name_(std::move(other.name_)), id(std::move(other.id))
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

bool Item::usable(IGame &) const
{
  return false;
}

void Item::use(IGame &)
{
}

void Item::picked_up(IGame &)
{
  std::cout << "You now have a " << name() << "." << std::endl;
}

void Item::dropped(IGame &)
{
  std::cout << "You no longer have a " << name() << "." << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Item &item)
{
  os << "Item(" << item.id << ", \"" << item.name_ << "\")";
  return os;
}

Plunger::Plunger() : Item("Plunger")
{
  // std::cout << *this << std::endl;
}

Plunger::~Plunger()
{
  // std::cout << *this << " destructor" << std::endl;
}

bool Plunger::usable(IGame &game) const
{
  // std::cout << "plunger useable called" << std::endl;
  Room *room = game.current_room();
  return room->id() == BATHROOM ||
         (room->id() == KITCHEN && !((Kitchen *)room)->rat_killed());
}

void Plunger::use(IGame &game)
{
  Room *room = game.current_room();
  if (room->id() == KITCHEN)
  {
    Kitchen *kitchen = (Kitchen *)room;
    if (kitchen->rat_killed())
    {
      return;
    }

    kitchen->kill_rat();

    Player *player = game.player();
    player->inventory.remove(this);
    player->inventory.add(std::make_shared<PlungerWithRat>());
    std::cout << "You suction the rat in your plunger." << std::endl
              << "You are carrying a plunger with a rat in it." << std::endl;

    game.rebuild_options();
  }
  else if (room->id() == BATHROOM)
  {
    std::cout << "You plunge the toilet, even though it doesn't need it." << std::endl;
  }
}

PlungerWithRat::PlungerWithRat() : Item("Plunger with rat") {}

bool PlungerWithRat::usable(IGame &game) const
{
  return game.current_room()->id() == BACK_DECK;
}

void PlungerWithRat::use(IGame &game)
{
  Room *room = game.current_room();
  if (room->id() == BACK_DECK)
  {
    Player *player = game.player();
    player->inventory.remove(this);
    std::shared_ptr<Item> sun_bathing_rat = std::make_shared<Item>("Sun-bathing rat");
    room->inventory.add(sun_bathing_rat);
    std::cout << "You release the rat. It is now sun-bathing on the back deck." << std::endl;
    sun_bathing_rat->dropped(game);

    std::shared_ptr<Plunger> plunger = std::make_shared<Plunger>();
    player->inventory.add(plunger);
    plunger->picked_up(game);

    game.rebuild_options();
  }
}
