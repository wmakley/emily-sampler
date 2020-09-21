use crate::action::Action;
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
            map: Map::new(300, 300),
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

    // Run game world one tick.
    pub fn update(&mut self) {
        let mut actions: Vec<Box<dyn Action>> = Vec::new();
        for entity in self.entities.iter() {
            let mut entity_actions = entity.update(&self);
            actions.append(&mut entity_actions);
        }
        for action in actions.iter() {
            action.execute(self);
        }
    }

    // Assume that ID is correct and new_position is valid.
    pub fn move_entity(&mut self, id: usize, old_position: &Point, new_position: Point) {
        if *old_position == new_position {
            return;
        }
        let entity = self.get_mut_entity(id).unwrap();
        entity.set_position(new_position);
        let old_key = self.map.index_from_point(old_position).unwrap();
        let new_key = self.map.index_from_point(&new_position).unwrap();

        assert!(self.entity_positions.contains_key(&old_key));
        assert!(!self.entity_positions.contains_key(&new_key));

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
        let key = self.map.index_from_point(point)?;
        return self
            .entity_positions
            .get(&key)
            .map(|id| &*self.entities[*id]);
    }

    pub fn player(&self) -> &Box<dyn Entity> {
        return self.get_entity(player::ID).unwrap();
    }

    // pub fn player_mut(&mut self) -> &mut Box<dyn Entity> {
    //     return self.get_mut_entity(player::ID).unwrap();
    // }
}

// an entity is something that can be on the map
pub trait Entity {
    fn id(&self) -> usize;
    /// A value owned by the GameState.
    fn set_id(&mut self, id: usize);
    fn position(&self) -> &Point;
    /// A value owned by the GameState. TODO: find if there is a cleaner way
    /// to tag entities with values that only the GameState can change?
    fn set_position(&mut self, new_position: Point);
    fn char(&self) -> char;
    fn fg_color(&self) -> TileColor;
    fn update(&self, game: &GameState) -> Vec<Box<dyn Action>>;
}
