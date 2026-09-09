#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/renderingPatternImage.cpp.

This example shows how to render an image of a pattern from a virtual camera
at a given pose and with an orthographic projection.

Run it from the build directory, where the module and the JSON files live:

    cd build/python && python3 renderingPatternImage.py [--show]
"""

import sys

import pyvernier as vernier

filename = "megarenaPattern.json"

# Loading the layout
layout = vernier.Layout.loadFromJSON(filename)
print("Pattern layout:", layout)

# Setting the pose (could be 3D)
x = -6000.0  # µm
y = -8000.0  # µm
alpha = 0.2  # rad
pixelSize = 2.0  # µm
patternPose = vernier.Pose(x, y, alpha, pixelSize)

# Rendering image
# The C++ version renders into a 512x512 Mat; here the array is returned.
image = layout.renderOrthographicProjection(patternPose, 512, 512)
print("Rendered a %dx%d image, intensities in [%.2f, %.2f]"
      % (image.shape[0], image.shape[1], image.min(), image.max()))

# Showing the image (needs a display, hence the flag)
if "--show" in sys.argv:
    vernier.showImage(filename, image)
    vernier.waitKey(3000)
else:
    vernier.saveImage("renderedPatternImage.png", image)
    print("renderedPatternImage.png written.")
