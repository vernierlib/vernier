/*
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.
 */

/** \file test_vernier_c.c
 *
 *  Tests of the C API. Compiled as C rather than C++ on purpose: it checks that
 *  vernier.h really is usable from C, which is the whole point of the API.
 *
 *  The images come from the PGM files of the data directory, rendered from the
 *  C++ layouts at known poses (see data/README.md): the C API only exposes the
 *  detectors, so the tests cannot render their own images.
 */

#include "vernier.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(int condition, const char * message) {
    if (!condition) {
        printf("  FAILED: %s (last error: %s)\n", message, vernierLastError());
        failures++;
    }
}

/** Reads a binary PGM into doubles in [0, 1], stored column by column as the
 * API expects. The file stores its pixels row by row, so this transposes. */
static double * readPgm(const char * filename, int * rows, int * cols) {
    FILE * file = fopen(filename, "rb");
    int width = 0;
    int height = 0;
    int maximum = 0;
    double * image = NULL;
    int row;
    int col;

    if (file == NULL) {
        printf("  FAILED: cannot open %s\n", filename);
        failures++;
        return NULL;
    }
    if (fscanf(file, "P5 %d %d %d", &width, &height, &maximum) != 3 || maximum != 255) {
        printf("  FAILED: %s is not a binary PGM of 8-bit pixels\n", filename);
        failures++;
        fclose(file);
        return NULL;
    }
    fgetc(file); /* the single whitespace before the pixels */

    *rows = height;
    *cols = width;
    image = malloc((size_t) width * height * sizeof (double));
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            image[row + col * height] = (double) fgetc(file) / 255.0;
        }
    }
    fclose(file);
    return image;
}

static VernierPose pose2D(double x, double y, double alpha, double pixelSize) {
    VernierPose pose;
    memset(&pose, 0, sizeof (pose));
    pose.x = x;
    pose.y = y;
    pose.alpha = alpha;
    pose.pixelSize = pixelSize;
    pose.is3D = 0;
    return pose;
}

static void checkPoseNear(VernierPose estimated, VernierPose truth, double tolerance, const char * message) {
    check(fabs(estimated.x - truth.x) < tolerance
            && fabs(estimated.y - truth.y) < tolerance
            && fabs(estimated.alpha - truth.alpha) < tolerance, message);
}

static void testVersionAndBackend(void) {
    int available = vernierCudaAvailable();
    check(strlen(vernierVersion()) > 0, "the version is not empty");
    check(available == 0 || available == 1, "cudaAvailable returns a boolean");
}

static void testPose(void) {
    VernierPose pose = pose2D(6.0, 3.0, 0.2, 2.0);
    char buffer[256];
    double cTp[16];
    double pTc[16];
    double identity;
    int row;
    int col;
    int k;

    check(vernierPoseToString(&pose, buffer, sizeof (buffer)) == VERNIER_OK, "the pose is described");
    check(strstr(buffer, "x=6") != NULL, "the description contains the position");
    check(vernierPoseToString(&pose, buffer, 4) == VERNIER_INVALID_ARGUMENT, "a short buffer is reported");
    check(strlen(buffer) == 3, "a short buffer is still null-terminated");
    check(vernierPoseToString(NULL, buffer, sizeof (buffer)) == VERNIER_INVALID_ARGUMENT, "a null pose is reported");

    check(vernierPoseCameraToPatternMatrix(&pose, cTp) == VERNIER_OK, "the camera-to-pattern matrix is returned");
    check(vernierPosePatternToCameraMatrix(&pose, pTc) == VERNIER_OK, "the pattern-to-camera matrix is returned");
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            identity = 0.0;
            for (k = 0; k < 4; k++) {
                identity += cTp[row + 4 * k] * pTc[k + 4 * col];
            }
            check(fabs(identity - (row == col ? 1.0 : 0.0)) < 1e-12, "the two matrices are inverse of each other");
        }
    }
}

