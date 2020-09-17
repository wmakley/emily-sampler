use std::fmt;

pub struct Item {
    pub id: u32,
    pub name: String,
}

impl fmt::Display for Item {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return write!(f, "{}", self.name);
    }
}

pub struct ItemFactory {
    next_id: u32,
}

impl ItemFactory {
    pub fn new() -> ItemFactory {
        return ItemFactory { next_id: 1 };
    }

    pub fn build_item(&mut self, name: String) -> Item {
        let item = Item {
            id: self.next_id,
            name: name,
        };
        self.next_id += 1;
        return item;
    }
}
