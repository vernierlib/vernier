/*
Copyright 2020 Lucas Heitzmann Gabrielli.
This file is part of gdstk, distributed under the terms of the
Boost Software License - Version 1.0.  See the accompanying
LICENSE file or <http://www.boost.org/LICENSE_1_0.txt>
*/

#ifndef GDSTK_HEADER_REPETITION
#define GDSTK_HEADER_REPETITION

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS 1
#endif
#define _USE_MATH_DEFINES

#include "array.hpp"
#include "property.hpp"
#include "vec.hpp"

namespace gdstk {

enum struct RepetitionType {
    None = 0,     // No repetition
    Rectangular,  // columns × rows matrix of copies spaced by spacing
    Regular,      // columns × rows matrix along directions v1 and v2
    Explicit,     // explicit repetition at offsets
    ExplicitX,    // explicit repetitions along the x axis at coords
    ExplicitY,    // explicit repetitions along the y axis at coords
};

struct Repetition {
    RepetitionType type;
    union {
        struct {               // Rectangular and Regular
            uint64_t columns;  // Along x or v1
            uint64_t rows;     // Along y or v2
            union {
                Vec2 spacing;  // Rectangular spacing
                struct {
                    Vec2 v1;  // Regular axis 1
                    Vec2 v2;  // Regular axis 2
                };
            };
        };
        // The original element should not be explicitly included, i.e., (0, 0)
        Array<Vec2> offsets;   // Explicit
        Array<double> coords;  // ExplicitX and ExplicitY
    };

    void print() const;

    void clear();

    // This repetition instance must be zeroed before copy_from
    void copy_from(const Repetition repetition);

    // Return the number of repetitions created by this object, including the
    // original
    uint64_t get_count() const;

    // Append the offsets generated by this repetition, including the original,
    // (0, 0), as first element, to result.
    void get_offsets(Array<Vec2>& result) const;

    // Append the extrema offsets generated by this repetition, including the
    // original, to result.
    void get_extrema(Array<Vec2>& result) const;

    // Transformations are applied in the order of arguments, starting with
    // magnification and rotating at the end.  This is equivalent to the
    // transformation defined by a Reference with the same arguments.
    void transform(double magnification, bool x_reflection, double rotation);
};

}  // namespace gdstk

#endif
