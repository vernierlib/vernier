#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/detectingHPCodePattern.cpp.

This example shows how to estimate the pose of HP code markers in an image.

The method is described in the following paper:

[1] A. N. Andre et al., "Automating Robotic Micro-Assembly of Fluidic Chips
and Single Fiber Compression Tests Based-on Visual Measurement With
High-Precision Fiducial Markers", IEEE Transactions on Automation Science and
Engineering, 2022.

Run it from the build directory, where the module and the images live:

    cd build/python && python3 detectingHPCodePattern.py [--show]
"""

import sys

import pyvernier as vernier

# Loading the image
filename = "HPCodeImage_20um.png"
image = vernier.readImage(filename)

# Detecting the pattern and estimating its pose
physicalPeriod = 20  # µm
snapshotSize = 256  # pixels
detector = vernier.HPCodePatternDetector(physicalPeriod, 33, snapshotSize)
detector.compute(image)

# Displaying the pose if a pattern has been found
print("Detector:", detector)
if detector.patternFound():
    # `markers` is a dict {code id: Pose}.
    print("Found %d markers in %s" % (len(detector.markers), filename))
    for code_id, pose in sorted(detector.markers.items()):
        print("  Stamp %d at %s" % (code_id, pose))
    image = detector.draw(image)
else:
    print("Marker not found...")

# Showing image and its spectrum (needs a display, hence the flag)
if "--show" in sys.argv:
    detector.showControlImages()
    vernier.showImage(filename, image)
    vernier.waitKey(3000)
