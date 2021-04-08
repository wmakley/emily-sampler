use crate::action::Action;
use crate::game::GameState;
use crate::map::{Point, TileColor};

/// an entity is something that can be on the map
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
