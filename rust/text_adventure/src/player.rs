pub struct Player {
    pub name: String,
    pub inventory: Vec<u32>,
    pub current_room_id: u32,
}

impl Player {
    pub fn new(name: String) -> Player {
        return Player {
            name: name.trim().to_string(),
            inventory: Vec::new(),
            current_room_id: 0,
        };
    }
}
