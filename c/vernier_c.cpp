/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

/** \file vernier_c.cpp
 *
 *  Implementation of the C API declared in vernier.h.
 *
 *  Each function turns the C arguments into their C++ counterparts, runs the
 *  call inside a guard that converts exceptions into status codes, and stores
 *  the message of the last failure for vernierLastError().
 */

#include "vernier.h"
#include "Vernier.hpp"

#include <cstring>
#include <map>
#include <string>

using namespace vernier;

/** A detector handle owns the C++ detector it wraps. */
struct VernierDetector {
    std::unique_ptr<PatternDetector> detector;
};

namespace {

    // Per thread, so that two threads calling the API do not overwrite each
    // other's message.
    thread_local std::string lastError;

    VernierStatus fail(VernierStatus status, const std::string & message) {
        lastError = message;
        return status;
    }

    /** Runs a call, turning any exception into a status code. */
    template <typename Body>
    VernierStatus guard(Body body) {
        try {
            body();
        } catch (const std::exception & e) {
            return fail(VERNIER_ERROR, e.what());
        } catch (...) {
            return fail(VERNIER_ERROR, "unknown C++ exception.");
        }
        lastError.clear();
        return VERNIER_OK;
    }

    VernierStatus copyString(const std::string & value, char * buffer, size_t size) {
        if (buffer == nullptr || size == 0) {
            return fail(VERNIER_INVALID_ARGUMENT, "the string buffer must not be null or empty.");
        }
        size_t copied = value.size() < size - 1 ? value.size() : size - 1;
        std::memcpy(buffer, value.data(), copied);
        buffer[copied] = '\0';
        if (copied < value.size()) {
            return fail(VERNIER_INVALID_ARGUMENT, "the string buffer is too small for " + value + ".");
        }
        lastError.clear();
        return VERNIER_OK;
    }

    Pose toPose(const VernierPose * pose) {
        Pose result(pose->x, pose->y, pose->z, pose->alpha, pose->beta, pose->gamma, pose->pixelSize);
        result.is3D = (pose->is3D != 0);
        return result;
    }

    void fromPose(const Pose & pose, VernierPose * result) {
        result->x = pose.x;
        result->y = pose.y;
        result->z = pose.z;
        result->alpha = pose.alpha;
        result->beta = pose.beta;
        result->gamma = pose.gamma;
        result->pixelSize = pose.pixelSize;
        result->is3D = pose.is3D ? 1 : 0;
    }

    Backend toBackend(VernierBackend backend) {
        return backend == VERNIER_BACKEND_CUDA ? Backend::CUDA : Backend::CPU;
    }

    /** The periodic detector behind a handle, for the parameters only it has. */
    PeriodicPatternDetector & asPeriodic(VernierDetector * detector) {
        PeriodicPatternDetector * periodic = dynamic_cast<PeriodicPatternDetector *> (detector->detector.get());
        if (periodic == nullptr) {
            throw Exception("this function is not available for this kind of detector.");
        }
        return *periodic;
    }

    /** Sets a parameter of a periodic detector, the shape most of them have. */
    VernierStatus setPeriodicDouble(VernierDetector * detector, double value,
            void (PeriodicPatternDetector::*setter)(double)) {
        if (detector == nullptr) {
            return fail(VERNIER_INVALID_ARGUMENT, "the detector must not be null.");
        }
        return guard([&] {
            (asPeriodic(detector).*setter)(value);
        });
    }

    /** Writes one of the descriptions of a detector into the caller's buffer. */
    VernierStatus detectorString(VernierDetector * detector, char * buffer, size_t size,
            std::string(PatternDetector::*getter)()) {
        if (detector == nullptr) {
            return fail(VERNIER_INVALID_ARGUMENT, "the detector must not be null.");
        }
        std::string value;
        VernierStatus status = guard([&] {
            value = (detector->detector.get()->*getter)();
        });
        return status == VERNIER_OK ? copyString(value, buffer, size) : status;
    }
}

/* ─── Library ─────────────────────────────────────────────────────────────── */

const char * vernierLastError(void) {
    return lastError.c_str();
}

