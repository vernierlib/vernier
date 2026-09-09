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


class TestBackend(unittest.TestCase):
    """Backend selection. The CUDA cases only run when the library was built
    with -DUSE_CUDA=ON and a device is present."""

    def _render(self):
        layout = vernier.PeriodicPatternLayout(15.0, 31, 31)
        return layout.renderOrthographicProjection(vernier.Pose(6.0, 3.0, 0.2, 2.0), 512, 512)

    def test_default_is_cpu(self):
        detector = vernier.PeriodicPatternDetector(15.0)
        self.assertEqual(detector.getBackend(), vernier.Backend.CPU)

    def test_cuda_request_without_cuda_raises(self):
        if vernier.cudaAvailable():
            self.skipTest("CUDA is available here")
        detector = vernier.PeriodicPatternDetector(15.0)
        with self.assertRaises(vernier.VernierError):
            detector.setBackend(vernier.Backend.CUDA)

    @unittest.skipUnless(vernier.cudaAvailable(), "built without CUDA, or no CUDA device")
    def test_cuda_matches_cpu(self):
        image = self._render()
        poses = []
        for backend in (vernier.Backend.CPU, vernier.Backend.CUDA):
            detector = vernier.PeriodicPatternDetector(15.0)
            detector.setSigma(1.0)
            detector.setCropFactor(0.4)
            detector.setBackend(backend)
            self.assertEqual(detector.getBackend(), backend)
            detector.compute(image)
            self.assertTrue(detector.patternFound())
            poses.append(detector.get2DPose())

        # Both backends work in double precision, so they should agree closely.
        self.assertAlmostEqual(poses[0].x, poses[1].x, delta=1e-6)
        self.assertAlmostEqual(poses[0].y, poses[1].y, delta=1e-6)
        self.assertAlmostEqual(poses[0].alpha, poses[1].alpha, delta=1e-9)


if __name__ == "__main__":
    unittest.main()
