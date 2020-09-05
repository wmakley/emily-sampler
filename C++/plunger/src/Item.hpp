#ifndef ITEM_HPP
#define ITEM_HPP

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include "IGame.hpp"

typedef uint32_t ItemId;

class Item
{
public:
  typedef std::function<bool(const IGame &)> UsablePredicate;

private:
  const std::string name_;
  const UsablePredicate usable_predicate_;

public:
  const ItemId id;
  static ItemId NEXT_ID;

  Item(const std::string &name);
  Item(const std::string &name, const UsablePredicate &pred);
  Item(const Item &other);
  Item(Item &&other);
  virtual ~Item();

  virtual const std::string name() const;

  bool usable(const IGame &game) const;
  virtual void use(IGame &game);

  friend std::ostream &operator<<(std::ostream &os, const Item &item);
};

// Make Items hashable by their globally unique ID
namespace std
{
  template <>
  struct hash<Item>
  {
    std::size_t operator()(Item const &i) const noexcept
    {
      return i.id;
    }
  };
} // namespace std

std::ostream &operator<<(std::ostream &os, const Item &item);

class Plunger : public Item
{
public:
  Plunger();
  void use(IGame &game) override;
};

class PlungerWithRat : public Item
{
public:
  PlungerWithRat();
  void use(IGame &game) override;
};

#endif
