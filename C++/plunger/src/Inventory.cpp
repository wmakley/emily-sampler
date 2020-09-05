#include "Inventory.hpp"
#include "Item.hpp"

Inventory::Inventory() {}
Inventory::~Inventory() {}

Inventory::Inventory(std::initializer_list<std::shared_ptr<Item>> items)
    : items_(items)
{
}

void Inventory::add(std::shared_ptr<Item> item)
{
  // std::cout << "add item by shared_ptr: " << item << std::endl;
  items_.push_back(item);
}

bool Inventory::remove(std::shared_ptr<Item> item_to_remove)
{
  auto it = std::find(std::begin(items_), std::end(items_), item_to_remove);
  return remove(it);
}

bool Inventory::remove(Item *item_to_remove)
{
  auto it = std::find_if(std::begin(items_), std::end(items_), [&item_to_remove](auto item) {
    return item.get() == item_to_remove;
  });

  return remove(it);
}

bool Inventory::remove(const iterator &it)
{
  if (it != items_.end())
  {
    items_.erase(it);
    return true;
  }

  return false;
}

bool Inventory::has_items() const
{
  return !empty();
}

bool Inventory::empty() const
{
  return items_.empty();
}

size_t Inventory::size() const
{
  return items_.size();
}

Inventory::iterator Inventory::begin()
{
  return items_.begin();
}

Inventory::iterator Inventory::end()
{
  return items_.end();
}

Inventory::const_iterator Inventory::begin() const
{
  return items_.begin();
}

Inventory::const_iterator Inventory::end() const
{
  return items_.end();
}

std::shared_ptr<Item> Inventory::operator[](size_t index) const
{
  return items_[index];
}

namespace std
{
  Inventory::iterator begin(Inventory &inventory)
  {
    return inventory.begin();
  }

  Inventory::iterator end(Inventory &inventory)
  {
    return inventory.end();
  }

  Inventory::const_iterator begin(const Inventory &inventory)
  {
    return inventory.begin();
  }

  Inventory::const_iterator end(const Inventory &inventory)
  {
    return inventory.end();
  }
} // namespace std
