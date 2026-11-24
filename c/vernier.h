/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

/** \file vernier.h
 *
 *  C API of the VERNIER library.
 *
 *  A flat C interface over the pattern detectors of the C++ library, so that
 *  they can be used from any language able to load a C shared library (C, C#,
 *  LabVIEW, Julia, Rust, ctypes, ...) without depending on a C++ ABI. The
 *  layouts, which describe and render patterns, are only available through the
 *  C++ classes.
 *
 *  Conventions:
 *
 *  - Detectors are opaque handles, created by the vernierDetectorNew* and
 *    vernierDetectorLoadFromJSON functions, which return NULL on failure, and
 *    released by vernierDetectorFree(). Freeing a null handle is a no-op.
 *  - Every other function returns a VernierStatus. Anything but VERNIER_OK
 *    leaves the detector unchanged and a description of the problem in
 *    vernierLastError().
 *  - Images are arrays of rows * cols doubles in [0, 1], stored column by
 *    column (element (row, col) is at image[row + col * rows]), which is the
 *    layout of Eigen and of Matlab, Fortran and Julia arrays.
 *  - Strings are copied into a caller-provided buffer. When the buffer is too
 *    small the text is truncated, null-terminated, and VERNIER_INVALID_ARGUMENT
 *    is returned.
 */

/* the guard is not VERNIER_H, which is the one of the C++ header Vernier.hpp */
#ifndef VERNIER_C_H
#define VERNIER_C_H

#include <stddef.h>

/* the library hides everything else, so that only these functions are exported
 * and the C++ internals cannot be bound to by mistake. The attributes are only
 * needed to build the library: callers get a plain declaration, which header
 * parsers such as Matlab's loadlibrary can read. */
#if defined(_WIN32)
#  if defined(VERNIER_C_BUILD)
#    define VERNIER_API __declspec(dllexport)
#  else
#    define VERNIER_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && defined(VERNIER_C_BUILD)
#  define VERNIER_API __attribute__((visibility("default")))
#else
#  define VERNIER_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Result of a call. */
typedef enum VernierStatus {
    VERNIER_OK = 0,
    /** The library reported an error, see vernierLastError(). */
    VERNIER_ERROR = 1,
    /** A null pointer or an out-of-range size was passed. */
    VERNIER_INVALID_ARGUMENT = 2
} VernierStatus;

/** Compute backend of the phase computation. CUDA needs a library built with
 * -DUSE_CUDA=ON and a CUDA device, see vernierCudaAvailable(). */
typedef enum VernierBackend {
    VERNIER_BACKEND_CPU = 0,
    VERNIER_BACKEND_CUDA = 1
} VernierBackend;

/** Pose of a pattern: translations x, y, z (in the unit of the period of the
 * pattern) and intrinsic rotations alpha about Z, beta about Y and gamma about
 * X (in radians). is3D is zero when the pose only defines x, y and alpha. */
typedef struct VernierPose {
    double x;
    double y;
    double z;
    double alpha;
    double beta;
    double gamma;
    double pixelSize;
    int is3D;
} VernierPose;

/** Detector of patterns, created by vernierDetectorNew() and friends. */
typedef struct VernierDetector VernierDetector;

/* ─── Library ─────────────────────────────────────────────────────────────── */

/** Description of the last error, or an empty string if the last call
 * succeeded. The string belongs to the library and is valid until the next
 * call from the same thread. */
VERNIER_API const char * vernierLastError(void);

/** Version of the library, e.g. "1.2". */
VERNIER_API const char * vernierVersion(void);

/** Returns non-zero if the library was built with CUDA support and a device is
 * present, that is, if VERNIER_BACKEND_CUDA can be selected. */
VERNIER_API int vernierCudaAvailable(void);

/* ─── Poses ───────────────────────────────────────────────────────────────── */

/** Writes a description of the pose into buffer. */
VERNIER_API VernierStatus vernierPoseToString(const VernierPose * pose, char * buffer, size_t size);

