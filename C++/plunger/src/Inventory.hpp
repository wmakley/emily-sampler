#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <memory>
#include <vector>

class Item;

class Inventory
{
private:
  typedef std::vector<std::shared_ptr<Item>> ItemVec;
  ItemVec items_;

public:
  typedef ItemVec::iterator iterator;
  typedef ItemVec::const_iterator const_iterator;

  Inventory();
  ~Inventory();

  void add(std::shared_ptr<Item> item);
  bool remove(std::shared_ptr<Item> item);
  bool remove(Item *item);

  bool has_items() const;
  bool empty() const;
  size_t size() const;
  std::shared_ptr<Item> operator[](size_t index) const;

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

private:
  bool remove(const iterator &iter);
};

Inventory::iterator begin(Inventory &inventory);
Inventory::iterator end(Inventory &inventory);
Inventory::const_iterator begin(const Inventory &inventory);
Inventory::const_iterator end(const Inventory &inventory);

#endif
