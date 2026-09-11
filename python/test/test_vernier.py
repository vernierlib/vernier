#
# This file is part of the VERNIER Library.
#
# Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
#

"""Tests for the VERNIER Python bindings.

Run from the build directory where the module lives:

    cd build/python && python3 -m unittest test_vernier -v
"""

import os
import struct
import tempfile
import unittest

import numpy as np

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


class TestImages(unittest.TestCase):
    """Image I/O and the annotated images returned by `draw()`. These need the
    example data files, which CMake copies next to the module."""

    IMAGE = "megarenaPatternImage_8bits_140um.png"

    def test_read_image(self):
        if not os.path.exists(self.IMAGE):
            self.skipTest("example images not found in %s" % os.getcwd())
        image = vernier.readImage(self.IMAGE)
        self.assertEqual(image.ndim, 2)
        self.assertEqual(image.dtype, np.float64)
        # Intensities are normalized to [0, 1], as PatternDetector expects.
        self.assertGreaterEqual(image.min(), 0.0)
        self.assertLessEqual(image.max(), 1.0)

    def test_draw_returns_rgb_copy(self):
        layout = vernier.PeriodicPatternLayout(15.0, 31, 31)
        image = layout.renderOrthographicProjection(vernier.Pose(6.0, 3.0, 0.2, 2.0), 128, 128)
        detector = vernier.PeriodicPatternDetector(15.0)
        detector.compute(image)

        annotated = detector.draw(image)
        self.assertEqual(annotated.shape, (128, 128, 3))
        self.assertEqual(annotated.dtype, np.uint8)
        # The overlay is drawn on a copy, the input array is untouched.
        self.assertEqual(image.shape, (128, 128))


class TestLayout(unittest.TestCase):

    def test_load_from_json(self):
        if not os.path.exists("megarenaPattern.json"):
            self.skipTest("example layouts not found in %s" % os.getcwd())
        layout = vernier.Layout.loadFromJSON("megarenaPattern.json")
        self.assertIn("Megarena", repr(layout))

    def test_load_from_missing_file_raises(self):
        with self.assertRaises(vernier.VernierError):
            vernier.Layout.loadFromJSON("thereIsNoSuchPattern.json")

    def test_png_cell_size_and_corner_radius(self):
        layout = vernier.PeriodicPatternLayout(15.0, 31, 31)
        self.assertEqual(layout.pngCellSize, 1)
        self.assertEqual(layout.pngCornerRadius, 0.0)
        layout.pngCellSize = 8
        layout.pngCornerRadius = 0.5
        with tempfile.TemporaryDirectory() as directory:
            filename = os.path.join(directory, "rounded.png")
            layout.saveToPNG(filename)
            with open(filename, "rb") as file:
                width, height = struct.unpack(">II", file.read(24)[16:24])
        # 2 * 31 - 1 cells of 8 pixels each
        self.assertEqual((width, height), (61 * 8, 61 * 8))

        layout.pngCornerRadius = 0.6
        with self.assertRaises(vernier.VernierError):
            layout.saveToPNG(os.path.join(tempfile.gettempdir(), "invalid.png"))


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
