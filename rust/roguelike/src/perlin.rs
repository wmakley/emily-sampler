pub struct PerlinNoise {
    /// The permutation vector
    p: Vec<i32>,
}

impl PerlinNoise {
    /// Initialize with the reference values for the permutation vector
    pub fn new() -> PerlinNoise {
        let mut p = vec![
            151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103,
            30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197,
            62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20,
            125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83,
            111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54,
            65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135,
            130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124,
            123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17,
            182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153,
            101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178,
            185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
            241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184,
            84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29,
            24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
        ];

        // Duplicate the permutation vector
        let original_len = p.len();
        for i in 0..original_len {
            p.push(p[i]);
        }

        return PerlinNoise { p: p };
    }

    pub fn noise(&self, x: f64, y: f64, z: f64) -> f64 {
        // Find the unit cube that contains the point
        let X: i32 = (x.floor() as i32) & 255;
        let Y: i32 = (y.floor() as i32) & 255;
        let Z: i32 = (z.floor() as i32) & 255;

        // Find relative x, y,z of point in cube
        let x = x - x.floor();
        let y = y - y.floor();
        let z = z - z.floor();

        // Compute fade curves for each of x, y, z
        let u: f64 = fade(x);
        let v: f64 = fade(y);
        let w: f64 = fade(z);

        // Hash coordinates of the 8 cube corners
        let A: i32 = self.p[X as usize] + Y;
        let AA: i32 = self.p[A as usize] + Z;
        let AB: i32 = self.p[A as usize + 1] + Z;
        let B: i32 = self.p[X as usize + 1] + Y;
        let BA: i32 = self.p[B as usize] + Z;
        let BB: i32 = self.p[B as usize + 1] + Z;

        // Add blended results from 8 corners of cube
        let res: f64 = lerp(
            w,
            lerp(
                v,
                lerp(
                    u,
                    grad(self.p[AA as usize], x, y, z),
                    grad(self.p[BA as usize], x - 1.0, y, z),
                ),
                lerp(
                    u,
                    grad(self.p[AB as usize], x, y - 1.0, z),
                    grad(self.p[BB as usize], x - 1.0, y - 1.0, z),
                ),
            ),
            lerp(
                v,
                lerp(
                    u,
                    grad(self.p[AA as usize + 1], x, y, z - 1.0),
                    grad(self.p[BA as usize + 1], x - 1.0, y, z - 1.0),
                ),
                lerp(
                    u,
                    grad(self.p[AB as usize + 1], x, y - 1.0, z - 1.0),
                    grad(self.p[BB as usize + 1], x - 1.0, y - 1.0, z - 1.0),
                ),
            ),
        );
        return (res + 1.0) / 2.0;
    }
}

fn fade(t: f64) -> f64 {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

fn lerp(t: f64, a: f64, b: f64) -> f64 {
    return a + t * (b - a);
}

fn grad(hash: i32, x: f64, y: f64, z: f64) -> f64 {
    let h: i32 = hash & 15;
    // Convert lower 4 bits of hash inot 12 gradient directions
    let u: f64 = if h < 8 { x } else { y };
    let v: f64 = if h < 4 {
        y
    } else {
        if h == 12 || h == 14 {
            x
        } else {
            z
        }
    };
    return (if (h & 1) == 0 { u } else { -u }) + (if (h & 2) == 0 { v } else { -v });
}
