/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Spatial.hpp"

namespace vernier {

    /** Unwraps one column of the phase map, upward then downward from the central
     *	row, starting from the phase iteration and phase value carried over by the
     *	unwrapping of the central row.
     *
     *	Every column is independent from the others, which is what makes the four
     *	quarters parallelizable.
     */
    static void unwrapColumn(Eigen::ArrayXXd& wrappedPhase, int col, int origineY, int sizeY,
            int phaseIteration, double phaseValue) {

        double phaseValuePrevY, difference;

        // Upper part of the column (quarter 3 on the left half, 4 on the right)
        int phaseIterationY = phaseIteration;
        double phaseValueNextY = phaseValue;
        for (int row = origineY; row > 0; row--) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row - 1, col);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row - 1, col) = phaseValueNextY + phaseIterationY * 2 * PI;
        }

        // Lower part of the column (quarter 2 on the left half, 1 on the right)
        phaseIterationY = phaseIteration;
        phaseValueNextY = phaseValue;
        for (int row = origineY; row < sizeY - 1; row++) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row + 1, col);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row + 1, col) = phaseValueNextY + phaseIterationY * 2 * PI;
        }
    }

    void quartersUnwrapPhase(Eigen::ArrayXXd& wrappedPhase) {
        int sizeX = wrappedPhase.cols();
        int sizeY = wrappedPhase.rows();
        int origineX = (sizeX / 2);
        int origineY = (sizeY / 2);

        // The central row is a prefix scan: it has to stay sequential. The seed
        // (phase iteration and phase value) handed over to each column is saved
        // along the way so that the columns can then be unwrapped in parallel.
        // Columns 0 to origineX are seeded by the left scan, columns origineX to
        // sizeX-1 by the right one (the central column gets both, in that order).
        std::vector<int> leftIteration(origineX + 1), rightIteration(sizeX - origineX);
        std::vector<double> leftValue(origineX + 1), rightValue(sizeX - origineX);

        double phaseValuePrevX, difference;

        // Left half of the central row
        int phaseIterationX = 0;
        double phaseValueNextX = wrappedPhase(origineY, origineX);
        for (int col = origineX; col > 0; col--) {
            phaseValuePrevX = phaseValueNextX;
            phaseValueNextX = wrappedPhase(origineY, col - 1);

            difference = phaseValueNextX - phaseValuePrevX;

            if (difference > PI) {
                phaseIterationX = phaseIterationX - 1;
            } else if (difference <= -PI) {
                phaseIterationX = phaseIterationX + 1;
            }
            wrappedPhase(origineY, col - 1) = phaseValueNextX + phaseIterationX * 2 * PI;

            leftIteration[col] = phaseIterationX;
            leftValue[col] = phaseValueNextX;
        }
        leftIteration[0] = phaseIterationX;
        leftValue[0] = phaseValueNextX;

        // Right half of the central row
        phaseIterationX = 0;
        phaseValueNextX = wrappedPhase(origineY, origineX);
        for (int col = origineX; col < sizeX - 1; col++) {
            phaseValuePrevX = phaseValueNextX;
            phaseValueNextX = wrappedPhase(origineY, col + 1);

            difference = phaseValueNextX - phaseValuePrevX;

            if (difference > PI) {
                phaseIterationX = phaseIterationX - 1;
            } else if (difference <= -PI) {
                phaseIterationX = phaseIterationX + 1;
            }
            wrappedPhase(origineY, col + 1) = phaseValueNextX + phaseIterationX * 2 * PI;

            rightIteration[col - origineX] = phaseIterationX;
            rightValue[col - origineX] = phaseValueNextX;
        }
        rightIteration[sizeX - 1 - origineX] = phaseIterationX;
        rightValue[sizeX - 1 - origineX] = phaseValueNextX;

        // The four quarters, one column at a time: no column reads or writes
        // another one, nor the central row.
#pragma omp parallel for
        for (int col = 0; col < sizeX; col++) {
            if (col <= origineX) {
                unwrapColumn(wrappedPhase, col, origineY, sizeY, leftIteration[col], leftValue[col]);
            }
            if (col >= origineX) {
                unwrapColumn(wrappedPhase, col, origineY, sizeY, rightIteration[col - origineX], rightValue[col - origineX]);
            }
        }
    }

    Eigen::ArrayXXd hannWindow(int size, int exposure) {
        ASSERT_MSG(size > 0, "The size of the window must be positive.")
        Eigen::ArrayXXd window(size, size);
        int radius = size / 2;
#pragma omp parallel for
        for (int col = -radius; col < radius; col++) {
            for (int row = -radius; row < radius; row++) {
                double distanceToCenter = sqrt(row * row + col * col);
                if (distanceToCenter < radius) {
                    window(radius + row, radius + col) = (1 + cos(PI * pow(distanceToCenter, exposure) / pow(radius, exposure))) / 2;
                } else {
                    window(radius + row, radius + col) = 0.0;
                }
            }
        }
        return window;
    };

    void takeSnapshot(int x, int y, int size, const Eigen::ArrayXXd & array, Eigen::ArrayXXd & snapshot) {
        snapshot.resize(size, size);
        int radius = size / 2;
#pragma omp parallel for
        for (int col = -radius; col < radius; col++) {
            for (int row = -radius; row < radius; row++) {
                if (y + row >= 0 && y + row < array.rows() && x + col >= 0 && x + col < array.cols()) {
                    snapshot(radius + row, radius + col) = array(y + row, x + col);
                } else {
                    snapshot(radius + row, radius + col) = 0.0;
                }

            }
        }
    }




}