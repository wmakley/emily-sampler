#ifndef ITEM_HPP
#define ITEM_HPP

#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include "IGame.hpp"
#include "ItemId.hpp"

class Item
{
private:
  const std::string name_;

public:
  const ItemId id;
  static ItemId NEXT_ID;

  Item(const std::string &name);
  Item(const Item &other);
  Item(Item &&other);
  virtual ~Item();

  virtual const std::string name() const;

  virtual bool usable(const IGame &game) const;
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
  bool usable(const IGame &game) const override;
  void use(IGame &game) override;
};

class PlungerWithRat : public Item
{
public:
  PlungerWithRat();
  bool usable(const IGame &game) const override;
  void use(IGame &game) override;
};

#endif
