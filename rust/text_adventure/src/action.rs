use crate::game::GameState;
use crate::room::Room;
use std::fmt;

pub trait Action: fmt::Display {
    fn execute(&self, game: &mut GameState);
}

pub struct WalkToRoom {
    desc: String,
    room_id: u32,
}

impl WalkToRoom {
    pub fn new(room_id: u32, game: &GameState) -> WalkToRoom {
        let room = game.get_room(&room_id);
        return WalkToRoom {
            desc: format!("Walk to {}", room.name()),
            room_id: room_id,
        };
    }

    pub fn from_room(room: &Room) -> WalkToRoom {
        return WalkToRoom {
            desc: format!("Walk to {}", room.name()),
            room_id: room.id(),
        };
    }
}

impl fmt::Display for WalkToRoom {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return write!(f, "{}", self.desc);
    }
}

impl Action for WalkToRoom {
    fn execute(&self, game: &mut GameState) {
        game.player.current_room_id = self.room_id;
    }
}