const char * vernierVersion(void) {
    return VERNIER_VERSION;
}

int vernierCudaAvailable(void) {
    return cudaAvailable() ? 1 : 0;
}

/* ─── Poses ───────────────────────────────────────────────────────────────── */

VernierStatus vernierPoseToString(const VernierPose * pose, char * buffer, size_t size) {
    if (pose == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the pose must not be null.");
    }
    std::string description;
    VernierStatus status = guard([&] {
        description = toPose(pose).toString();
    });
    return status == VERNIER_OK ? copyString(description, buffer, size) : status;
}

namespace {

    VernierStatus poseMatrix(const VernierPose * pose, double * matrix, bool cameraToPattern) {
        if (pose == nullptr || matrix == nullptr) {
            return fail(VERNIER_INVALID_ARGUMENT, "the pose and the matrix must not be null.");
        }
        return guard([&] {
            Pose converted = toPose(pose);
            Eigen::Matrix4d transformation = cameraToPattern
                    ? converted.getCameraToPatternTransformationMatrix()
                    : converted.getPatternToCameraTransformationMatrix();
            // named, as Map<Matrix4d>(matrix) = ... would declare a variable
            Eigen::Map<Eigen::Matrix4d> destination(matrix);
            destination = transformation;
        });
    }
}

VernierStatus vernierPoseCameraToPatternMatrix(const VernierPose * pose, double * matrix) {
    return poseMatrix(pose, matrix, true);
}

VernierStatus vernierPosePatternToCameraMatrix(const VernierPose * pose, double * matrix) {
    return poseMatrix(pose, matrix, false);
}

/* ─── Detectors ───────────────────────────────────────────────────────────── */

namespace {

    /** Builds a handle around a detector, or returns null and keeps the reason. */
    template <typename Body>
    VernierDetector * create(Body body) {
        VernierDetector * created = nullptr;
        VernierStatus status = guard([&] {
            std::unique_ptr<VernierDetector> handle(new VernierDetector());
            handle->detector = body();
            created = handle.release();
        });
        return status == VERNIER_OK ? created : nullptr;
    }
}

VernierDetector * vernierDetectorNew(const char * classname, VernierBackend backend) {
    if (classname == nullptr) {
        fail(VERNIER_INVALID_ARGUMENT, "the class name must not be null.");
        return nullptr;
    }
    return create([&] {
        return Detector::newInstance(classname, toBackend(backend));
    });
}

VernierDetector * vernierDetectorLoadFromJSON(const char * filename, VernierBackend backend) {
    if (filename == nullptr) {
        fail(VERNIER_INVALID_ARGUMENT, "the file name must not be null.");
        return nullptr;
    }
    return create([&] {
        return Detector::loadFromJSON(filename, toBackend(backend));
    });
}

VernierDetector * vernierDetectorNewPeriodic(double physicalPeriod) {
    return create([&] {
        return std::unique_ptr<PatternDetector>(new PeriodicPatternDetector(physicalPeriod));
    });
}

VernierDetector * vernierDetectorNewMegarena(double physicalPeriod, int codeSize) {
    return create([&] {
        return std::unique_ptr<PatternDetector>(new MegarenaPatternDetector(physicalPeriod, codeSize));
    });
}

VernierDetector * vernierDetectorNewBitmap(double physicalPeriod, const char * filename) {
    if (filename == nullptr) {
        fail(VERNIER_INVALID_ARGUMENT, "the bitmap file name must not be null.");
        return nullptr;
    }
    return create([&] {
        return std::unique_ptr<PatternDetector>(new BitmapPatternDetector(physicalPeriod, filename));
    });
}

VernierDetector * vernierDetectorNewStamp(double physicalPeriod, const char * filename, int snapshotSize) {
    if (filename == nullptr) {
        fail(VERNIER_INVALID_ARGUMENT, "the bitmap file name must not be null.");
        return nullptr;
    }
    if (snapshotSize <= 0) {
        fail(VERNIER_INVALID_ARGUMENT, "the snapshot size must be positive.");
        return nullptr;
    }
    return create([&] {
        return std::unique_ptr<PatternDetector>(new StampPatternDetector(physicalPeriod, filename, snapshotSize));
    });
}

