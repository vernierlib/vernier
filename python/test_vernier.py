#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Tests for the VERNIER Python bindings.

Run from the build directory where the module lives:

    cd build/python && python3 -m unittest test_vernier -v
"""

import unittest

import pyvernier as vernier


class TestPose(unittest.TestCase):

    def test_fields(self):
        pose = vernier.Pose(6.0, 3.0, 0.2, 2.0)
        self.assertAlmostEqual(pose.x, 6.0)
        self.assertAlmostEqual(pose.y, 3.0)
        self.assertAlmostEqual(pose.alpha, 0.2)
        self.assertIn("6", repr(pose))


class TestRoundtrip(unittest.TestCase):
    """Render a periodic pattern at a known pose, then check the detector
    recovers it."""

    def test_render_then_detect(self):
        period = 15.0
        layout = vernier.PeriodicPatternLayout(period, 31, 31)
        image = layout.renderOrthographicProjection(vernier.Pose(6.0, 3.0, 0.2, 2.0), 512, 512)

        detector = vernier.PeriodicPatternDetector(period)
        detector.setSigma(1.0)
        detector.setCropFactor(0.4)
        detector.compute(image)

        self.assertTrue(detector.patternFound())
        pose = detector.get2DPose()
        self.assertAlmostEqual(pose.x, 6.0, delta=0.1)
        self.assertAlmostEqual(pose.y, 3.0, delta=0.1)
        self.assertAlmostEqual(pose.alpha, 0.2, delta=0.02)


if __name__ == "__main__":
    unittest.main()
