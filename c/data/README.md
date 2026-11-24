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

Quantising to 8 bits costs little accuracy: the C++ detectors estimate the same
poses from these images as from the double-precision renderings, to about 0.02
for the periodic patterns and 0.001 for the megarena one.

To render them again, for instance after a change to the layouts:

```C++
PeriodicPatternLayout layout(15.0, 31, 31);
Eigen::ArrayXXd image(512, 512);
layout.renderOrthographicProjection(Pose(6.0, 3.0, 0.2, 2.0), image);
// then write image as a binary PGM, row by row, scaled to 0-255
```
