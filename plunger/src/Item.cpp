#include "Item.hpp"

uint32_t Item::NEXT_ID = 1;

Item::Item(const std::string& name): name_(name), id(Item::NEXT_ID) {
  NEXT_ID++;
}

Item::Item(const Item& other)
  : name_(other.name_), id(other.id)
{
  // std::cout << *this << ": copy constructor" << std::endl;
}

Item::Item(Item&& other)
  : name_(std::move(other.name_)), id(std::move(other.id))
{
  // std::cout << *this << ": move constructor" << std::endl;
}

Item::~Item() {
  // std::cout << *this << ": destructor" << std::endl;
}

const std::string Item::name() const {
  return name_;
}

bool Item::usable() const {
  return false;
}

void Item::use() {
}

void Item::picked_up() {
  std::cout << "You now have a " << name() << "." << std::endl;
}

void Item::dropped() {
  std::cout << "You no longer have a " << name() << "." << std::endl;
}


Plunger::Plunger(): Item("Plunger") {
  // std::cout << *this << std::endl;
}

Plunger::~Plunger() {
  // std::cout << *this << " destructor" << std::endl;
}

bool Plunger::usable() const {
  // std::cout << "plunger useable called" << std::endl;
  return true;
}

PlungerWithRat::PlungerWithRat(): Item("Plunger with rat") {}

bool PlungerWithRat::usable() const {
  return true;
}


std::ostream& operator<<(std::ostream& os, const Item& item)
{
  os << "Item(" << item.id << ", \"" << item.name_ << "\")";
  return os;
}
