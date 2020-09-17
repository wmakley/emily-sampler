use crate::map::{Map, Point, TileColor};
use crate::player::{self, Player};
use std::collections::HashMap;

pub struct GameState {
    pub map: Map,
    pub changed: bool,
    entities: Vec<Box<dyn Entity>>,
    entity_positions: HashMap<usize, usize>,
}

impl GameState {
    pub fn new(player: Player) -> GameState {
        let mut game = GameState {
            map: Map::new(500, 500),
            changed: false,
            entities: Vec::new(),
            entity_positions: HashMap::new(),
        };
        game.add_entity(Box::new(player));
        return game;
    }

    pub fn add_entity(&mut self, mut entity: Box<dyn Entity>) {
        let id = self.entities.len();
        entity.set_id(id);

        let key = self.map.index_from_point(entity.position()).unwrap();

        if self.entity_positions.contains_key(&key) {
            panic!("There is already an entity at that position!");
        }

        self.entity_positions.insert(key, id);
        self.entities.push(entity);
    }

    pub fn move_entity(&mut self, id: usize, old_position: &Point, new_position: Point) {
        if *old_position == new_position {
            return;
        }
        let entity = self.get_mut_entity(id).unwrap();
        entity.set_position(new_position);
        let old_key = self.map.index_from_point(old_position).unwrap();
        let new_key = self.map.index_from_point(&new_position).unwrap();
        self.entity_positions.remove(&old_key);
        self.entity_positions.insert(new_key, id);
        self.changed = true;
    }

    pub fn get_entity(&self, id: usize) -> Option<&Box<dyn Entity>> {
        return self.entities.get(id);
    }

    pub fn get_mut_entity(&mut self, id: usize) -> Option<&mut Box<dyn Entity>> {
        return self.entities.get_mut(id);
    }

    pub fn entity_at(&self, point: &Point) -> Option<&dyn Entity> {
        let key = self.map.index_from_point(point).unwrap();
        return self
            .entity_positions
            .get(&key)
            .map(|id| &*self.entities[*id]);
    }
}

// an entity is something that can be on the map
pub trait Entity {
    fn id(&self) -> usize;
    fn set_id(&mut self, id: usize);
    fn position(&self) -> &Point;
    fn set_position(&mut self, new_position: Point);
    fn char(&self) -> char;
    fn fg_color(&self) -> TileColor;
}
