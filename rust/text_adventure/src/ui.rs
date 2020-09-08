use crate::action;
use crate::game::GameState;
use crate::player::Player;
use std::io;
use std::io::Write;

pub struct UI {
    actions: Vec<Box<dyn action::Action>>,
}

impl UI {
    pub fn new() -> UI {
        return UI {
            actions: Vec::new(),
        };
    }

    pub fn print_intro(&self) {
        println!("Welcome to my game!");
    }

    pub fn ask_name(&self) -> String {
        print!("Please enter your name: ");
        io::stdout().flush().expect("Could not flush stdout");
        let mut name = String::new();
        io::stdin()
            .read_line(&mut name)
            .expect("Failed to read line");
        println!();
        return name;
    }

    pub fn greet_player(&self, player: &Player) {
        println!("Welcome, {}!\n", player.name);
    }

    pub fn describe_room(&self, game: &GameState) {
        let room = game.current_room();
        println!("You are in the {}.\n{}\n", room.name(), room.desc());
    }

    pub fn build_options(&mut self, game: &GameState) {
        self.actions.clear();
        let room = game.current_room();
        for &room_id in room.links.iter() {
            self.actions
                .push(Box::from(action::WalkToRoom::new(room_id, game)));
        }
    }

    pub fn print_options(&self) {
        println!("What would you like to do?\n");
        for (i, action) in self.actions.iter().enumerate() {
            println!("  ({}) {}", i + 1, action);
        }
        println!("  (q) Quit game.\n");
        print!(">> ");
        io::stdout().flush().expect("Could not flush stdout");
    }

    pub fn get_input(&self) -> String {
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        return input.trim().to_string();
    }

    pub fn execute_action(&self, index: usize, game: &mut GameState) {
        let action = &self.actions.get(index).unwrap();
        action.execute(game);
    }
}
