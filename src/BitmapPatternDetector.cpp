/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2025 CNRS, ENSMM, UMLP.
 */

#include "BitmapPatternDetector.hpp"

namespace vernier {

    BitmapPatternDetector::BitmapPatternDetector()
    : PeriodicPatternDetector() {
    }

    BitmapPatternDetector::BitmapPatternDetector(Eigen::ArrayXXd bitmapKernel, double approxPixelPeriod, double physicalPeriod)
    : PeriodicPatternDetector() {
        classname = "BitmapPatternDetector";
        resize(bitmapKernel, approxPixelPeriod, physicalPeriod);
    }

    BitmapPatternDetector::BitmapPatternDetector(std::string filename, double approxPixelPeriod)
    : PeriodicPatternDetector() {
        classname = "BitmapPatternDetector";
        resize(filename, approxPixelPeriod);
    }

    void BitmapPatternDetector::resize(Eigen::ArrayXXd bitmapKernel, double approxPixelPeriod, double physicalPeriod) {
        this->bitmapKernel = bitmapKernel;
        wTemplate = bitmapKernel.cols();
        hTemplate = bitmapKernel.rows();
        PeriodicPatternDetector::setPixelPeriod(approxPixelPeriod);
        PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        templatePrime.resize(hTemplate, wTemplate);
        imagePrime.resize(hTemplate, wTemplate);
    }

    void BitmapPatternDetector::readJSON(rapidjson::Value& document) {

        PatternDetector::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
            PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }
//        if (document.HasMember("approxPixelPeriod") && document["approxPixelPeriod"].IsDouble()) {
//            this->approxPixelPeriod = document["approxPixelPeriod"].GetDouble();
//            PeriodicPatternDetector::setApproxPixelPeriod(document["approxPixelPeriod"].GetDouble());
//        }
        if (document.HasMember("bitmap") && document["bitmap"].IsArray()) {
            hTemplate = document["bitmap"].Size();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
        }

        if (document["bitmap"][0].IsArray() && document["bitmap"][0].Size() > 0) {
            wTemplate = document["bitmap"][0].Size();
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the first row of the bitmap has a wrong format.");
        }

        bitmapKernel.resize(hTemplate, wTemplate);


        for (rapidjson::SizeType row = 0; row < hTemplate; row++) {
            const rapidjson::Value& value = document["bitmap"][row];
            if (value.IsArray() && value.Size() == wTemplate) {
                for (rapidjson::SizeType col = 0; col < wTemplate; col++) {
                    if (value[col].IsInt()) {
                        bitmapKernel(row, col) = value[col].GetInt();
                    } else {
                        Exception("The file is not a valid bitmap pattern file, the row " + to_string(row) + " of the bitmap has a wrong format");
                    }
                }
            } else {
                throw Exception("The file is not a valid bitmap pattern file, the row " + to_string(row) + " of the bitmap has a wrong format");
            }
        }

        int countZeros = 0;

        for (int i = 0; i < bitmapKernel.rows(); i++) {
            for (int j = 0; j < bitmapKernel.cols(); j++) {
                if (bitmapKernel(i, j) == 1) {
                    countZeros++;
                }
            }
        }
        double ratio = (double) countZeros / ((double) bitmapKernel.rows() * (double) bitmapKernel.cols());

        //std::cout << "ratio of zeros : " << ratio << std::endl;

        if (ratio > 0.5) {
            padKernel();
            hTemplate = bitmapKernel.rows();
            wTemplate = bitmapKernel.cols();
        }

        templatePrime.resize(hTemplate, wTemplate);
        imagePrime.resize(hTemplate, wTemplate);


        //if (document.HasMember("sigmaFilter") && document["sigmaFilter"].IsDouble()) {
        //	this->sigmaPercent = document["sigmaFilter"].GetDouble();
        //}
        //else {
        //	this->sigmaPercent = 0.005;
        //}

        bitmapKernel.transposeInPlace();

        Eigen::ArrayXXd kernel1, kernel2, kernel3, kernel4;
        kernel1 = bitmapKernel;
        rotate90deg(bitmapKernel);
        kernel2 = bitmapKernel;
        rotate90deg(bitmapKernel);
        kernel3 = bitmapKernel;
        rotate90deg(bitmapKernel);
        kernel4 = bitmapKernel;
        rotate90deg(bitmapKernel);

