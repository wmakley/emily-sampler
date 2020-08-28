#ifndef ITEM_HPP
#define ITEM_HPP

#include <cstdint>
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

typedef uint32_t ItemId;

class Item {
private:
  const std::string name_;

public:
  const ItemId id;
  static ItemId NEXT_ID;

  Item(const std::string& name);
  Item(const Item& other);
  Item(Item&& other);
  virtual ~Item();

  virtual const std::string name() const;
  virtual bool usable() const;

  virtual void use();
  virtual void picked_up();
  virtual void dropped();

  friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

// Make Items hashable by their globally unique ID
namespace std
{
  template<> struct hash<Item>
  {
    std::size_t operator()(Item const& i) const noexcept
    {
      return i.id;
    }
  };
}


std::ostream& operator<<(std::ostream& os, const Item& item);


class Plunger : public Item {
public:
  Plunger();
  ~Plunger();
  bool usable() const override;
};

class PlungerWithRat : public Item {
public:
  PlungerWithRat();
  bool usable() const override;
};

#endif
