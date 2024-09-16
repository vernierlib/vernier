/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#ifndef QRCODETHUMBNAIL_HPP
#define QRCODETHUMBNAIL_HPP

#include "Common.hpp"
#include "Thumbnail.hpp"
#include "PatternPhase.hpp"

namespace Vernier {
    
    const int QRCODE_33P = 33;
    const int QRCODE_37P = 37;
    const int QRCODE_39P = 39;
    
#ifdef USE_OPENCV

    class QRCodeThumbnail {
    private:
        int nRows, nCols;
        Plane plane1, plane2;
        int length1, length2;
        Eigen::ArrayXXd qrCodeBackground;
        Eigen::ArrayXXd qrCodeForeground;
        int sideOffset;
        int size;
        Eigen::ArrayXXd thumbnailArray;
        double pixelPeriod;
        Thumbnail thumbnail;
        int centerX, centerY;
        PatternPhase snapPhase;

        void takeSnapshot(int x, int y, int radius, Eigen::ArrayXXcd image);

        void removeNanFromArray(Eigen::ArrayXXd& image);

        int detectQuarter(int rowStart, int colStart);

    public:
        Eigen::ArrayXXd snapshot;

        QRCodeThumbnail();

        void resize(int nRows, int nCols, int snapSize, int flagQRCode = 37);

        void compute(Eigen::ArrayXXd image, Plane& plane1, Plane& plane2);

        int getQRCodeCenterX();

        int getQRCodeCenterY();
    };
#endif //USE_OPENCV
}

#endif // !QRCODETHUMBNAIL_HPP