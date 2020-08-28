#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <memory>
#include <vector>

#include "Item.hpp"

class Inventory {
private:
  std::vector< std::shared_ptr<Item> > items_;

public:
  Inventory();
  ~Inventory();

  void add( std::shared_ptr<Item> item );
  bool remove( std::shared_ptr<Item> item );

  bool has_items() const;
  bool empty() const;
  size_t size() const;

  std::shared_ptr<Item> operator[](size_t index) const;
};

#endif
