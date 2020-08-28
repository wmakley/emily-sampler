# Wants:

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

Player ->
  Game State |
    Room ->
      Game State
    Item ->
      Game State
