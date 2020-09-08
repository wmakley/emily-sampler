use crate::item::Item;
use crate::item::ItemFactory;
use crate::player::Player;
use crate::room;
use crate::room::Room;
use std::collections::HashMap;

pub struct GameState {
    pub player: Player,
    items: HashMap<u32, Item>,
    rooms: HashMap<u32, Room>,
    item_factory: ItemFactory,
}

impl GameState {
    pub fn new(player: Player) -> GameState {
        let mut game_state = GameState {
            player: player,
            items: HashMap::new(),
            rooms: HashMap::new(),
            item_factory: ItemFactory::new(),
        };

        let mut kitchen = Room::new(
            room::KITCHEN,
            String::from("Kitchen"),
            String::from("It is clean and spotless."),
        );
        kitchen.link_to(room::MUD_ROOM);

        let mut mud_room = Room::new(
            room::MUD_ROOM,
            String::from("Mud Room"),
            String::from("Is is slightly dirty."),
        );
        mud_room.link_to(room::KITCHEN);
        let shoes = game_state.item_factory.build_item(String::from("Shoes"));
        mud_room.inventory.push(shoes.id);
        game_state.items.insert(shoes.id, shoes);

        game_state.rooms.insert(room::KITCHEN, kitchen);
        game_state.rooms.insert(room::MUD_ROOM, mud_room);

        return game_state;
    }

    // pub fn get_item(&self, id: &u32) -> &Item {
    //     return self.items.get(id).unwrap();
    // }

    pub fn get_room(&self, id: &u32) -> &Room {
        return self.rooms.get(id).unwrap();
    }

    pub fn current_room(&self) -> &Room {
        return &self.get_room(&self.player.current_room_id);
    }
}
