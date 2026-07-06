/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

/** \file vernier_python.cpp
 *
 *  Python bindings for the VERNIER library, built with nanobind.
 *
 *  This exposes the classes needed for the common workflow: build a pattern
 *  layout, render an image of it, then detect the pattern and read back its
 *  pose. Images are exchanged as 2-D float64 NumPy arrays, which map directly
 *  onto the Eigen::ArrayXXd used by the library.
 */

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/eigen/dense.h>

#include "Vernier.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace vernier;

NB_MODULE(pyvernier, m) {

    m.doc() = "Python bindings for the VERNIER pose-measurement library.";

    // Library errors surface as `pyvernier.VernierError`.
    nb::exception<Exception>(m, "VernierError");

    // ─── Pose ─────────────────────────────────────────────────────────────────

    nb::class_<Pose>(m, "Pose", "Pose of a pattern: translations and rotations.")
        .def(nb::init<>())
        .def(nb::init<double, double, double, double>(),
            "x"_a, "y"_a, "alpha"_a, "pixelSize"_a = 1.0,
            "2D pose from x, y and the rotation alpha about the Z axis.")
        .def(nb::init<double, double, double, double, double, double, double>(),
            "x"_a, "y"_a, "z"_a, "alpha"_a, "beta"_a, "gamma"_a, "pixelSize"_a = 1.0,
            "3D pose from x, y, z and the intrinsic rotations alpha, beta, gamma.")
        .def_rw("x", &Pose::x)
        .def_rw("y", &Pose::y)
        .def_rw("z", &Pose::z)
        .def_rw("alpha", &Pose::alpha)
        .def_rw("beta", &Pose::beta)
        .def_rw("gamma", &Pose::gamma)
        .def("__repr__", &Pose::toString);

    // ─── Detectors ─────────────────────────────────────────────────────────────

    nb::class_<PatternDetector>(m, "PatternDetector")
        .def("compute", nb::overload_cast<const Eigen::ArrayXXd&>(&PatternDetector::compute),
            "image"_a, "Detects the pattern in a 2-D float64 image array.")
        .def("patternFound", &PatternDetector::patternFound, "id"_a = -1)
        .def("get2DPose", &PatternDetector::get2DPose, "id"_a = -1)
        .def("get3DPose", &PatternDetector::get3DPose, "id"_a = -1)
        .def("__repr__", &PatternDetector::toString);

    nb::class_<PeriodicPatternDetector, PatternDetector>(m, "PeriodicPatternDetector")
        .def(nb::init<double>(), "physicalPeriod"_a = 1.0)
        .def("setSigma", &PeriodicPatternDetector::setSigma, "sigma"_a)
        .def("setCropFactor", &PeriodicPatternDetector::setCropFactor, "cropFactor"_a);

    nb::class_<MegarenaPatternDetector, PeriodicPatternDetector>(m, "MegarenaPatternDetector")
        .def(nb::init<double, int>(), "physicalPeriod"_a, "codeSize"_a);

    // ─── Layouts (for rendering test images) ───────────────────────────────────

    nb::class_<PatternLayout>(m, "PatternLayout")
        // The C++ API renders in place; here we allocate the array and return it.
        .def("renderOrthographicProjection",
            [](PatternLayout& self, Pose pose, int rows, int cols) {
                Eigen::ArrayXXd out(rows, cols);
                self.renderOrthographicProjection(pose, out);
                return out;
            },
            "pose"_a, "rows"_a, "cols"_a,
            "Renders a `rows x cols` image of the layout at the given pose.");

    nb::class_<PeriodicPatternLayout, PatternLayout>(m, "PeriodicPatternLayout")
        .def(nb::init<double, int, int>(), "period"_a, "nRows"_a, "nCols"_a);
}