/** Writes the 4x4 matrix transforming a 3D point from the pattern frame to the
 * camera frame, column by column. */
VERNIER_API VernierStatus vernierPoseCameraToPatternMatrix(const VernierPose * pose, double * matrix);

/** Writes the 4x4 matrix transforming a 3D point from the camera frame to the
 * pattern frame, column by column. */
VERNIER_API VernierStatus vernierPosePatternToCameraMatrix(const VernierPose * pose, double * matrix);

/* ─── Detectors ───────────────────────────────────────────────────────────── */

/** Creates a detector from its class name: "PeriodicPattern",
 * "MegarenaPattern", "BitmapPattern", "HPCodePattern" or "StampPattern".
 * Returns NULL on failure, see vernierLastError(), as do the other constructors
 * below. */
VERNIER_API VernierDetector * vernierDetectorNew(const char * classname, VernierBackend backend);

/** Creates the detector described by a JSON pattern file. */
VERNIER_API VernierDetector * vernierDetectorLoadFromJSON(const char * filename, VernierBackend backend);

/** Creates a detector of periodic patterns of the given physical period. */
VERNIER_API VernierDetector * vernierDetectorNewPeriodic(double physicalPeriod);

/** Creates a detector of megarena patterns of the given physical period and
 * code size (in bits). */
VERNIER_API VernierDetector * vernierDetectorNewMegarena(double physicalPeriod, int codeSize);

/** Creates a detector of bitmap patterns of the given physical period, the
 * bitmap being read from a PNG file (white dots on a black background). */
VERNIER_API VernierDetector * vernierDetectorNewBitmap(double physicalPeriod, const char * filename);

/** Creates a detector of stamps of the given physical period. The stamp bitmap
 * is read from a PNG file and must be square with an odd number of dots.
 * snapshotSize is the largest diagonal of a stamp in the image, in pixels. */
VERNIER_API VernierDetector * vernierDetectorNewStamp(double physicalPeriod, const char * filename,
        int snapshotSize);

/** Creates a detector of HP codes of the given physical period. numberHalfPeriods
 * is the size of the code in half periods along one direction (4k + 1, e.g. 33
 * or 37) and snapshotSize the largest size of a code in the image, in pixels. */
VERNIER_API VernierDetector * vernierDetectorNewHPCode(double physicalPeriod, int numberHalfPeriods,
        int snapshotSize);

/** Releases a detector. Freeing a null handle is a no-op. */
VERNIER_API void vernierDetectorFree(VernierDetector * detector);

/** Detects the patterns in an image of rows * cols doubles in [0, 1], stored
 * column by column, and estimates their poses. */
VERNIER_API VernierStatus vernierDetectorCompute(VernierDetector * detector, const double * image,
        int rows, int cols);

/** Sets found to non-zero if patterns have been detected and localized. Pass
 * -1 as id for the first pattern. */
VERNIER_API VernierStatus vernierDetectorPatternFound(VernierDetector * detector, int id, int * found);

/** Returns the number of detected patterns. */
VERNIER_API VernierStatus vernierDetectorPatternCount(VernierDetector * detector, int * count);

/** Returns the 2D pose (x, y, alpha) of a pattern. Pass -1 as id for the first
 * one. */
VERNIER_API VernierStatus vernierDetectorGet2DPose(VernierDetector * detector, int id, VernierPose * pose);

/** Returns the most likely 3D pose of a pattern. */
VERNIER_API VernierStatus vernierDetectorGet3DPose(VernierDetector * detector, int id, VernierPose * pose);

/** Returns the possible 3D poses of a pattern in case of ambiguities. Writes at
 * most capacity poses and always sets count to the number available, so passing
 * a null array and a capacity of zero queries that number. */
VERNIER_API VernierStatus vernierDetectorGetAll3DPoses(VernierDetector * detector, int id,
        VernierPose * poses, int capacity, int * count);

