use std::convert::TryFrom;
use std::fmt;
use termion;
use termion::color::{self, Color};

pub struct Map {
    pub tiles: Box<[Tile]>,
    pub width: usize,
    pub height: usize,
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Point {
    pub x: usize,
    pub y: usize,
}

impl Point {
    pub fn new(x: usize, y: usize) -> Point {
        return Point { x: x, y: y };
    }

    pub fn try_from_ints(x: i32, y: i32) -> Result<Point, std::num::TryFromIntError> {
        let xu = usize::try_from(x)?;
        let yu = usize::try_from(y)?;
        return Ok(Point { x: xu, y: yu });
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Rect {
    pub left: usize,
    pub top: usize,
    pub width: u16,
    pub height: u16,
}

impl Map {
    pub fn new(width: usize, height: usize) -> Map {
        let tiles = vec![Tile::of_type(GROUND); width * height].into_boxed_slice();

        let mut map = Map {
            width: width,
            height: height,
            tiles: tiles,
        };

        map.gen_walls();

        return map;
    }

    fn gen_walls(&mut self) {
        // side walls
        for i in 0..self.height {
            let left_index = self.index_from_point(&Point { x: 0, y: i }).unwrap();
            self.tiles[left_index] = Tile::of_type(WALL);
            let right_index = self
                .index_from_point(&Point {
                    x: self.width - 1,
                    y: i,
                })
                .unwrap();
            self.tiles[right_index] = Tile::of_type(WALL);
        }

        // top and bottom walls
        for i in 1..self.width - 1 {
            let top_index = self.index_from_point(&Point { x: i, y: 0 }).unwrap();
            self.tiles[top_index] = Tile::of_type(WALL2);
            let bottom_index = self
                .index_from_point(&Point {
                    x: i,
                    y: self.height - 1,
                })
                .unwrap();
            self.tiles[bottom_index] = Tile::of_type(WALL2);
        }
    }

    pub fn tile_at(&self, point: &Point) -> Option<&Tile> {
        let index = self.index_from_point(point);
        return index.map(|i| &self.tiles[i]);
    }

    pub fn index_from_point(&self, point: &Point) -> Option<usize> {
        if point.x >= self.width || point.y >= self.height {
            return None;
        }
        return Some(point.x + (self.width * point.y));
    }

    pub fn point_from_index(&self, index: usize) -> Option<Point> {
        if index > self.tiles.len() {
            return None;
        }

        let row = index / self.width;
        let remainder = index % self.width;

        return Some(Point {
            x: remainder,
            y: row,
        });
    }
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Tile {
    tile_type: usize,
}

impl Tile {
    pub fn of_type(tile_type: usize) -> Tile {
        debug_assert!(tile_type < TILE_TYPES.len());
        return Tile {
            tile_type: tile_type,
        };
    }

    pub fn get_type(&self) -> &TileType {
        return &TILE_TYPES[self.tile_type];
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
#[allow(unused)]
pub enum TileColor {
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    LightBlack,
    LightRed,
    LightGreen,
    LightYellow,
    LightBlue,
    LightMagenta,
    LightCyan,
    LightWhite,
    Rgb(u8, u8, u8),
}

impl Color for TileColor {
    fn write_fg(&self, f: &mut fmt::Formatter) -> fmt::Result {
        return match self {
            TileColor::Black => color::Black.write_fg(f),
            TileColor::Red => color::Red.write_fg(f),
            TileColor::Green => color::Green.write_fg(f),
            TileColor::Yellow => color::Yellow.write_fg(f),
            TileColor::Blue => color::Blue.write_fg(f),
            TileColor::Magenta => color::Magenta.write_fg(f),
            TileColor::Cyan => color::Cyan.write_fg(f),
            TileColor::White => color::White.write_fg(f),
            TileColor::LightBlack => color::LightBlack.write_fg(f),
            TileColor::LightRed => color::LightRed.write_fg(f),
            TileColor::LightGreen => color::LightGreen.write_fg(f),
            TileColor::LightYellow => color::LightYellow.write_fg(f),
            TileColor::LightBlue => color::LightBlue.write_fg(f),
            TileColor::LightMagenta => color::LightMagenta.write_fg(f),
            TileColor::LightCyan => color::LightCyan.write_fg(f),
            TileColor::LightWhite => color::LightWhite.write_fg(f),
            TileColor::Rgb(r, g, b) => color::Rgb(*r, *g, *b).write_fg(f),
        };
    }

    fn write_bg(&self, f: &mut fmt::Formatter) -> fmt::Result {
        return match self {
            TileColor::Black => color::Black.write_bg(f),
            TileColor::Red => color::Red.write_bg(f),
            TileColor::Green => color::Green.write_bg(f),
            TileColor::Yellow => color::Yellow.write_bg(f),
            TileColor::Blue => color::Blue.write_bg(f),
            TileColor::Magenta => color::Magenta.write_bg(f),
            TileColor::Cyan => color::Cyan.write_bg(f),
            TileColor::White => color::White.write_bg(f),
            TileColor::LightBlack => color::LightBlack.write_bg(f),
            TileColor::LightRed => color::LightRed.write_bg(f),
            TileColor::LightGreen => color::LightGreen.write_bg(f),
            TileColor::LightYellow => color::LightYellow.write_bg(f),
            TileColor::LightBlue => color::LightBlue.write_bg(f),
            TileColor::LightMagenta => color::LightMagenta.write_bg(f),
            TileColor::LightCyan => color::LightCyan.write_bg(f),
            TileColor::LightWhite => color::LightWhite.write_bg(f),
            TileColor::Rgb(r, g, b) => color::Rgb(*r, *g, *b).write_bg(f),
        };
    }
}

pub struct TileType {
    pub char: char,
    pub passable: bool,
    pub bg_color: TileColor,
    pub fg_color: TileColor,
}

const TILE_TYPES: &'static [TileType] = &[
    TileType {
        char: '.',
        passable: true,
        bg_color: TileColor::Black,
        fg_color: TileColor::White,
    },
    TileType {
        char: '#',
        passable: false,
        bg_color: TileColor::Black,
        fg_color: TileColor::White,
    },
    TileType {
        char: '%',
        passable: false,
        bg_color: TileColor::Black,
        fg_color: TileColor::LightCyan,
    },
    TileType {
        char: ' ',
        passable: false,
        bg_color: TileColor::Black,
        fg_color: TileColor::White,
    },
];

pub const GROUND: usize = 0;
pub const WALL: usize = 1;
pub const WALL2: usize = 2;
pub const BLANK: usize = 3;
