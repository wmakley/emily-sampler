pub const KITCHEN: u32 = 1;
pub const MUD_ROOM: u32 = 2;

pub struct Room {
    id: u32,
    name: String,
    desc: String,
    pub inventory: Vec<u32>,
    pub links: Vec<u32>,
}

impl Room {
    pub fn new(id: u32, name: String, desc: String) -> Room {
        return Room {
            id: id,
            name: name,
            desc: desc,
            inventory: Vec::new(),
            links: Vec::new(),
        };
    }

    pub fn id(&self) -> u32 {
        return self.id;
    }
    pub fn name(&self) -> &str {
        return &self.name;
    }

    pub fn desc(&self) -> &str {
        return &self.desc;
    }

    pub fn link_to(&mut self, room_id: u32) {
        &self.links.push(room_id);
    }
}