        if (kernel1.isApprox(kernel2) && kernel2.isApprox(kernel3) && kernel3.isApprox(kernel4)) {
            this->isSymetric = true;
        } else {
            this->isSymetric = false;
        }
        if (bitmapKernel.rows() % 4 != 1) {
            isInverted = true;
        } else {
            isInverted = false;
        }

        //std::cout << "is symetric : " << isSymetric << std::endl;
        //std::cout << "is inverted : " << isInverted << std::endl;
    }

    void BitmapPatternDetector::resize(std::string filename, double approxPixelPeriod) {
        BufferedReader bufferedReader(filename);
        rapidjson::Document document;
        document.ParseInsitu(bufferedReader.data());
        if (!document.IsObject()) {
            throw Exception(filename + " is not a valid JSON file.");
        }

        readJSON(document);
        PeriodicPatternDetector::setPixelPeriod(approxPixelPeriod);
        PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        templatePrime.resize(hTemplate, wTemplate);
        imagePrime.resize(hTemplate, wTemplate);
    }

    void BitmapPatternDetector::resize(int nRows, int nCols) {
//        if (nRows != this->nRows || nCols != this->nCols) {
//            this->nRows = nRows;
//            this->nCols = nCols;
//            PeriodicPatternDetector::resize(nRows, nCols);
//            pattern.resize(nRows, nCols);
//            snapshot.resize(nRows, nCols);
//            this->thumbLength1 = 0;
//            this->thumbLength2 = 0;
//        }
    }

    void BitmapPatternDetector::compute(Eigen::ArrayXXd& pattern) {
        this->pattern = pattern;
        resize(pattern.rows(), pattern.cols());

        PeriodicPatternDetector::setSigma(0.0001);
//        PeriodicPatternDetector::resize(nRows, nCols);

        PeriodicPatternDetector::compute(pattern);

        computeAbsolutePose(pattern);
    }



    void BitmapPatternDetector::computePerspective(Eigen::ArrayXXd& pattern) {
        this->pattern = pattern;
        resize(pattern.rows(), pattern.cols());

        PeriodicPatternDetector::setSigma(0.0001);
//        PeriodicPatternDetector::resize(nRows, nCols);

        this->setPerspectiveMode();
        PeriodicPatternDetector::compute(pattern);

        computeAbsolutePose(pattern);
    }


    void BitmapPatternDetector::computeAbsolutePose(Eigen::ArrayXXd& patternImage) {

        //THUMBNAIL COMPUTING
        if (thumbLength1 == 0 || thumbLength2 == 0) {
            double approxPixelPeriod = (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0;
            patternPhase.setPixelPeriod(approxPixelPeriod);
            this->thumbLength1 = (int) (round((2.0 * (double) patternImage.rows() / (approxPixelPeriod))));
            this->thumbLength2 = (int) (round((2.0 * (double) patternImage.cols() / (approxPixelPeriod))));

            if (thumbLength1 % 2 != 0) {
                thumbLength1++;
            }
            if (thumbLength2 % 2 != 0) {
                thumbLength2++;
            }
        }


        thumbnail.resize(thumbLength1, thumbLength2);

        thumbnail.computeThumbnailTotal(plane1, plane2, patternImage, PI / 3.0);

        //cv::Mat thumbnailImg(100, 100, CV_64FC3);
        //thumbnail.drawThumbnailDetection(thumbnailImg);
        //cv::imshow("thumbnail", thumbnailImg);

        thumbnailArray = thumbnail.getThumbnailArray();
        removeNanFromArray(thumbnailArray);
        thumbnailArray /= thumbnailArray.maxCoeff();

        this->quarter = getThumbnailCenter(thumbnailArray, bitmapKernel, centerRow, centerCol);

        double a1 = plane1.getA();
        double b1 = plane1.getB();
        double c1 = plane1.getC();
        phiOffset1 = plane1.getC();

        double a2 = plane2.getA();
        double b2 = plane2.getB();
        double c2 = plane2.getC();
        phiOffset2 = plane2.getC();

        phiRoughX = (double) (centerRow - thumbLength1 / 2) * PI;
        phiRoughY = (double) (centerCol - thumbLength2 / 2) * PI;

        yPattern = (a2 * (phiOffset1 - phiRoughX) - a1 * (phiOffset2 - phiRoughY)) / (a1 * b2 - a2 * b1);
        xPattern = (b1 * (phiOffset2 - phiRoughY) - (phiOffset1 - phiRoughX) * b2) / (a1 * b2 - a2 * b1);

        xPattern += (double) pattern.cols() / 2.0;
        yPattern += (double) pattern.rows() / 2.0;


        //FINE BITMAP POSITION DETECTION
        double radius = patternPhase.getPixelPeriod() * bitmapKernel.rows() / 4.0;
        takeSnapshot(snapshot, radius, (int) round(xPattern), (int) round(yPattern));

        Eigen::ArrayXXd snapArray = snapshot.real();

        //		cv::Mat snapImg(snapshot.rows(), snapshot.cols(), CV_64FC1, snapArray.data());
        //		cv::imshow("snap", snapImg / 4096);
        //		cv::imwrite("D:/Nextcloud2/Antoine/Articles/TASE_HPCodes/Figures/snapshotImgTiltedHPCode.png", snapImg / 16);


        PeriodicPatternDetector::setSigma(0.0033);
//        PeriodicPatternDetector::resize(nRows, nCols);
        PeriodicPatternDetector::compute(snapArray);

        patternPhase.setPixelPeriod( (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0 );
    }

//    void BitmapPatternDetector::compute(char* data, int nRows, int nCols) {
//        Eigen::MatrixXd mPatternMatrix(nRows, nCols);
//        std::memcpy(mPatternMatrix.data(), data, nRows * nCols * sizeof (double));
//        mPatternMatrix.transposeInPlace();
//        pattern = mPatternMatrix.array();
//        compute(pattern);
//    }

    Pose BitmapPatternDetector::getCameraPoseInPattern() {
        plane1.setC(plane1.getC() + phiOffset1);
        plane2.setC(plane2.getC() + phiOffset2);
        Pose pose2D = PeriodicPatternDetector::get2DPose();
        plane1.setC(plane1.getC() - phiOffset1);
        plane2.setC(plane2.getC() - phiOffset2);
        return pose2D;
    }

    Pose BitmapPatternDetector::getPatternPoseInCamera() {
        double physicalPeriodSave = this->physicalPeriod;
        this->physicalPeriod = 0;
        Pose pose2D = PeriodicPatternDetector::get2DPose();

        pose2D.x += (int) round(xPattern);
        pose2D.y += (int) round(yPattern);
        double pixelSize;
        if (physicalPeriod == 0) {
            pixelSize = 1;
        } else {
            pixelSize = physicalPeriod / patternPhase.getPixelPeriod();
        }
        pose2D.x *= pixelSize;
        pose2D.y *= pixelSize;

        pose2D.alpha += (double) quarter * PI / 2.0;

        std::cout << "pose cpp " << pose2D.x << "; " << pose2D.y << std::endl;

        return pose2D;
    }

    std::vector<Pose> BitmapPatternDetector::getAll3DPoses(int id) {

        Pose pose2D = getPatternPoseInCamera();

        std::vector<Pose> poseVec = PeriodicPatternDetector::getAll3DPoses();

        for (int i = 0; i < 4; i++) {
            poseVec[i].x = pose2D.x;
            poseVec[i].y = pose2D.y;
            poseVec[i].alpha = pose2D.alpha + (double) quarter * PI / 2.0;
        }

        return poseVec;

    }

    Plane BitmapPatternDetector::getPlane1() {
        return plane1;
    }

    Plane BitmapPatternDetector::getPlane2() {
        return plane2;
    }

//    void BitmapPatternDetector::removeNanFromArray(Eigen::ArrayXXd& image) {
//        for (int i = 0; i < image.rows(); i++) {
//            for (int j = 0; j < image.cols(); j++) {
//                if (isnan(image(i, j))) {
//                    image(i, j) = 0;
//                }
//            }
//        }
//    }

    void BitmapPatternDetector::takeSnapshot(Eigen::ArrayXXcd& snapshot, double radius, double xCenter, double yCenter) {
        if (isInverted) {
            pattern = 1.0 - pattern;
        }
        int size = snapshot.rows();
        for (int col = -radius; col < radius; col++) {
            for (int row = -radius; row < radius; row++) {
                double distanceToCenter = sqrt((row + 0.5) * (row + 0.5) + (col + 0.5) * (col + 0.5));
                //double hanning = 0.42659 - 0.49656 * cos(2 * PI * distanceToCenter / radius) + 0.076849 * cos(4 * PI * distanceToCenter / radius);
                double hanning = (1 + cos(vernier::PI * distanceToCenter / radius)) / 2;
                if (distanceToCenter < radius && yCenter + row > 0 && xCenter + col > 0 && yCenter + row < pattern.rows() && xCenter + col < pattern.cols()) {
                    snapshot.real()(size / 2 + row, size / 2 + col) = hanning * pattern.real()(yCenter + row, xCenter + col);
                    //snapshot.real()(size / 2 + row, size / 2 + col) = pattern.real()(yCenter + row, xCenter + col);
                }
            }
        }
    }

    int BitmapPatternDetector::getThumbnailCenter(Eigen::ArrayXXd& thumbnailArray, Eigen::ArrayXXd& bitmapKernel, int& centerRow, int& centerCol) {
        //MATCHING TEMPLATE ON THUMBNAIL
        int WImage = thumbnailArray.cols();
        int HImage = thumbnailArray.rows();
        Eigen::ArrayXXd maxMatch(4, 3);

        Eigen::MatrixXf::Index maxRowEigen, maxColEigen;
        Eigen::MatrixXf::Index maxRowMatch, maxColMatch;


        cv::Mat thumbnailImg(thumbnailArray.rows(), thumbnailArray.cols(), CV_64FC1, thumbnailArray.data());
        cv::Mat templateImg(bitmapKernel.rows(), bitmapKernel.cols(), CV_64FC1, bitmapKernel.data());
        thumbnailImg.convertTo(thumbnailImg, CV_32FC1);
        templateImg.convertTo(templateImg, CV_32FC1);

        cv::Mat resultImg(thumbnailImg.rows - templateImg.rows + 1, thumbnailImg.cols - templateImg.cols + 1, CV_32FC1);


        if (isSymetric) {

            cv::matchTemplate(thumbnailImg, templateImg, resultImg, cv::TM_CCOEFF_NORMED);

            cv::normalize(resultImg, resultImg, 1, 0, cv::NORM_MINMAX);

            double minVal;
            double maxVal;
            cv::Point minLoc;
            cv::Point maxLoc;
            minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

            centerRow = maxLoc.x + bitmapKernel.rows() / 2;
            centerRow = maxLoc.y + bitmapKernel.cols() / 2;

//            templatePrime = bitmapKernel - (1.0 / ((double) wTemplate * (double) hTemplate)) * bitmapKernel.sum();
//            resultArray.resize(HImage - hTemplate + 1, WImage - wTemplate + 1);
//
//            for (int i = 0; i < HImage - hTemplate + 1; i++) {
//                for (int j = 0; j < WImage - wTemplate + 1; j++) {
//                    imagePrime = thumbnailArray.block(i, j, hTemplate, wTemplate) - (1.0 / ((double) wTemplate * (double) hTemplate)) * thumbnailArray.block(i, j, hTemplate, wTemplate).sum();
//                    resultArray(i, j) = (templatePrime * imagePrime).sum() / (sqrt(templatePrime.pow(2).sum() * imagePrime.pow(2).sum()));
//                }
//            }
//
//            double max = resultArray.maxCoeff(&maxRowEigen, &maxColEigen);
//
//            centerRow = maxRowEigen + bitmapKernel.rows() / 2;
//            centerCol = maxColEigen + bitmapKernel.cols() / 2;

            maxRowMatch = 0;
        } else {
            for (int i = 0; i < 4; i++) {


                cv::matchTemplate(thumbnailImg, templateImg, resultImg, cv::TM_CCOEFF_NORMED);

                cv::normalize(resultImg, resultImg, 1, 0, cv::NORM_MINMAX);

                double minVal;
                double maxVal;
                cv::Point minLoc;
                cv::Point maxLoc;
                minMaxLoc(resultImg, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

                maxMatch(i, 0) = maxVal;
                maxMatch(i, 1) = maxLoc.x;
                maxMatch(i, 2) = maxLoc.y;


//                templatePrime = bitmapKernel - (1.0 / ((double) wTemplate * (double) hTemplate)) * bitmapKernel.sum();
//                resultArray.resize(HImage - hTemplate + 1, WImage - wTemplate + 1);
//                for (int i = 0; i < HImage - hTemplate + 1; i++) {
//                    for (int j = 0; j < WImage - wTemplate + 1; j++) {
//                        imagePrime = thumbnailArray.block(i, j, hTemplate, wTemplate) - (1.0 / ((double) wTemplate * (double) hTemplate)) * thumbnailArray.block(i, j, hTemplate, wTemplate).sum();
//                        resultArray(i, j) = (templatePrime * imagePrime).sum() / (sqrt(templatePrime.pow(2).sum() * imagePrime.pow(2).sum()));
//                    }
//                }
//
//                double max = resultArray.maxCoeff(&maxRowEigen, &maxColEigen);
//
//                maxMatch(i, 0) = max;
//                maxMatch(i, 1) = maxRowEigen;
//                maxMatch(i, 2) = maxColEigen;


                rotate90deg(bitmapKernel);
            }

            double maxMatchResult = maxMatch.block(0, 0, 4, 1).maxCoeff(&maxRowMatch, &maxColMatch);

            //SEARCHNG FOR ROUGH BITMAP CENTER
            centerRow = maxMatch(maxRowMatch, 1) + bitmapKernel.rows() / 2;
            centerCol = maxMatch(maxColMatch, 2) + bitmapKernel.cols() / 2;

            maxRowMatch += 2;

        }

        return maxRowMatch;
    }

    void BitmapPatternDetector::rotate90deg(Eigen::ArrayXXd& bitmapKernel) {
        int halfFloor = floor((double) bitmapKernel.rows() / 2.0);
        int halfCeil = ceil((double) bitmapKernel.cols() / 2.0);

        double temp;
        for (int i = 0; i < halfFloor; i++) {
            for (int j = 0; j < halfCeil; j++) {

                temp = bitmapKernel(i, j);
                bitmapKernel(i, j) = bitmapKernel(j, bitmapKernel.cols() - 1 - i);
                bitmapKernel(j, bitmapKernel.cols() - 1 - i) = bitmapKernel(bitmapKernel.cols() - 1 - i, bitmapKernel.cols() - 1 - j);
                bitmapKernel(bitmapKernel.cols() - 1 - i, bitmapKernel.cols() - 1 - j) = bitmapKernel(bitmapKernel.cols() - 1 - j, i);
                bitmapKernel(bitmapKernel.cols() - 1 - j, i) = temp;
            }
        }
    }

    void BitmapPatternDetector::padKernel() {
        Eigen::ArrayXXd bitmapKernelPadded(bitmapKernel.rows() * 2 - 1, bitmapKernel.cols() * 2 - 1);
        bitmapKernelPadded.setZero();
        for (int i = 0; i < bitmapKernel.rows(); i++) {
            for (int j = 0; j < bitmapKernel.cols(); j++) {
                if (bitmapKernel(i, j) == 1) {
                    bitmapKernelPadded(2 * i, 2 * j) = 1;
                }
            }
        }

        this->bitmapKernel.resize(bitmapKernelPadded.rows(), bitmapKernelPadded.cols());
        this->bitmapKernel = bitmapKernelPadded;
    }



    void BitmapPatternDetector::guiDisplayThumbnail(cv::Mat& thumbnailImage) {
        int rectSize = round(patternPhase.getPixelPeriod() / 2.0);
        for (int i = 0; i < thumbnailArray.rows(); i++) {
            for (int j = 0; j < thumbnailArray.cols(); j++) {
                cv::rectangle(thumbnailImage, cv::Rect(i * rectSize, j * rectSize, rectSize, rectSize), cv::Scalar(thumbnailArray(i, j), thumbnailArray(i, j), thumbnailArray(i, j)), -1);
            }
        }
        quarter -= 2;
        // DRAW THUMBNAIL FOR FEEDBACK
        for (int k = 0; k < quarter; k++) {
            rotate90deg(bitmapKernel);
        }

        double rectWidth = rectSize;

        for (int i = 0; i < bitmapKernel.rows(); i++) {
            for (int j = 0; j < bitmapKernel.cols(); j++) {
                if (bitmapKernel(i, j) != 0) {
                    cv::Scalar colorThumbKernel = cv::Scalar(0, bitmapKernel(i, j), 0);
                    cv::rectangle(thumbnailImage, cv::Rect((i + centerRow - bitmapKernel.rows() / 2.0 + 0.5) * rectSize, (j + centerCol - bitmapKernel.rows() / 2.0 + 0.5) * rectSize, rectWidth, rectWidth), colorThumbKernel, 2);
                }
            }
        }
        cv::rectangle(thumbnailImage, cv::Rect(centerRow * rectSize, centerCol * rectSize, rectWidth, rectWidth), cv::Scalar(1, 0, 0), -1);
        cv::putText(thumbnailImage, "quarter : " + to_string(quarter), cv::Point(20, 40), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, cv::Scalar(255, 0, 0), 1);

    }

}