static void testPeriodicPattern(void) {
    VernierPose truth = pose2D(6.0, 3.0, 0.2, 2.0);
    VernierPose estimated;
    VernierBackend backend;
    VernierDetector * detector;
    int rows = 0;
    int cols = 0;
    double * image = readPgm("data/periodicPattern.pgm", &rows, &cols);
    int found = 0;
    int count = 0;

    if (image == NULL) {
        return;
    }
    check(rows == 512 && cols == 512, "the fixture is 512 by 512");

    detector = vernierDetectorNewPeriodic(15.0);
    check(detector != NULL, "the periodic detector is created");
    check(vernierDetectorSetSigma(detector, 1.0) == VERNIER_OK, "sigma is set");
    check(vernierDetectorSetCropFactor(detector, 0.4) == VERNIER_OK, "the crop factor is set");
    check(vernierDetectorGetBackend(detector, &backend) == VERNIER_OK && backend == VERNIER_BACKEND_CPU,
            "the default backend is the CPU");
    check(vernierDetectorCompute(detector, image, rows, cols) == VERNIER_OK, "the image is analysed");
    check(vernierDetectorPatternFound(detector, -1, &found) == VERNIER_OK && found, "the pattern is found");
    check(vernierDetectorPatternCount(detector, &count) == VERNIER_OK && count == 1, "one pattern is counted");
    check(vernierDetectorGet2DPose(detector, -1, &estimated) == VERNIER_OK, "the pose is estimated");
    checkPoseNear(estimated, truth, 0.05, "the estimated pose matches the rendered one");

    vernierDetectorFree(detector);
    free(image);
}

static void testNonSquareImage(void) {
    /* a caller mixing up rows and columns would get a wrong pose */
    VernierPose truth = pose2D(1.5, -2.5, 0.4, 1.2);
    VernierPose estimated;
    VernierDetector * detector;
    int rows = 0;
    int cols = 0;
    double * image = readPgm("data/nonSquarePattern.pgm", &rows, &cols);
    int found = 0;

    if (image == NULL) {
        return;
    }
    check(rows == 300 && cols == 420, "the fixture is 300 by 420");

    detector = vernierDetectorNewPeriodic(8.0);
    check(detector != NULL, "the periodic detector is created");
    check(vernierDetectorCompute(detector, image, rows, cols) == VERNIER_OK, "the image is analysed");
    check(vernierDetectorPatternFound(detector, -1, &found) == VERNIER_OK && found, "the pattern is found");
    check(vernierDetectorGet2DPose(detector, -1, &estimated) == VERNIER_OK, "the pose is estimated");
    checkPoseNear(estimated, truth, 0.05, "the estimated pose matches the rendered one");

    vernierDetectorFree(detector);
    free(image);
}

static void testMegarenaPattern(void) {
    VernierPose truth = pose2D(-15.0 * 400.0, -15.0 * 200.0, 0.2, 1.0);
    VernierPose estimated;
    VernierDetector * detector;
    int rows = 0;
    int cols = 0;
    double * image = readPgm("data/megarenaPattern.pgm", &rows, &cols);
    int found = 0;

    if (image == NULL) {
        return;
    }
    detector = vernierDetectorNewMegarena(10.0, 12);
    check(detector != NULL, "the megarena detector is created");
    check(vernierDetectorCompute(detector, image, rows, cols) == VERNIER_OK, "the image is analysed");
    check(vernierDetectorPatternFound(detector, -1, &found) == VERNIER_OK && found, "the pattern is found");
    check(vernierDetectorGet2DPose(detector, -1, &estimated) == VERNIER_OK, "the absolute pose is estimated");
    checkPoseNear(estimated, truth, 0.01, "the estimated absolute pose matches the rendered one");

    vernierDetectorFree(detector);
    free(image);
}