VernierDetector * vernierDetectorNewHPCode(double physicalPeriod, int numberHalfPeriods, int snapshotSize) {
    if (snapshotSize <= 0) {
        fail(VERNIER_INVALID_ARGUMENT, "the snapshot size must be positive.");
        return nullptr;
    }
    return create([&] {
        return std::unique_ptr<PatternDetector>(
                new HPCodePatternDetector(physicalPeriod, numberHalfPeriods, snapshotSize));
    });
}

void vernierDetectorFree(VernierDetector * detector) {
    delete detector;
}

VernierStatus vernierDetectorCompute(VernierDetector * detector, const double * image, int rows, int cols) {
    if (detector == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector must not be null.");
    }
    return guard([&] {
        if (image == nullptr) {
            throw Exception("the image must not be null.");
        }
        if (rows <= 0 || cols <= 0) {
            throw Exception("the image must have at least one row and one column.");
        }
        detector->detector->compute(Eigen::Map<const Eigen::ArrayXXd>(image, rows, cols));
    });
}

VernierStatus vernierDetectorPatternFound(VernierDetector * detector, int id, int * found) {
    if (detector == nullptr || found == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the result must not be null.");
    }
    return guard([&] {
        *found = detector->detector->patternFound(id) ? 1 : 0;
    });
}

VernierStatus vernierDetectorPatternCount(VernierDetector * detector, int * count) {
    if (detector == nullptr || count == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the result must not be null.");
    }
    return guard([&] {
        *count = detector->detector->patternCount();
    });
}

VernierStatus vernierDetectorGet2DPose(VernierDetector * detector, int id, VernierPose * pose) {
    if (detector == nullptr || pose == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the pose must not be null.");
    }
    return guard([&] {
        fromPose(detector->detector->get2DPose(id), pose);
    });
}

VernierStatus vernierDetectorGet3DPose(VernierDetector * detector, int id, VernierPose * pose) {
    if (detector == nullptr || pose == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the pose must not be null.");
    }
    return guard([&] {
        fromPose(detector->detector->get3DPose(id), pose);
    });
}

VernierStatus vernierDetectorGetAll3DPoses(VernierDetector * detector, int id, VernierPose * poses,
        int capacity, int * count) {
    if (detector == nullptr || count == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the count must not be null.");
    }
    if (capacity < 0 || (capacity > 0 && poses == nullptr)) {
        return fail(VERNIER_INVALID_ARGUMENT, "the capacity must be positive and the poses must not be null.");
    }
    return guard([&] {
        std::vector<Pose> found = detector->detector->getAll3DPoses(id);
        *count = (int) found.size();
        int copied = (int) found.size() < capacity ? (int) found.size() : capacity;
        for (int i = 0; i < copied; i++) {
            fromPose(found[i], &poses[i]);
        }
    });
}

namespace {

    /** The markers of a stamp or HP code detector, the two kinds that have some. */
    const std::map<int, Pose> & markersOf(VernierDetector * detector) {
        PatternDetector * base = detector->detector.get();
        if (StampPatternDetector * stamp = dynamic_cast<StampPatternDetector *> (base)) {
            return stamp->markers;
        }
        if (HPCodePatternDetector * code = dynamic_cast<HPCodePatternDetector *> (base)) {
            return code->markers;
        }
        throw Exception("only stamp and HP code detectors have markers.");
    }
}

VernierStatus vernierDetectorGetMarkerIds(VernierDetector * detector, int * ids, int capacity, int * count) {
    if (detector == nullptr || count == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the count must not be null.");
    }
    if (capacity < 0 || (capacity > 0 && ids == nullptr)) {
        return fail(VERNIER_INVALID_ARGUMENT, "the capacity must be positive and the ids must not be null.");
    }
    return guard([&] {
        const std::map<int, Pose> & markers = markersOf(detector);
        *count = (int) markers.size();
        int copied = 0;
        for (std::map<int, Pose>::const_iterator it = markers.begin(); it != markers.end() && copied < capacity; ++it) {
            ids[copied++] = it->first;
        }
    });
}

