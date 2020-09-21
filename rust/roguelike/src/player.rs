use crate::action::{Action, EndPlayerTurn, Move};
use crate::game::{Entity, GameState};
use crate::map::{Point, TileColor};
use std::fmt;

// Player is always entity 0
pub const ID: usize = 0;

pub struct Player {
    id: usize,
    pub name: String,
    pub inventory: Vec<u32>,
    position: Point,
}

impl Player {
    pub fn new(name: String) -> Player {
        return Player {
            id: ID,
            name: name.trim().to_string(),
            inventory: Vec::new(),
            position: Point { x: 5, y: 5 },
        };
    }
}

impl Entity for Player {
    fn id(&self) -> usize {
        return ID;
    }

    fn set_id(&mut self, id: usize) {
        self.id = id;
    }

    fn position(&self) -> &Point {
        return &self.position;
    }

    fn set_position(&mut self, new_position: Point) {
        self.position = new_position;
    }

    fn char(&self) -> char {
        return '@';
    }

    fn fg_color(&self) -> TileColor {
        return TileColor::White;
    }

    fn update(&self, _game: &GameState) -> Vec<Box<dyn Action>> {
        return Vec::with_capacity(0);
    }
}

#[derive(Debug)]
pub struct PlayerMove {
    entity_move: Move,
}

impl PlayerMove {
    pub fn up() -> PlayerMove {
        return PlayerMove {
            entity_move: Move::up(ID),
        };
    }

    pub fn down() -> PlayerMove {
        return PlayerMove {
            entity_move: Move::down(ID),
        };
    }

    pub fn left() -> PlayerMove {
        return PlayerMove {
            entity_move: Move::left(ID),
        };
    }

    pub fn right() -> PlayerMove {
        return PlayerMove {
            entity_move: Move::right(ID),
        };
    }
}

impl Action for PlayerMove {
    fn execute(&self, game: &mut GameState) {
        self.entity_move.execute(game);
        EndPlayerTurn.execute(game);
    }
}

impl fmt::Display for PlayerMove {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return self.entity_move.fmt(f);
    }
}
