use crate::perlin::PerlinNoise;
use std::fmt;
use termion;
use termion::color::{self, Color};

/// MAX_WIDTH and MAX_HEIGHT have been chosen so that they do not exceed
/// i32::MAX when multiplied together, to make signed arithmetic easier.
pub const MAX_WIDTH: usize = 46_340;
pub const MAX_HEIGHT: usize = 46_340;

pub struct Map {
    pub tiles: Box<[Tile]>,
    pub width: usize,
    pub height: usize,
}

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Tile {
    tile_type: usize,
}

/// Reference to a point on the Map. No points can ever be
/// less than zero (upper-left corner). x and y may also not
/// exceed MAX_WIDTH and MAX_HEIGHT.
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Point {
    pub x: usize,
    pub y: usize,
}

impl Point {
    /// Create a point with bounds checking against MAX_WIDTH and MAX_HEIGHT.
    /// (Panics if out of bounds.)
    pub fn new(x: usize, y: usize) -> Point {
        assert!(x <= MAX_WIDTH);
        assert!(y <= MAX_HEIGHT);
        return Point { x: x, y: y };
    }
}

impl fmt::Display for Point {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        return write!(f, "{:?}", self);
    }
}

impl Map {
    pub fn new(width: usize, height: usize) -> Map {
        assert!(width <= MAX_WIDTH);
        assert!(width > 0);
        assert!(height <= MAX_HEIGHT);
        assert!(height > 0);

        let tiles = vec![Tile::of_type(GROUND); width * height].into_boxed_slice();

        let mut map = Map {
            width: width,
            height: height,
            tiles: tiles,
        };

        map.gen_perlin();

        return map;
    }

    fn gen_perlin(&mut self) {
        let width_f = self.width as f64;
        let height_f = self.height as f64;
        let perlin = PerlinNoise::new();
        for i in 0..self.height {
            for j in 0..self.width {
                let x = j as f64 / width_f;
                let y = i as f64 / height_f;

                let n = perlin.noise(10.0 * x, 10.0 * y, 0.8);

                let point = Point { x: j, y: i };

                // water
                if n < 0.35 {
                    self.set_tile_at(&point, WATER).unwrap();
                }
                // floors or plains
                else if n >= 0.35 && n < 0.6 {
                    self.set_tile_at(&point, GROUND).unwrap();
                }
                // walls / mountains
                else if n >= 0.6 && n < 0.8 {
                    self.set_tile_at(&point, MOUNTAIN).unwrap();
                }
                // ice / snow
                else {
                    self.set_tile_at(&point, SNOW).unwrap();
                }
            }
        }
    }

    pub fn tile_at(&self, point: &Point) -> Option<&Tile> {
        let index = self.index_from_point(point);
        return index.map(|i| &self.tiles[i]);
    }

    pub fn set_tile_at(&mut self, point: &Point, tile_type: usize) -> Option<usize> {
        let index = self.index_from_point(point)?;
        self.tiles[index].tile_type = tile_type;
        return Some(tile_type);
    }

    pub fn index_from_point(&self, point: &Point) -> Option<usize> {
        if point.x >= self.width || point.y >= self.height {
            return None;
        }
        return Some(point.x + (self.width * point.y));
    }

    #[allow(unused)]
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
        char: ' ',
        passable: false,
        bg_color: TileColor::Black,
        fg_color: TileColor::White,
    },
    TileType {
        char: '~',
        passable: true,
        bg_color: TileColor::Black,
        fg_color: TileColor::Blue,
    },
    TileType {
        char: '^',
        passable: false,
        bg_color: TileColor::LightBlack,
        fg_color: TileColor::White,
    },
    TileType {
        char: 'S',
        passable: false,
        bg_color: TileColor::LightBlack,
        fg_color: TileColor::White,
    },
];

pub const GROUND: usize = 0;
pub const WALL: usize = 1;
pub const BLANK: usize = 2;
pub const WATER: usize = 3;
pub const MOUNTAIN: usize = 4;
pub const SNOW: usize = 5;
