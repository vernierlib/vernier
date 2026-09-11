#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/detectingStampPattern.cpp.

This example shows how to estimate the pose of stamp markers in an image.

Run it from the build directory, where the module and the images live:

    cd build/python && python3 detectingStampPattern.py [--show]
"""

import sys

import pyvernier as vernier

# Loading the image
filename = "stampPatternImage_15um.jpg"
image = vernier.readImage(filename)

# Detecting the pattern and estimating its pose
physicalPeriod = 15  # µm
snapshotSize = 820  # pixels
detector = vernier.StampPatternDetector(physicalPeriod, "stampPattern.png", snapshotSize)
detector.compute(image)

# Displaying the pose if a pattern has been found
print("Detector:", detector)
if detector.patternFound():
    # `markers` is a dict {stamp id: Pose}.
    print("Found %d markers in %s" % (len(detector.markers), filename))
    for stamp_id, pose in sorted(detector.markers.items()):
        print("  Stamp %d at %s" % (stamp_id, pose))
    image = detector.draw(image)
else:
    print("Marker not found...")

# Showing image and its spectrum (needs a display, hence the flag)
if "--show" in sys.argv:
    detector.showControlImages()
    vernier.showImage(filename, image)
    vernier.waitKey(3000)
