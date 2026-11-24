# Test images of the C API

The C API exposes the pattern detectors only, not the layouts that describe and
render patterns, so its tests cannot render their own images the way the C++
tests do. These images were rendered once with the C++ layouts, at known poses,
and are read back by `test_vernier_c.c`.

They are binary PGM files (`P5`, 8 bits per pixel), stored row by row, which the
test transposes into the column-major layout the API expects.

| File | Pattern | Pose of the pattern | Size |
| --- | --- | --- | --- |
| `periodicPattern.pgm` | periodic, period 15, 31 by 31 dots | x = 6, y = 3, alpha = 0.2, pixelSize = 2 | 512 x 512 |
| `nonSquarePattern.pgm` | periodic, period 8, 81 by 61 dots | x = 1.5, y = -2.5, alpha = 0.4, pixelSize = 1.2 | 300 x 420 |
| `megarenaPattern.pgm` | megarena, period 10, code of 12 bits | x = -6000, y = -3000, alpha = 0.2, pixelSize = 1 | 512 x 512 |
| `stampPattern.pgm` | stamp, period 5, bitmap `stamp.png` (69 by 69 dots, id 0) | x = 20, y = -10, alpha = 0.3, pixelSize = 1 | 512 x 512 |
| `hpcodePattern.pgm` | HP code, period 8, 33 half periods (id 0) | x = 12, y = -7, alpha = 0.5, pixelSize = 1 | 512 x 512 |

`stamp.png` is the bitmap the stamp detector is given, a copy of
`test/data/stamp/stamp+.png`.

Quantising to 8 bits costs little accuracy: the C++ detectors estimate the same
poses from these images as from the double-precision renderings, to about 0.02
for the periodic patterns, 0.001 for the megarena one and 0.001 for the stamp
and the HP code.

To render them again, for instance after a change to the layouts:

```C++
PeriodicPatternLayout layout(15.0, 31, 31);
Eigen::ArrayXXd image(512, 512);
layout.renderOrthographicProjection(Pose(6.0, 3.0, 0.2, 2.0), image);
// then write image as a binary PGM, row by row, scaled to 0-255
```

The stamp comes from `BitmapPatternLayout("stamp.png", 5.0)` and the HP code
from `HPCodePatternLayout(8.0, 17)` (17 periods, that is 33 half periods),
both rendered the same way into a 512 by 512 array.
