#include "Inventory.hpp"

Inventory::Inventory() {}
Inventory::~Inventory() {}

void Inventory::add( std::shared_ptr<Item> item ) {
  // std::cout << "add item by shared_ptr: " << item << std::endl;
  items_.push_back(item);
}

bool Inventory::remove( std::shared_ptr<Item> item_to_remove ) {
  auto it = std::find(items_.begin(), items_.end(), item_to_remove);

  if (it != items_.end()) {
    items_.erase(it);
    return true;
  }

  return false;
}

// size_t index_of_item(const std::string& name) {
//   size_t i = 0;
//   auto result = std::find_if(items_.begin(), items_.end(), [&name,&i](auto item) {
//     return (item->name() == name);
//   });
//   return -1;
// }

bool Inventory::has_items() const {
  return !empty();
}

bool Inventory::empty() const {
  return items_.size() == 0;
}

size_t Inventory::size() const {
  return items_.size();
}

std::shared_ptr<Item> Inventory::operator[](size_t index) const {
  return items_[index];
}
