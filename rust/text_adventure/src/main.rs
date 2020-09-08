mod action;
mod game;
mod item;
mod player;
mod room;
mod ui;

use action::Action;
use player::Player;

fn main() {
    let mut ui = ui::UI::new();
    ui.print_intro();
    let name = ui.ask_name();
    let player = Player::new(name);
    let mut game = game::GameState::new(player);
    ui.greet_player(&game.player);

    action::WalkToRoom::from_room(game.get_room(&room::KITCHEN)).execute(&mut game);

    let mut done = false;
    let mut input: String;
    while !done {
        ui.build_options(&game);
        ui.describe_room(&game);
        ui.print_options();

        input = ui.get_input();

        if input.as_bytes()[0] == b'q' {
            done = true;
        } else {
            let number: usize = input.parse().unwrap();
            ui.execute_action(number - 1, &mut game);
        }
    }
}
