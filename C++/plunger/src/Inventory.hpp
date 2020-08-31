#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <memory>
#include <vector>

class Item;

class Inventory
{
  typedef std::vector<std::shared_ptr<Item>> ItemVec;
  typedef ItemVec::iterator iterator;

private:
  ItemVec items_;

public:
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

private:
  bool remove(const iterator &iter);
};

#endif
