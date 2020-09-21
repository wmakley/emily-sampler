extern crate termion;

use crate::action::*;
use crate::game::GameState;
use crate::map::{Point, Tile, BLANK};
use crate::player::PlayerMove;
use log;
use std::io;

// use std::convert::TryFrom;
use termion::input::Keys;
use termion::input::TermRead;
use termion::{clear, color, cursor, style};

pub struct UI<R: io::Read, W: io::Write> {
    stdin: Keys<R>,
    stdout: W,
    windows: Vec<Box<dyn Window<W>>>,
}

impl<R: io::Read, W: io::Write> UI<R, W> {
    pub fn new(stdout: W, stdin: R) -> UI<R, W> {
        let termsize = termion::terminal_size().ok();
        let termwidth = termsize.map(|(w, _)| w).unwrap_or(70);
        let termheight = termsize.map(|(_, h)| h).unwrap_or(40);

        let map_win = Box::new(MapWindow::new(Rect {
            top: 1,
            left: 1,
            width: termwidth - 2,
            height: termheight - 2,
        }));

        return UI {
            stdin: stdin.keys(),
            stdout: stdout,
            windows: vec![map_win],
        };
    }

    /// Render the entire UI including all sub-windows.
    pub fn render(&mut self, game: &GameState) {
        write!(self.stdout, "{}{}", clear::All, cursor::Hide,).unwrap();

        for win in self.windows.iter() {
            write!(
                self.stdout,
                "{}",
                cursor::Goto(win.rect().left + 1, win.rect().top + 1)
            )
            .unwrap();

            win.render(&mut self.stdout, game);
        }

        // Reset colors back to terminal defaults
        write!(
            self.stdout,
            "{}{}",
            color::Bg(color::Reset),
            color::Fg(color::Reset)
        )
        .unwrap();

        self.stdout.flush().unwrap();
    }

    pub fn start_event_loop(&mut self, game: &mut GameState) {
        let mut done = false;
        while !done {
            let b = self.stdin.next().unwrap().unwrap();
            use termion::event::Key::*;

            log::debug!("Key: {:?}", b);
            match b {
                Char('q') | Char('Q') => done = true,
                Up => PlayerMove::up().execute(game),
                Right => PlayerMove::right().execute(game),
                Down => PlayerMove::down().execute(game),
                Left => PlayerMove::left().execute(game),
                _ => {}
            }

            log::debug!("player position: {:?}", game.player().position());

            if game.changed {
                self.render(game);
                game.changed = false;
            }
        }
    }
}

impl<R: io::Read, W: io::Write> Drop for UI<R, W> {
    fn drop(&mut self) {
        writeln!(
            self.stdout,
            "{}{}\n\n\rThanks for playing!!!",
            style::Reset,
            cursor::Show
        )
        .unwrap();
    }
}

/// Basic dimensions and positioning for all windows.
/// Zero-based, not one-based. Conversion happens only in cursor::Goto,
/// not before.
#[derive(Debug)]
struct Rect {
    width: u16,
    height: u16,
    top: u16,
    left: u16,
}

trait Window<W: io::Write> {
    /// Get the window's Rect.
    fn rect(&self) -> &Rect;

    /// Render the window inside its rect.
    fn render(&self, stdout: &mut W, game: &GameState);
}

struct MapWindow {
    rect: Rect,
}

impl<W: io::Write> Window<W> for MapWindow {
    fn rect(&self) -> &Rect {
        return &self.rect;
    }

    fn render(&self, stdout: &mut W, game: &GameState) {
        let player_position = game.player().position();
        let blank = Tile::of_type(BLANK);
        let mut prev_tile_type = blank.get_type();

        write!(
            stdout,
            "{}{}",
            color::Bg(prev_tile_type.bg_color),
            color::Fg(prev_tile_type.fg_color),
        )
        .unwrap();

        for win_y in 0..self.rect.height {
            for win_x in 0..self.rect.width {
                let point = self.map_point_from_window_point(win_x, win_y, player_position);
                let tile = point.and_then(|p| game.map.tile_at(&p)).unwrap_or(&blank);
                let entity = point.and_then(|p| game.entity_at(&p));

                let tile_type = tile.get_type();
                if tile_type.bg_color != prev_tile_type.bg_color {
                    write!(stdout, "{}", color::Bg(tile_type.bg_color)).unwrap();
                }

                match entity {
                    Some(e) => {
                        if e.fg_color() != prev_tile_type.fg_color {
                            write!(stdout, "{}", color::Fg(tile_type.fg_color)).unwrap();
                        }
                        write!(stdout, "{}", e.char()).unwrap();
                    }

                    None => {
                        if tile_type.fg_color != prev_tile_type.fg_color {
                            write!(stdout, "{}", color::Fg(tile_type.fg_color)).unwrap();
                        }
                        write!(stdout, "{}", tile.get_type().char).unwrap();
                    }
                }

                prev_tile_type = tile_type;
            }

            write!(
                stdout,
                "{}",
                cursor::Goto(self.rect.left + 1, self.rect.top + win_y + 2)
            )
            .unwrap();
        }
    }
}

impl MapWindow {
    pub fn new(rect: Rect) -> MapWindow {
        return MapWindow { rect: rect };
    }

    fn map_point_from_window_point(
        &self,
        win_x: u16,
        win_y: u16,
        player_position: &Point,
    ) -> Option<Point> {
        debug_assert!(win_x <= self.rect.width);
        debug_assert!(win_y <= self.rect.height);

        let x = win_x as i64;
        let x = x + (player_position.x as i64) - (self.rect.width as i64 / 2);
        if x < 0 {
            return None;
        }

        let y = win_y as i64;
        let y = y + (player_position.y as i64) - (self.rect.height as i64 / 2);
        if y < 0 {
            return None;
        }

        // Bounds checks should already have been performed, so this should
        // always succeed.
        let point = Point::new(x as usize, y as usize);

        let (test_x, test_y) = (25, 31);
        if win_x == test_x && win_y == test_y {
            log::debug!("Map point for win ({},{}) = {:?}", test_x, test_y, point);
        }

        return Some(point);
    }

    // Screen coords are 1-based (due to terminal convention)
    // fn screen_coord_from_map_coord(&self, point: &Point) -> Option<(u16, u16)> {
    //     // todo
    //     let x = u16::try_from(point.x + 1).unwrap();
    //     let y = u16::try_from(point.y + 1).unwrap();
    //     return Some((x, y));
    // }
}
