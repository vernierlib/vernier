#include "Vernier.hpp"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

using namespace vernier;
using namespace std;

/* Renders a periodic pattern of the given pixel period, turned slightly off
 * axis so the detector has a real angle to recover rather than a degenerate one. */
static Eigen::ArrayXXd syntheticImage(int size, double period) {
    double theta  = 0.1;
    double cx = size / 2.0, cy = size / 2.0;
    double c = cos(-theta), s = sin(-theta);
    Eigen::ArrayXXd img(size, size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            double dx = col - cx, dy = row - cy;
            double xp = c * dx - s * dy;
            img(row, col) = 0.5 + 0.5 * cos(2.0 * PI * xp / period);
        }
    }
    return img;
}

int main(int argc, char* argv[]) {
    int size = 512, iters = 20;
    string backendName = "cpu";
    for (int i = 1; i + 1 < argc; i++) {
        if (string(argv[i]) == "--size")    size        = stoi(argv[i + 1]);
        if (string(argv[i]) == "--iters")   iters       = stoi(argv[i + 1]);
        if (string(argv[i]) == "--backend") backendName = argv[i + 1];
    }

    Backend backend;
    if (backendName == "cpu") {
        backend = Backend::CPU;
    } else if (backendName == "cuda") {
        backend = Backend::CUDA;
    } else {
        cerr << "unknown backend '" << backendName << "', expected cpu or cuda" << endl;
        return 1;
    }

    double period = size / 16.0;
    Eigen::ArrayXXd image = syntheticImage(size, period);

    /* This is the whole of the public API a caller needs: ask the factory for a
     * detector on the backend you want, and work through the base interface. */
    unique_ptr<PatternDetector> detector;
    try {
        detector = Detector::newInstance("PeriodicPattern", backend);
    } catch (const exception& e) {
        cerr << backendName << " backend unavailable: " << e.what() << endl;
        return 1;
    }
    detector->setDouble("physicalPeriod", period);

    /* Warm-up, so the FFT plans and any device buffers are built before timing
     * rather than landing on the first measured iteration. */
    detector->compute(image);
    if (!detector->patternFound()) {
        cerr << "the pattern was not detected, so these timings would mean nothing" << endl;
        return 1;
    }

    /* Times a whole detection, not just the phase computation: this is the work
     * a caller actually asks for. */
    double best = numeric_limits<double>::infinity(), total = 0.0;
    for (int i = 0; i < iters; i++) {
        auto t0 = chrono::high_resolution_clock::now();
        detector->compute(image);
        double ms = chrono::duration<double, milli>(
            chrono::high_resolution_clock::now() - t0).count();
        total += ms;
        if (ms < best) best = ms;
    }

    cout << "backend=cpp-" << (detector->getBackend() == Backend::CUDA ? "cuda" : "cpu")
         << " size=" << size << "x" << size
         << " iters=" << iters
         << fixed << setprecision(2)
         << " mean=" << total / iters << "ms"
         << " best=" << best << "ms" << endl;
    cout << "pose: " << detector->get2DPose().toString() << endl;
    return 0;
}
