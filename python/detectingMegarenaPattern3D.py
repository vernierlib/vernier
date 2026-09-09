#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/detectingMegarenaPattern3D.cpp.

This example shows how to estimate the 3D pose of a megarena pattern in an
image.

The method is described in the following paper:

[1] A. N. André, P. Sandoz, M. Jacquot, and G. J. Laurent, Pose Measurement
at Small Scale by Spectral Analysis of Periodic Patterns, International Journal
of Computer Vision, 2022.

Run it from the build directory, where the module and the images live:

    cd build/python && python3 detectingMegarenaPattern3D.py [--show]
"""

import sys

import pyvernier as vernier

# Loading the image
filename = "megarenaPatternImage_8bits_140um.png"
image = vernier.readImage(filename)

# Detecting the pattern and estimating its pose
physicalPeriod = 140  # µm
codeSize = 8  # bits
detector = vernier.MegarenaPatternDetector(physicalPeriod, codeSize)
detector.compute(image)

# Displaying the pose if a pattern has been found
print("Detector:", detector)
if detector.patternFound():
    print("Estimated 3D pose:", detector.get3DPose())
    image = detector.draw(image)
else:
    print("Pattern not found...")

# Showing image and its spectrum (needs a display, hence the flag)
if "--show" in sys.argv:
    detector.showControlImages()
    vernier.showImage(filename, image)
    vernier.waitKey(3000)
