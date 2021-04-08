mod action;
mod entity;
mod game;
mod item;
mod map;
mod perlin;
mod player;
mod ui;

use log::LevelFilter;
use player::Player;
use std::io::{self};
use termion::raw::IntoRawMode;
use ui::UI;

fn main() {
    let stdout = io::stdout();
    let stdout = stdout.lock();
    let stdout = stdout.into_raw_mode().unwrap();

    let stdin = io::stdin();
    let stdin = stdin.lock();

    let stderr = io::stderr();
    let stderr = stderr.lock();

    simple_logging::log_to_file("debug.log", LevelFilter::Debug).unwrap();

    let mut ui = UI::new(stdout, stdin);
    let player = Player::new(String::from("Emily"));
    let mut game = game::GameState::new(player);

    ui.render(&game);
    ui.start_event_loop(&mut game);
}
