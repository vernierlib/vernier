/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Spatial.hpp"

namespace vernier {

    void Spatial::quartersUnwrapPhase(Eigen::ArrayXXd& wrappedPhase) {
        int sizeX = wrappedPhase.cols();
        int sizeY = wrappedPhase.rows();
        int origineX = (sizeX / 2);
        int origineY = (sizeY / 2);
 
        int phaseIterationX, phaseIterationY;
        double phaseValuePrevX, phaseValuePrevY, phaseValueNextX, phaseValueNextY;
        double difference;

        phaseIterationX = 0;
        phaseValueNextX = wrappedPhase(origineY, origineX);

        // Loop for the left half of the wrapped phase
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

            // Quarter 3
            phaseIterationY = phaseIterationX;
            phaseValueNextY = phaseValueNextX;
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

            // Quarter 2
            phaseIterationY = phaseIterationX;
            phaseValueNextY = phaseValueNextX;
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
        // first row unwrapping
        // Quarter 3 first row
        phaseIterationY = phaseIterationX;
        phaseValueNextY = phaseValueNextX;
        for (int row = origineY; row > 0; row--) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row - 1, 0);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row - 1, 0) = phaseValueNextY + phaseIterationY * 2 * PI;
        }

        // Quarter 2 first row
        phaseIterationY = phaseIterationX;
        phaseValueNextY = phaseValueNextX;
        for (int row = origineY; row < sizeY - 1; row++) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row + 1, 0);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row + 1, 0) = phaseValueNextY + phaseIterationY * 2 * PI;
        }

        // Loop for the right half of the wrapped phase
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

            // Quarter 4
            phaseIterationY = phaseIterationX;
            phaseValueNextY = phaseValueNextX;
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

            // Quarter 1
            phaseIterationY = phaseIterationX;
            phaseValueNextY = phaseValueNextX;
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
        // last row unwrapping
        // Quarter 4 last row
        phaseIterationY = phaseIterationX;
        phaseValueNextY = phaseValueNextX;
        for (int row = origineY; row > 0; row--) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row - 1, sizeX - 1);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row - 1, sizeX - 1) = phaseValueNextY + phaseIterationY * 2 * PI;
        }

        // Quarter 1 last row
        phaseIterationY = phaseIterationX;
        phaseValueNextY = phaseValueNextX;
        for (int row = origineY; row < sizeY - 1; row++) {
            phaseValuePrevY = phaseValueNextY;
            phaseValueNextY = wrappedPhase(row + 1, sizeX - 1);
            difference = phaseValueNextY - phaseValuePrevY;

            if (difference > PI) {
                phaseIterationY = phaseIterationY - 1;
            } else if (difference <= -PI) {
                phaseIterationY = phaseIterationY + 1;
            }
            wrappedPhase(row + 1, sizeX - 1) = phaseValueNextY + phaseIterationY * 2 * PI;
        }
    }
}