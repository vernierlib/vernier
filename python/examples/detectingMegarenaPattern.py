#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/detectingMegarenaPattern.cpp.

This example shows how to estimate the pose of a megarena pattern in an image.

The method is described in the following papers:

[1] A. N. Andre, P. Sandoz, B. Mauze, M. Jacquot, and G. J. Laurent,
Sensing One Nanometer over Ten Centimeters: A Micro-Encoded Target for Visual
In-Plane Position Measurement, IEEE/ASME Transactions on Mechatronics, 2020.

[2] A. N. Andre, P. Sandoz, B. Mauze, M. Jacquot, and G. J. Laurent,
Robust phase-based decoding for absolute (X, Y, Theta) positioning by vision,
IEEE Transactions on Instrumentation and Measurement, 2020.

Run it from the build directory, where the module and the images live:

    cd build/python && python3 detectingMegarenaPattern.py [--show]
"""

import sys

import pyvernier as vernier

# Loading the image
filename = "megarenaPatternImage_12bits_9um.jpg"
image = vernier.readImage(filename)

# Detecting the pattern and estimating its pose
physicalPeriod = 9  # µm
codeSize = 12  # bits
detector = vernier.MegarenaPatternDetector(physicalPeriod, codeSize)
detector.compute(image)

# Displaying the pose if a pattern has been found
print("Detector:", detector)
if detector.patternFound():
    print("Estimated pose:", detector.get2DPose())
    # The C++ draw() annotates the image in place; here it returns an RGB copy.
    image = detector.draw(image)
else:
    print("Pattern not found...")

# Showing image and its spectrum (needs a display, hence the flag)
if "--show" in sys.argv:
    detector.showControlImages()
    vernier.showImage(filename, image)
    vernier.waitKey(3000)
