# Plunger

This is a simple game about a rat and a plunger. It demonstrates one possible
way to implement this kind of game in C++ using the command pattern and lots of
runtime polymorphism for item and room behavior.

## Design Notes

* Rooms
  * Should be stateful.
  * Have features that can be interacted with.
  * Items that can be picked up or dropped.
* Items
  * Can be picked up and dropped anywhere.
* Game state:
  * Rooms and their states.
  * Player and her state.

-> = Changes
| = delegates

Keyboard ->
  Game State |
    Item ->
      Game State
    Room ->
      Own State
