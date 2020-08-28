#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Inventory.hpp"
#include "RoomId.hpp"

class Room {
private:
  const std::string name_;
  const std::string desc_;

public:
  Inventory inventory;
  std::vector<RoomId> links;

  Room(const std::string& name, const std::string& desc);
  Room(const Room& other);

  virtual ~Room();

  virtual const std::string name() const;
  virtual const std::string desc() const;

  void add_link(RoomId room_id);

  friend std::ostream& operator<<(std::ostream& os, const Room& room);
};

std::ostream& operator<<(std::ostream& os, const Room& room);

class Kitchen: public Room {
private:
  bool rat_killed_;

public:
  Kitchen();
  Kitchen(bool rat_killed);
  Kitchen(const Kitchen& other);
  const std::string desc() const override;
  void kill_rat();
};

#endif
