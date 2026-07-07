#include "Vernier.hpp"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

using namespace vernier;
using namespace std;

static Eigen::ArrayXXd syntheticImage(int size) {
    double period = size / 16.0;
    double theta  = 0.1;
    double cx = size / 2.0, cy = size / 2.0;
    double c = cos(-theta), s = sin(-theta);
    Eigen::ArrayXXd img(size, size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            double dx = col - cx, dy = row - cy;
            double xp = c * dx - s * dy;
            img(row, col) = 0.5 + 0.5 * cos(2.0 * M_PI * xp / period);
        }
    }
    return img;
}

int main(int argc, char* argv[]) {
    int size = 512, iters = 20;
    string backend = "cpu";
    for (int i = 1; i + 1 < argc; i++) {
        if (string(argv[i]) == "--size")    size    = stoi(argv[i + 1]);
        if (string(argv[i]) == "--iters")   iters   = stoi(argv[i + 1]);
        if (string(argv[i]) == "--backend") backend = argv[i + 1];
    }

    Eigen::ArrayXXd image = syntheticImage(size);
    PatternPhase patternPhase(size, size);

    if (backend == "cuda") {
        try {
            patternPhase.setBackend(Backend::CUDA);
        } catch (const std::exception& e) {
            cerr << "cuda backend unavailable: " << e.what() << endl;
            return 1;
        }
    }

    // Warm-up: prime FFT plans / device buffers.
    patternPhase.compute(image);

    double best = numeric_limits<double>::infinity(), total = 0.0;
    for (int i = 0; i < iters; i++) {
        auto t0 = chrono::high_resolution_clock::now();
        patternPhase.compute(image);
        double ms = chrono::duration<double, milli>(
            chrono::high_resolution_clock::now() - t0).count();
        total += ms;
        if (ms < best) best = ms;
    }

    cout << "backend=cpp-" << backend
         << " size=" << size << "x" << size
         << " iters=" << iters
         << fixed << setprecision(2)
         << " mean=" << total / iters << "ms"
         << " best=" << best << "ms" << endl;
    return 0;
}
