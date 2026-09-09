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
 *
 *  The phase-retrieval stage (PatternPhase) can run on the CPU or, when the
 *  library is built with -DUSE_CUDA=ON, on a CUDA device; the backend is
 *  selected at runtime through `Backend` / `setBackend()`.
 */

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/eigen/dense.h>

#include <cstring>

#include "Vernier.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace vernier;

namespace {

    /** A (rows, cols, 3) uint8 RGB NumPy array, used for the annotated images
     * returned by `draw()`. Grayscale images stay 2-D float64 (Eigen) arrays. */
    using RgbArray = nb::ndarray<nb::numpy, uint8_t, nb::ndim<3>>;

    /** Copies a BGR cv::Mat into a fresh RGB NumPy array owning its buffer. */
    RgbArray matToRgbArray(const cv::Mat& bgr) {
        cv::Mat rgb;
        cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
        size_t size = (size_t) rgb.rows * rgb.cols * 3;
        uint8_t* data = new uint8_t[size];
        std::memcpy(data, rgb.data, size);
        nb::capsule owner(data, [](void* p) noexcept {
            delete[] (uint8_t*) p;
        });
        return RgbArray(data, {(size_t) rgb.rows, (size_t) rgb.cols, 3}, owner);
    }

    /** Wraps an RGB NumPy array as a BGR cv::Mat (no copy of the pixels). */
    cv::Mat rgbArrayToMat(const nb::ndarray<const uint8_t, nb::ndim<3>, nb::c_contig>& image) {
        if (image.shape(2) != 3) {
            throw Exception("A color image must have 3 channels.");
        }
        cv::Mat rgb((int) image.shape(0), (int) image.shape(1), CV_8UC3,
                (void*) image.data());
        cv::Mat bgr;
        cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
        return bgr;
    }

    /** Converts a [0, 1] grayscale array into the 8-bit BGR image OpenCV displays
     * and writes, which is also what the `draw()` overlays are painted on. */
    cv::Mat arrayToBgrMat(const Eigen::ArrayXXd& image) {
        cv::Mat image64F, image8U, bgr;
        vernier::eigen2cv(image, image64F);
        image64F.convertTo(image8U, CV_8U, 255.0);
        cv::cvtColor(image8U, bgr, cv::COLOR_GRAY2BGR);
        return bgr;
    }

    /** Reads an image file as a 2-D float64 array in [0, 1], applying the same
     * conversion as PatternDetector::compute(cv::Mat) so that the Python and C++
     * examples feed the detectors exactly the same pixels. */
    Eigen::ArrayXXd readImage(const std::string& filename) {
        cv::Mat image = cv::imread(filename, cv::IMREAD_UNCHANGED);
        if (image.empty()) {
            throw Exception("Unable to read the image file " + filename + ".");
        }
        cv::Mat gray;
        if (image.channels() > 1) {
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        } else {
            gray = image;
        }
        double scale;
        switch (gray.depth()) {
            case CV_8U: scale = 1.0 / 255.0; break;
            case CV_16U: scale = 1.0 / 65535.0; break;
            case CV_32F:
            case CV_64F: scale = 1.0; break;
            default: throw Exception("The image must be 8-bit, 16-bit or floating-point.");
        }
        cv::Mat image64F;
        gray.convertTo(image64F, CV_64F, scale);
        Eigen::ArrayXXd array;
        vernier::cv2eigen(image64F, array);
        return array;
    }
}

