#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Python port of examples/analysingImage.cpp.

This example shows how to analyse the spectrum of an image and to calculate
the phase planes of a periodic pattern if two peaks have been found.

The method is described in the paper: A. N. Andre, P. Sandoz, B. Mauze,
M. Jacquot, and G. J. Laurent, Sensing One Nanometer over Ten Centimeters:
A Micro-Encoded Target for Visual In-Plane Position Measurement, IEEE/ASME
Transactions on Mechatronics, 2020.

Run it from the build directory, where the module and the images live:

    cd build/python && python3 analysingImage.py [--show]
"""

import sys

import pyvernier as vernier

# Loading the image
image = vernier.readImage("megarenaPatternImage_12bits_9um.jpg")

# Analysing the spectrum of the image
patternPhase = vernier.PatternPhase()
patternPhase.compute(image)

# Displaying the phase planes if two peaks have been found
if patternPhase.peaksFound():
    print("Phase plane 1:", patternPhase.getPlane1())
    print("Phase plane 2:", patternPhase.getPlane2())
else:
    print("Peaks not found...")

# Showing image spectrum and fringes (needs a display, hence the flag)
if "--show" in sys.argv:
    patternPhase.showControlImages()
    vernier.waitKey(3000)