/** Returns the ids of the markers found by a stamp or HP code detector, in
 * increasing order, to be passed as id to vernierDetectorPatternFound() and
 * vernierDetectorGet2DPose(). Writes at most capacity ids and always sets count
 * to the number available, so passing a null array and a capacity of zero
 * queries that number. */
VERNIER_API VernierStatus vernierDetectorGetMarkerIds(VernierDetector * detector, int * ids, int capacity,
        int * count);

/** Adds a bitmap to a stamp detector, read from a PNG file of the same size as
 * the first one, so that several kinds of stamps are recognised. */
VERNIER_API VernierStatus vernierDetectorAddStampBitmap(VernierDetector * detector, const char * filename);

/** Returns the 3D pose of a pattern assuming a perspective projection of the
 * given focal length (in pixels). Periodic pattern detectors only. */
VERNIER_API VernierStatus vernierDetectorGet3DPosePerspective(VernierDetector * detector,
        double focalLength, VernierPose * pose);

/** Selects the compute backend. Fails and keeps the current backend if the
 * requested one is not available. */
VERNIER_API VernierStatus vernierDetectorSetBackend(VernierDetector * detector, VernierBackend backend);

/** Returns the compute backend in use. */
VERNIER_API VernierStatus vernierDetectorGetBackend(VernierDetector * detector, VernierBackend * backend);

/** Sets the physical period of the pattern. Periodic pattern detectors only,
 * as are the other parameters below. */
VERNIER_API VernierStatus vernierDetectorSetPhysicalPeriod(VernierDetector * detector, double physicalPeriod);

/** Returns the physical period of the pattern. */
VERNIER_API VernierStatus vernierDetectorGetPhysicalPeriod(VernierDetector * detector, double * physicalPeriod);

/** Sets the standard deviation of the spectral filter. */
VERNIER_API VernierStatus vernierDetectorSetSigma(VernierDetector * detector, double sigma);

/** Sets the fraction of the phase map used for the plane regression. */
VERNIER_API VernierStatus vernierDetectorSetCropFactor(VernierDetector * detector, double cropFactor);

/** Sets the highest frequency considered in the spectrum. */
VERNIER_API VernierStatus vernierDetectorSetMaxFrequency(VernierDetector * detector, double maxFrequency);

/** Sets the lowest frequency considered in the spectrum. */
VERNIER_API VernierStatus vernierDetectorSetMinFrequency(VernierDetector * detector, double minFrequency);

/** Sets the minimum power of a spectrum peak. */
VERNIER_API VernierStatus vernierDetectorSetMinPeakPower(VernierDetector * detector, double minPeakPower);

/** Sets the size of the smoothing kernel. */
VERNIER_API VernierStatus vernierDetectorSetSmoothingKernelSize(VernierDetector * detector, int smoothingKernelSize);

/** Writes the kind of pattern detected, e.g. "PeriodicPattern". */
VERNIER_API VernierStatus vernierDetectorGetClassname(VernierDetector * detector, char * buffer, size_t size);

/** Writes a description of the detector. */
VERNIER_API VernierStatus vernierDetectorToString(VernierDetector * detector, char * buffer, size_t size);

/** Writes the description of the pattern, as read from its JSON file. */
VERNIER_API VernierStatus vernierDetectorGetDescription(VernierDetector * detector, char * buffer, size_t size);

/** Writes the author of the pattern, as read from its JSON file. */
VERNIER_API VernierStatus vernierDetectorGetAuthor(VernierDetector * detector, char * buffer, size_t size);

/** Writes the date of the pattern, as read from its JSON file. */
VERNIER_API VernierStatus vernierDetectorGetDate(VernierDetector * detector, char * buffer, size_t size);

/** Writes the length unit of the pattern, as read from its JSON file. */
VERNIER_API VernierStatus vernierDetectorGetUnit(VernierDetector * detector, char * buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