NB_MODULE(pyvernier, m) {

    m.doc() = "Python bindings for the VERNIER pose-measurement library.";

    // Library errors surface as `pyvernier.VernierError`.
    nb::exception<Exception>(m, "VernierError");

    // ─── Compute backend ─────────────────────────────────────────────────────

    nb::enum_<Backend>(m, "Backend", "Backend running the phase-retrieval pipeline.")
        .value("CPU", Backend::CPU, "Reference CPU path (FFTW or Ooura).")
        .value("CUDA", Backend::CUDA, "GPU path (cuFFT + custom kernels).");

    m.def("cudaAvailable", &PatternPhase::cudaAvailable,
        "True if the library was built with -DUSE_CUDA=ON and a CUDA device is present.");

    // ─── Images ──────────────────────────────────────────────────────────────

    // Images are 2-D float64 arrays in [0, 1]; the annotated images produced by
    // `draw()` are (rows, cols, 3) uint8 RGB arrays.

    m.def("readImage", &readImage, "filename"_a,
        "Reads an image file as a 2-D float64 array in [0, 1] (converted to grayscale).");

    m.def("saveImage",
        [](const std::string& filename, const Eigen::ArrayXXd& image) {
            cv::imwrite(filename, arrayToBgrMat(image));
        }, "filename"_a, "image"_a, "Writes a grayscale image to a file.");

    m.def("saveImage",
        [](const std::string& filename,
                nb::ndarray<const uint8_t, nb::ndim<3>, nb::c_contig> image) {
            cv::imwrite(filename, rgbArrayToMat(image));
        }, "filename"_a, "image"_a, "Writes an RGB image to a file.");

    m.def("showImage",
        [](const std::string& windowName, const Eigen::ArrayXXd& image) {
            cv::imshow(windowName, arrayToBgrMat(image));
        }, "windowName"_a, "image"_a,
        "Opens a window showing a grayscale image (needs a display; call waitKey "
        "afterwards to keep the window open).");

    m.def("showImage",
        [](const std::string& windowName,
                nb::ndarray<const uint8_t, nb::ndim<3>, nb::c_contig> image) {
            cv::imshow(windowName, rgbArrayToMat(image));
        }, "windowName"_a, "image"_a, "Opens a window showing an RGB image.");

    m.def("waitKey",
        [](int delay) {
            return cv::waitKey(delay);
        }, "delay"_a = 0,
        "Waits `delay` milliseconds (0 = forever) for a key press, refreshing the "
        "windows opened by showImage / showControlImages.");

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

    // ─── Phase planes ────────────────────────────────────────────────────────

    nb::class_<PhasePlane>(m, "PhasePlane",
            "Plane fitted to an unwrapped phase: a*x + b*y + c = phase.")
        .def(nb::init<double, double, double>(), "a"_a, "b"_a, "c"_a)
        .def_rw("a", &PhasePlane::a)
        .def_rw("b", &PhasePlane::b)
        .def_rw("c", &PhasePlane::c)
        .def("getPosition", &PhasePlane::getPosition,
            "physicalPeriod"_a, "y"_a = 0.0, "x"_a = 0.0, "periodShift"_a = 0)
        .def("__repr__", &PhasePlane::toString);

    // ─── Phase retrieval ─────────────────────────────────────────────────────

    nb::class_<PatternPhase>(m, "PatternPhase",
            "Phase-retrieval pipeline: the stage a detector spends its time in, and "
            "the one the CUDA backend accelerates.")
        .def(nb::init<>())
        .def(nb::init<int, int>(), "nRows"_a, "nCols"_a)
        .def("resize", &PatternPhase::resize, "nRows"_a, "nCols"_a)
        .def("compute", nb::overload_cast<const Eigen::ArrayXXd&>(&PatternPhase::compute),
            "image"_a, "Runs the pipeline on a 2-D float64 image array.")
        .def("setBackend", &PatternPhase::setBackend, "backend"_a,
            "Selects the backend. Raises VernierError if CUDA is requested but the "
            "library was built without it or no device is present.")
        .def("getBackend", &PatternPhase::getBackend)
        .def_static("cudaAvailable", &PatternPhase::cudaAvailable)
        .def("peaksFound", &PatternPhase::peaksFound)
        .def("setSigma", &PatternPhase::setSigma, "sigma"_a)
        .def("setCropFactor", &PatternPhase::setCropFactor, "cropFactor"_a)
        .def("getUnwrappedPhase1", &PatternPhase::getUnwrappedPhase1)
        .def("getUnwrappedPhase2", &PatternPhase::getUnwrappedPhase2)
        .def("getPlane1", &PatternPhase::getPlane1)
        .def("getPlane2", &PatternPhase::getPlane2)
        .def("showControlImages", &PatternPhase::showControlImages,
            "Opens windows showing the spectrum and the fringes (needs a display).");

    // ─── Detectors ─────────────────────────────────────────────────────────────

    nb::class_<PatternDetector>(m, "PatternDetector")
        .def("compute", nb::overload_cast<const Eigen::ArrayXXd&>(&PatternDetector::compute),
            "image"_a, "Detects the pattern in a 2-D float64 image array.")
        .def("patternFound", &PatternDetector::patternFound, "id"_a = -1)
        .def("get2DPose", &PatternDetector::get2DPose, "id"_a = -1)
        .def("get3DPose", &PatternDetector::get3DPose, "id"_a = -1)
        .def("getAll3DPoses", &PatternDetector::getAll3DPoses, "id"_a = -1)
        .def("patternCount", &PatternDetector::patternCount)
        // The C++ `draw(cv::Mat&)` annotates in place; here the image is returned.
        .def("draw",
            [](PatternDetector& self, const Eigen::ArrayXXd& image) {
                cv::Mat annotated = arrayToBgrMat(image);
                self.draw(annotated);
                return matToRgbArray(annotated);
            },
            "image"_a,
            "Returns an RGB copy of the image with the detection drawn on it.")
        .def("showControlImages", &PatternDetector::showControlImages,
            "Opens windows showing the intermediate images (needs a display).")
        .def("__repr__", &PatternDetector::toString);

    nb::class_<PeriodicPatternDetector, PatternDetector>(m, "PeriodicPatternDetector")
        .def(nb::init<double>(), "physicalPeriod"_a = 1.0)
        .def("setSigma", &PeriodicPatternDetector::setSigma, "sigma"_a)
        .def("setCropFactor", &PeriodicPatternDetector::setCropFactor, "cropFactor"_a)
        // The phase stage is owned by the detector, hence `reference_internal`:
        // the returned object keeps the detector alive.
        .def("getPatternPhase", &PeriodicPatternDetector::getPatternPhase,
            nb::rv_policy::reference_internal,
            "The underlying phase-retrieval stage (backend selection, phases, ...).")
        .def("setBackend",
            [](PeriodicPatternDetector& self, Backend backend) {
                self.getPatternPhase()->setBackend(backend);
            },
            "backend"_a, "Shortcut for `getPatternPhase().setBackend(...)`.")
        .def("getBackend",
            [](PeriodicPatternDetector& self) {
                return self.getPatternPhase()->getBackend();
            },
            "Shortcut for `getPatternPhase().getBackend()`.");

    nb::class_<MegarenaPatternDetector, PeriodicPatternDetector>(m, "MegarenaPatternDetector")
        .def(nb::init<double, int>(), "physicalPeriod"_a, "codeSize"_a);

    nb::class_<HPCodePatternDetector, PeriodicPatternDetector>(m, "HPCodePatternDetector")
        .def(nb::init<double, int, int>(),
            "physicalPeriod"_a = 1.0, "numberHalfPeriods"_a = 37, "snapshotSize"_a = 128)
        // `markers` is a std::map<int, Pose>, converted to a dict of copies.
        .def_ro("markers", &HPCodePatternDetector::markers,
            "Poses of the detected codes, keyed by code id.");

    nb::class_<StampPatternDetector, PeriodicPatternDetector>(m, "StampPatternDetector")
        .def(nb::init<double, const std::string&, int>(),
            "physicalPeriod"_a, "filename"_a, "snapshotSize"_a)
        .def("addBitmap", &StampPatternDetector::addBitmap, "filename"_a)
        .def_ro("markers", &StampPatternDetector::markers,
            "Poses of the detected stamps, keyed by stamp id.");

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
            "Renders a `rows x cols` image of the layout at the given pose.")
        .def("saveToPNG", &PatternLayout::saveToPNG, "filename"_a = "")
        .def("saveToSVG", &PatternLayout::saveToSVG, "filename"_a = "")
        .def("saveToJSON", &PatternLayout::saveToJSON, "filename"_a = "")
        .def("__repr__", &PatternLayout::toString);

    nb::class_<Layout>(m, "Layout", "Factory building pattern layouts from JSON files.")
        // loadFromJSON hands back a new PatternLayout; Python owns it from here.
        .def_static("loadFromJSON", &Layout::loadFromJSON, "filename"_a,
            nb::rv_policy::take_ownership,
            "Builds the pattern layout described by a JSON file.");

    nb::class_<PeriodicPatternLayout, PatternLayout>(m, "PeriodicPatternLayout")
        .def(nb::init<double, int, int>(), "period"_a, "nRows"_a, "nCols"_a);
}
