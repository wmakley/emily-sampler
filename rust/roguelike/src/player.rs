use crate::game::Entity;
use crate::map::{Point, TileColor};

// Player is always entity 0
pub const ID: usize = 0;

pub struct Player {
    id: usize,
    pub name: String,
    pub inventory: Vec<u32>,
    pub position: Point,
}

impl Player {
    pub fn new(name: String) -> Player {
        return Player {
            id: ID,
            name: name.trim().to_string(),
            inventory: Vec::new(),
            position: Point { x: 1, y: 1 },
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
}
