#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/generatingPatternLayout.cpp.

This example shows how to generate pattern layouts from a JSON file.

Run it from the build directory, where the module and the JSON files live:

    cd build/python && python3 generatingPatternLayout.py [megarenaPattern]
"""

import sys

import pyvernier as vernier

# One of "stampPattern", "megarenaPattern", "bitmapPattern".
filename = sys.argv[1] if len(sys.argv) > 1 else "stampPattern"

# Loading the layout
layout = vernier.Layout.loadFromJSON(filename + ".json")
print("Pattern layout:", layout)

# Writing the layout in a PNG file
layout.saveToPNG(filename + ".png")
print(filename + ".png generation completed.")

# Writing the layout in a SVG file
layout.saveToSVG(filename + ".svg")
print(filename + ".svg generation completed.")
