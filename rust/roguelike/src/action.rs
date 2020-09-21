use crate::game::GameState;
use std::fmt;

pub trait Action: fmt::Display {
    fn execute(&self, game: &mut GameState);
}

#[derive(Debug, PartialEq, Eq)]
pub enum Direction {
    Up,
    Down,
    Left,
    Right,
}

impl fmt::Display for Direction {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let msg = match self {
            Direction::Up => "Up",
            Direction::Down => "Down",
            Direction::Left => "Left",
            Direction::Right => "Right",
        };
        return write!(f, "{}", msg);
    }
}

#[derive(Debug, PartialEq, Eq)]
pub struct Move {
    pub dir: Direction,
    pub entity_id: usize,
}

#[derive(Debug, PartialEq, Eq)]
pub struct EndPlayerTurn;

impl fmt::Display for Move {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return write!(f, "Move {}", self.dir);
    }
}

impl Move {
    pub fn up(entity_id: usize) -> Move {
        return Move {
            dir: Direction::Up,
            entity_id: entity_id,
        };
    }

    pub fn down(entity_id: usize) -> Move {
        return Move {
            dir: Direction::Down,
            entity_id: entity_id,
        };
    }

    pub fn left(entity_id: usize) -> Move {
        return Move {
            dir: Direction::Left,
            entity_id: entity_id,
        };
    }

    pub fn right(entity_id: usize) -> Move {
        return Move {
            dir: Direction::Right,
            entity_id: entity_id,
        };
    }
}

impl Action for Move {
    fn execute(&self, game: &mut GameState) {
        let map_width = game.map.width;
        let map_height = game.map.height;

        let entity = game.get_entity(self.entity_id).unwrap();

        let old_position = entity.position().clone();
        let mut new_position = old_position;

        match self.dir {
            Direction::Up => {
                if old_position.y == 0 {
                    return;
                }
                new_position.y -= 1;
            }
            Direction::Down => {
                if old_position.y == map_height - 1 {
                    return;
                }
                new_position.y += 1;
            }
            Direction::Left => {
                if old_position.x == 0 {
                    return;
                }
                new_position.x -= 1;
            }
            Direction::Right => {
                if old_position.x == map_width - 1 {
                    return;
                }
                new_position.x += 1;
            }
        }
        if new_position == old_position {
            return;
        }

        // check if tile impassable
        let destination_tile = game.map.tile_at(&new_position).unwrap();
        if !destination_tile.get_type().passable {
            return;
        }
        // check if entity in the way
        match game.entity_at(&new_position) {
            Some(_) => return,
            _ => {}
        }

        game.move_entity(self.entity_id, &old_position, new_position);
    }
}

impl Action for EndPlayerTurn {
    fn execute(&self, game: &mut GameState) {
        game.update();
    }
}

impl fmt::Display for EndPlayerTurn {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return write!(f, "End Player Turn");
    }
}
