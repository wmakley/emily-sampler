#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Inventory.hpp"
#include "RoomId.hpp"
#include "IGame.hpp"

class Room
{
private:
  const RoomId id_;
  const std::string name_;
  const std::string desc_;

public:
  Inventory inventory;
  std::vector<RoomId> links;

  Room(const RoomId id, const std::string &name, const std::string &desc);
  Room(const Room &other);

  virtual ~Room();

  RoomId id() const;
  const std::string name() const;
  virtual const std::string desc(const IGame &) const;

  void add_link(RoomId room_id);

  friend std::ostream &operator<<(std::ostream &os, const Room &room);
};

std::ostream &operator<<(std::ostream &os, const Room &room);

class Kitchen : public Room
{
public:
  Kitchen();
  const std::string desc(const IGame &) const override;
};

#endif
