extern crate termion;

use crate::action::*;
use crate::game::GameState;
use crate::map::{Point, Tile, BLANK};
use crate::player;
use std::io::{Read, Write};

use std::convert::TryFrom;
use termion::input::Keys;
use termion::input::TermRead;
use termion::{color, cursor, style};

pub struct UI<R: Read, W: Write> {
    stdin: Keys<R>,
    stdout: W,
    viewport: Viewport,
}

struct Viewport {
    pub left: i32,
    pub top: i32,
    pub width: u16,
    pub height: u16,
}

impl<R: Read, W: Write> UI<R, W> {
    pub fn new(stdout: W, stdin: R) -> UI<R, W> {
        let termsize = termion::terminal_size().ok();
        let termwidth = termsize.map(|(w, _)| w).unwrap_or(70);
        let termheight = termsize.map(|(_, h)| h).unwrap_or(40);
        return UI {
            stdin: stdin.keys(),
            stdout: stdout,
            viewport: Viewport {
                top: 0,
                left: 0,
                width: termwidth,
                height: termheight,
            },
        };
    }

    pub fn start_event_loop(&mut self, game: &mut GameState) {
        let mut done = false;
        while !done {
            let b = self.stdin.next().unwrap().unwrap();
            use termion::event::Key::*;

            match b {
                Char('q') | Char('Q') => done = true,
                Up => Move::up(player::ID).execute(game),
                Right => Move::right(player::ID).execute(game),
                Down => Move::down(player::ID).execute(game),
                Left => Move::left(player::ID).execute(game),
                _ => {}
            }

            if game.changed {
                self.render_map(game);
                game.changed = false;
            }
        }
    }

    pub fn render_map(&mut self, game: &GameState) {
        // Move cursor to upper-left and set initial colors
        write!(
            self.stdout,
            "{}{}{}{}",
            cursor::Hide,
            cursor::Goto(1, 1),
            color::Bg(color::Black),
            color::Fg(color::White)
        )
        .unwrap();

        let viewport = &self.viewport;
        let blank = Tile::of_type(BLANK);
        let mut prev_tile_type = blank.get_type();

        for scr_y in 1..viewport.height + 1 {
            for scr_x in 1..viewport.width + 1 {
                let point = self.map_coord_from_screen_coord(scr_x, scr_y);
                let tile = point.and_then(|p| game.map.tile_at(&p)).unwrap_or(&blank);
                let entity = point.and_then(|p| game.entity_at(&p));

                let tile_type = tile.get_type();
                if tile_type.bg_color != prev_tile_type.bg_color {
                    write!(self.stdout, "{}", color::Bg(tile_type.bg_color)).unwrap();
                }

                match entity {
                    Some(e) => {
                        if e.fg_color() != prev_tile_type.fg_color {
                            write!(self.stdout, "{}", color::Fg(tile_type.fg_color)).unwrap();
                        }
                        write!(self.stdout, "{}", e.char()).unwrap();
                    }

                    None => {
                        if tile_type.fg_color != prev_tile_type.fg_color {
                            write!(self.stdout, "{}", color::Fg(tile_type.fg_color)).unwrap();
                        }
                        write!(self.stdout, "{}", tile.get_type().char).unwrap();
                    }
                }

                prev_tile_type = tile_type;
            }
            write!(self.stdout, "{}", cursor::Goto(1, scr_y + 1)).unwrap();
        }

        self.stdout.flush().unwrap();
    }

    fn map_coord_from_screen_coord(&self, scr_x: u16, scr_y: u16) -> Option<Point> {
        assert!(scr_x <= self.viewport.width);
        assert!(scr_y <= self.viewport.height);

        let x = (i32::from(scr_x)) - 1; // convert to zero-based int
        let x = x + self.viewport.left;
        if x < 0 {
            return None;
        }

        let y = (i32::from(scr_y)) - 1;
        let y = y + self.viewport.top;
        if y < 0 {
            return None;
        }

        // Bounds checks should already have been performed, so this should
        // always succeed.
        let point = Point::try_from_ints(x, y).unwrap();

        return Some(point);
    }

    // Screen coords are 1-based (due to terminal convention)
    #[allow(unused)]
    fn screen_coord_from_map_coord(&self, point: &Point) -> Option<(u16, u16)> {
        // todo
        let x = u16::try_from(point.x + 1).unwrap();
        let y = u16::try_from(point.y + 1).unwrap();
        return Some((x, y));
    }
}

impl<R: Read, W: Write> Drop for UI<R, W> {
    fn drop(&mut self) {
        writeln!(
            self.stdout,
            "{}{}\n\nThanks for playing!!!",
            style::Reset,
            cursor::Show
        )
        .unwrap();
    }
}
