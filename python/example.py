#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Minimal example: render a periodic pattern and estimate its pose.

This mirrors the C++ TestPeriodicPatternDetector example. Run it from the build
directory where the compiled `pyvernier` module lives:

    cd build/python && python3 example.py
"""

import pyvernier as vernier

# Build a periodic pattern layout (period, nRows, nCols).
physical_period = 15.0
layout = vernier.PeriodicPatternLayout(physical_period, 31, 31)

# Render a 512x512 image of the pattern at a known pose.
true_pose = vernier.Pose(6.0, 3.0, 0.2, 2.0)  # x, y, alpha, pixelSize
image = layout.renderOrthographicProjection(true_pose, 512, 512)
print("True pose:     ", true_pose)

# Detect the pattern and estimate its pose.
detector = vernier.PeriodicPatternDetector(physical_period)
detector.setSigma(1.0)
detector.setCropFactor(0.4)
detector.compute(image)

if detector.patternFound():
    print("Estimated pose:", detector.get2DPose())
else:
    print("Pattern not found...")
