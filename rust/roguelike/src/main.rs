mod action;
mod game;
mod item;
mod map;
mod player;
mod ui;

use player::Player;
use std::io::{self};
use termion::raw::IntoRawMode;
use ui::UI;

fn main() {
    let stdout = io::stdout();
    let stdout = stdout.lock();
    let stdin = io::stdin();
    let stdin = stdin.lock();
    // let stderr = io::stderr();
    // let stderr = stderr.lock();
    let stdout = stdout.into_raw_mode().unwrap();
    let mut ui = UI::new(stdout, stdin);
    let player = Player::new(String::from("Emily"));
    let mut game = game::GameState::new(player);
    ui.render_map(&game);
    ui.start_event_loop(&mut game);
}