static void testFactories(void) {
    VernierDetector * detector;
    char buffer[256];
    double period = 0.0;
    const char * filename = "testPattern.json";
    FILE * file;

    detector = vernierDetectorNew("StampPattern", VERNIER_BACKEND_CPU);
    check(detector != NULL, "a detector is created from its class name");
    check(vernierDetectorGetClassname(detector, buffer, sizeof (buffer)) == VERNIER_OK
            && strcmp(buffer, "StampPattern") == 0, "the class name is read back");
    check(vernierDetectorToString(detector, buffer, sizeof (buffer)) == VERNIER_OK && strlen(buffer) > 0,
            "the detector is described");
    vernierDetectorFree(detector);

    /* the megarena detector reads the code size from "codeSize" */
    file = fopen(filename, "w");
    fprintf(file, "{ \"MegarenaPattern\": { \"period\": 14.0, \"codeSize\": 12,"
            " \"description\": \"test pattern\", \"unit\": \"um\" } }");
    fclose(file);

    detector = vernierDetectorLoadFromJSON(filename, VERNIER_BACKEND_CPU);
    check(detector != NULL, "a detector is loaded from a JSON file");
    check(vernierDetectorGetClassname(detector, buffer, sizeof (buffer)) == VERNIER_OK
            && strcmp(buffer, "MegarenaPattern") == 0, "the class name is read back");
    check(vernierDetectorGetPhysicalPeriod(detector, &period) == VERNIER_OK && period == 14.0,
            "the period is read back");
    check(vernierDetectorGetDescription(detector, buffer, sizeof (buffer)) == VERNIER_OK
            && strcmp(buffer, "test pattern") == 0, "the description is read back");
    check(vernierDetectorGetUnit(detector, buffer, sizeof (buffer)) == VERNIER_OK
            && strcmp(buffer, "um") == 0, "the unit is read back");
    vernierDetectorFree(detector);
    remove(filename);
}

static void testErrors(void) {
    VernierDetector * detector;
    double image[4] = {0.0, 0.0, 0.0, 0.0};
    char buffer[8];

    check(vernierDetectorNew("NoSuchPattern", VERNIER_BACKEND_CPU) == NULL, "an unknown class name fails");
    check(strlen(vernierLastError()) > 0, "the failure is described");
    check(vernierDetectorNew(NULL, VERNIER_BACKEND_CPU) == NULL, "a null class name is reported");
    check(vernierDetectorLoadFromJSON("no_such_file.json", VERNIER_BACKEND_CPU) == NULL, "a missing file fails");

    detector = vernierDetectorNewPeriodic(15.0);
    check(detector != NULL, "the periodic detector is created");
    check(vernierDetectorCompute(detector, NULL, 512, 512) == VERNIER_ERROR, "a null image is reported");
    check(vernierDetectorCompute(detector, image, 0, 0) == VERNIER_ERROR, "an empty image is reported");
    check(vernierDetectorGetClassname(detector, buffer, sizeof (buffer)) == VERNIER_INVALID_ARGUMENT,
            "a short buffer is reported");
    check(strcmp(buffer, "Periodi") == 0, "a short buffer holds the truncated name");
    check(vernierDetectorPatternCount(detector, NULL) == VERNIER_INVALID_ARGUMENT, "a null result is reported");
    check(vernierDetectorPatternCount(NULL, NULL) == VERNIER_INVALID_ARGUMENT, "a null detector is reported");
    if (!vernierCudaAvailable()) {
        VernierBackend backend;
        check(vernierDetectorSetBackend(detector, VERNIER_BACKEND_CUDA) == VERNIER_ERROR,
                "an unavailable backend fails");
        check(vernierDetectorGetBackend(detector, &backend) == VERNIER_OK && backend == VERNIER_BACKEND_CPU,
                "a rejected backend leaves the current one in place");
    }
    vernierDetectorFree(detector);
    vernierDetectorFree(NULL);
}

int main(void) {
    struct {
        const char * name;
        void (*run)(void);
    } tests[] = {
        {"testVersionAndBackend", testVersionAndBackend},
        {"testPose", testPose},
        {"testPeriodicPattern", testPeriodicPattern},
        {"testNonSquareImage", testNonSquareImage},
        {"testMegarenaPattern", testMegarenaPattern},
        {"testFactories", testFactories},
        {"testErrors", testErrors}
    };
    int count = (int) (sizeof (tests) / sizeof (tests[0]));
    int i;

    for (i = 0; i < count; i++) {
        printf("%s... ", tests[i].name);
        fflush(stdout);
        tests[i].run();
        printf("%s\n", failures == 0 ? "ok" : "see failures above");
    }
    if (failures == 0) {
        printf("All %d tests passed.\n", count);
        return 0;
    }
    printf("%d check(s) failed.\n", failures);
    return 1;
}