VernierStatus vernierDetectorAddStampBitmap(VernierDetector * detector, const char * filename) {
    if (detector == nullptr || filename == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the file name must not be null.");
    }
    return guard([&] {
        StampPatternDetector * stamp = dynamic_cast<StampPatternDetector *> (detector->detector.get());
        if (stamp == nullptr) {
            throw Exception("only stamp detectors accept additional bitmaps.");
        }
        stamp->addBitmap(filename);
    });
}

VernierStatus vernierDetectorGet3DPosePerspective(VernierDetector * detector, double focalLength, VernierPose * pose) {
    if (detector == nullptr || pose == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the pose must not be null.");
    }
    return guard([&] {
        fromPose(asPeriodic(detector).get3DPosePerspective(focalLength), pose);
    });
}

VernierStatus vernierDetectorSetBackend(VernierDetector * detector, VernierBackend backend) {
    if (detector == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector must not be null.");
    }
    return guard([&] {
        detector->detector->setBackend(toBackend(backend));
    });
}

VernierStatus vernierDetectorGetBackend(VernierDetector * detector, VernierBackend * backend) {
    if (detector == nullptr || backend == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the backend must not be null.");
    }
    return guard([&] {
        *backend = detector->detector->getBackend() == Backend::CUDA ? VERNIER_BACKEND_CUDA : VERNIER_BACKEND_CPU;
    });
}

VernierStatus vernierDetectorSetPhysicalPeriod(VernierDetector * detector, double physicalPeriod) {
    return setPeriodicDouble(detector, physicalPeriod, &PeriodicPatternDetector::setPhysicalPeriod);
}

VernierStatus vernierDetectorGetPhysicalPeriod(VernierDetector * detector, double * physicalPeriod) {
    if (detector == nullptr || physicalPeriod == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector and the period must not be null.");
    }
    return guard([&] {
        *physicalPeriod = asPeriodic(detector).getPhysicalPeriod();
    });
}

VernierStatus vernierDetectorSetSigma(VernierDetector * detector, double sigma) {
    return setPeriodicDouble(detector, sigma, &PeriodicPatternDetector::setSigma);
}

VernierStatus vernierDetectorSetCropFactor(VernierDetector * detector, double cropFactor) {
    return setPeriodicDouble(detector, cropFactor, &PeriodicPatternDetector::setCropFactor);
}

VernierStatus vernierDetectorSetMaxFrequency(VernierDetector * detector, double maxFrequency) {
    return setPeriodicDouble(detector, maxFrequency, &PeriodicPatternDetector::setMaxFrequency);
}

VernierStatus vernierDetectorSetMinFrequency(VernierDetector * detector, double minFrequency) {
    return setPeriodicDouble(detector, minFrequency, &PeriodicPatternDetector::setMinFrequency);
}

VernierStatus vernierDetectorSetMinPeakPower(VernierDetector * detector, double minPeakPower) {
    return setPeriodicDouble(detector, minPeakPower, &PeriodicPatternDetector::setMinPeakPower);
}

VernierStatus vernierDetectorSetSmoothingKernelSize(VernierDetector * detector, int smoothingKernelSize) {
    if (detector == nullptr) {
        return fail(VERNIER_INVALID_ARGUMENT, "the detector must not be null.");
    }
    return guard([&] {
        asPeriodic(detector).setSmoothingKernelSize(smoothingKernelSize);
    });
}

VernierStatus vernierDetectorGetClassname(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::getClassname);
}

VernierStatus vernierDetectorToString(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::toString);
}

VernierStatus vernierDetectorGetDescription(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::getDescription);
}

VernierStatus vernierDetectorGetAuthor(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::getAuthor);
}

VernierStatus vernierDetectorGetDate(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::getDate);
}

VernierStatus vernierDetectorGetUnit(VernierDetector * detector, char * buffer, size_t size) {
    return detectorString(detector, buffer, size, &PatternDetector::getUnit);
}
