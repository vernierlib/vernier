/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "MegarenaPatternDetector.hpp"
#include "MegarenaBitSequences.cpp"

namespace Vernier {

    MegarenaPatternDetector::MegarenaPatternDetector()
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";
    }

#ifdef USE_OPENCV

    MegarenaPatternDetector::MegarenaPatternDetector(cv::Mat image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod)
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";
        resize(image, bitSequence, approxPixelPeriod, physicalPeriod);
    }
#endif // USE_OPENCV

    MegarenaPatternDetector::MegarenaPatternDetector(Eigen::ArrayXXcd image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod)
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";
        resize(image, bitSequence, approxPixelPeriod, physicalPeriod);
    }

    MegarenaPatternDetector::MegarenaPatternDetector(Eigen::ArrayXXi bitSequence, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod, double sigmaPercent)
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";
        resize(bitSequence, nRows, nCols, approxPixelPeriod, physicalPeriod, sigmaPercent);
    }

    MegarenaPatternDetector::MegarenaPatternDetector(int codeSize, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod, double sigmaPercent)
    : PeriodicPatternDetector() {
        classname = "MegarenaPattern";

        if (codeSize == 8) {
            initializeBitSequence8(bitSequence);
        } else if (codeSize == 12) {
            initializeBitSequence12(bitSequence);
        }

        resize(bitSequence, nRows, nCols, approxPixelPeriod, physicalPeriod, sigmaPercent);
    }

    void MegarenaPatternDetector::resize(Eigen::ArrayXXi bitSequence, int nRows, int nCols, double approxPixelPeriod, double physicalPeriod, double sigmaPercent) {
        //PeriodicPatternDetector::resize(nRows, nCols, approxPixelPeriod, physicalPeriod, sigmaPercent);
        setPixelPeriod(approxPixelPeriod);
        setPhysicalPeriod(physicalPeriod);
        PeriodicPatternDetector::resize(nRows, nCols);
        PeriodicPatternDetector::setSigma(sigmaPercent);

        this->bitSequence = bitSequence;
        decoding.resize(bitSequence);

        this->length1 = nRows / approxPixelPeriod;
        this->length2 = nRows / approxPixelPeriod;

        length1++;
        length2++;

        if (length1 % 2 == 0) {
            length1++;
        }
        if (length2 % 2 == 0) {
            length2++;
        }

        if (approxPixelPeriod == 0) {
            patternPhase.setPeaksSearchMethod(1);
        }
    }

#ifdef USE_OPENCV

    void MegarenaPatternDetector::resize(cv::Mat image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod) {
        resize(bitSequence, image.rows, image.cols, approxPixelPeriod, physicalPeriod, 0.001);
    }
#endif // USE_OPENCV

    void MegarenaPatternDetector::resize(Eigen::ArrayXXcd image, Eigen::ArrayXXi bitSequence, double approxPixelPeriod, double physicalPeriod) {
        resize(bitSequence, image.rows(), image.cols(), approxPixelPeriod, physicalPeriod, 0.001);
    }

    void MegarenaPatternDetector::resize(int nRows, int nCols) {
        //        if (nRows != this->nRows || nCols != this->nCols) {
        //            this->nRows = nRows;
        //            this->nCols = nCols;
        //            PeriodicPatternDetector::resize(nRows, nCols);
        //            pattern.resize(nRows, nCols);      
        //        }
    }

    void MegarenaPatternDetector::compute(Eigen::ArrayXXd& pattern) {
        resize(pattern.rows(), pattern.cols());
        PeriodicPatternDetector::compute(pattern);
        computeAbsolutePose(pattern);
    }

#ifdef USE_OPENCV

    void MegarenaPatternDetector::computePerspective(Eigen::ArrayXXd& pattern) {
        resize(pattern.rows(), pattern.cols());

        this->setPerspectiveMode();
        PeriodicPatternDetector::compute(pattern);

        plane1 = PeriodicPatternDetector::getPlane1();
        plane2 = PeriodicPatternDetector::getPlane2();

        computeAbsolutePose(pattern);

    }
#endif // USE_OPENCV

    void MegarenaPatternDetector::computeAbsolutePose(Eigen::ArrayXXd& pattern) {
        double approxPixelPeriod = (plane1.getPixelicPeriod() + plane2.getPixelicPeriod()) / 2.0;
        patternPhase.setPixelPeriod(approxPixelPeriod);

        double roughPatternAngle = plane1.getAngle();

        //cv::imshow("Spectrum before", getGuiSpectrum());


        length1 = (pattern.rows() / (approxPixelPeriod)); // / (cos(abs(roughPatternAngle)));
        length2 = (pattern.cols() / (approxPixelPeriod)); // / (cos(abs(roughPatternAngle)));

        length1++;
        length2++;

        if (length1 % 2 == 0) {
            length1++;
        }
        if (length2 % 2 == 0) {
            length2++;
        }

        thumbnail.resize(length1, length2);

        thumbnail.compute(plane1, plane2, pattern);

        Eigen::ArrayXXd sequence1 = thumbnail.getSequence1();
        Eigen::ArrayXXd sequence2 = thumbnail.getSequence2();

        int MSB1 = thumbnail.getMSB1();
        int MSB2 = thumbnail.getMSB2();

        codePosition1 = decoding.findCodePosition(sequence1, MSB1);
        codePosition2 = decoding.findCodePosition(sequence2, MSB2);

        plane1Save = plane1;
        plane2Save = plane2;


        // if (codePosition1 >= 0 && codePosition2 >= 0) {
        // 	std::swap(codePosition1, codePosition2);
        // 	std::swap(plane1, plane2);
        // }
        // if (codePosition1 < 0 && codePosition2 >= 0) {
        // 	plane1.revertCoefficients();
        // }
        // if (codePosition1 < 0 && codePosition2 < 0) {
        // 	std::swap(codePosition1, codePosition2);
        // 	std::swap(plane1, plane2);
        // 	plane1.revertCoefficients();
        // 	plane2.revertCoefficients();
        // }
        // if (codePosition1 >= 0 && codePosition2 < 0) {
        // 	plane2.revertCoefficients();
        // }

        // std::cout<<"Code position 1 : "<<codePosition1<<std::endl;
        // std::cout<<"Code position 2 : "<<codePosition2<<std::endl;
        if (codePosition1 >= 0 && codePosition2 >= 0) {
            // std::cout<<"No change"<<std::endl;		
        } else if (codePosition1 < 0 && codePosition2 >= 0) {
            // std::cout<<"-PI/2 rotation"<<std::endl;
            std::swap(codePosition1, codePosition2);
            std::swap(plane1, plane2);
            plane2.revertCoefficients();
        } else if (codePosition1 >= 0 && codePosition2 < 0) {
            // std::cout<<"+PI/2 rotation"<<std::endl;
            std::swap(codePosition1, codePosition2);
            std::swap(plane1, plane2);
            plane1.revertCoefficients();
        } else {
            // std::cout<<"PI rotation"<<std::endl;
            plane1.revertCoefficients();
            plane2.revertCoefficients();
        }
        // std::cout<<"Code position 1 : "<<codePosition1<<std::endl;
        // std::cout<<"Code position 2 : "<<codePosition2<<std::endl;

    }

    Pose MegarenaPatternDetector::get2DPose() {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, codePosition1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, codePosition2);
        double alpha = plane1.getAngle();
        double pixelSize = physicalPeriod / plane1.getPixelicPeriod();

        return Pose(x, y, alpha, pixelSize);
    }

    //     Eigen::Matrix4d MegarenaDetector::getPatternPose() {
    //         double y = -plane1.getPosition(physicalPeriod, -0.5, -0.5, codePosition1);
    //         double x = -plane2.getPosition(physicalPeriod, -0.5, -0.5, codePosition2);
    //         double alpha = getPlane1().getAngle();

    //         return Pose(x,y,alpha);
    //     }

    // Pose MegarenaDetector::getCameraPoseInPattern() {
    // 	pose2D.x = plane1.getPosition(physicalPeriod, -0.5, -0.5, codePosition1);
    // 	pose2D.y = plane2.getPosition(physicalPeriod, -0.5, -0.5, codePosition2);

    // 	pose2D.alpha = std::fmod(plane2.getAngle(), PI);

    // 	return pose2D;
    // }

    // std::vector<Pose> MegarenaDetector::get3DPose() {
    // 	pose2D.x = plane1.getPosition(physicalPeriod, -0.5, -0.5, codePosition1);
    // 	pose2D.y = plane2.getPosition(physicalPeriod, -0.5, -0.5, codePosition2);

    // 	double anglePlane1 = std::fmod(plane1Save.getAngle(), PI / 2.0);
    // 	double anglePlane2 = std::fmod(plane2.getAngle(), PI);

    // 	if (anglePlane2 < -3 * PI / 4.0) {
    // 		anglePlane1 -= PI;
    // 	}
    // 	else if (anglePlane2 >= -3 * PI / 4.0 && anglePlane2 < -PI / 4.0) {
    // 		anglePlane1 -= PI / 2.0;
    // 	}
    // 	else if (anglePlane2 >= PI / 4.0 && anglePlane2 < 3 * PI / 4.0) {
    // 		anglePlane1 += PI / 2.0;
    // 	}
    // 	else if (anglePlane2 >= 3 * PI / 4.0) {
    // 		anglePlane1 += PI;
    // 	}

    // 	//pose2D.alpha = std::fmod(plane1Save.getAngle(), PI) + quarter * PI / 2.0;
    // 	pose2D.alpha = anglePlane1;

    // 	//PeriodicPatternDetector::compute(pattern);
    // 	plane1 = plane1Save;
    // 	plane2 = plane2Save;
    // 	std::vector<Pose> poseVec = PeriodicPatternDetector::get3DPose();

    // 	for (int i = 0; i < 4; i++) {
    // 		poseVec[i].x = pose2D.x;
    // 		poseVec[i].y = pose2D.y;
    // 		poseVec[i].alpha = pose2D.alpha;
    // 	}

    // 	return poseVec;
    // }

    std::vector<Pose> MegarenaPatternDetector::getAll3DPoses() {
        double x = -plane1.getPosition(physicalPeriod, 0.0, 0.0, codePosition1);
        double y = -plane2.getPosition(physicalPeriod, 0.0, 0.0, codePosition2);

        std::vector<Pose> poseVec = PeriodicPatternDetector::getAll3DPoses();
        for (int i = 0; i < 4; i++) {
            poseVec[i].x = x;
            poseVec[i].y = y;
        }
        return poseVec;
    }

    Thumbnail MegarenaPatternDetector::getThumbnail() {
        return thumbnail;
    }

    void MegarenaPatternDetector::showControlImages() {
#ifdef USE_OPENCV  
        // cv::imshow("Thumbnails", array2image(this->thumbnail.getThumbnailArray()));
        //this->thumbnail.guiCodeDirection();
        //this->thumbnail.guiDisplayCode();
        cv::imshow("Thumbnail",array2image(this->thumbnail.getThumbnailArray()));
#endif // USE_OPENCV
        PeriodicPatternDetector::showControlImages();
    }

    void MegarenaPatternDetector::readJSON(rapidjson::Value& document) {

        PatternDetector::readJSON(document);

        if (document.HasMember("period") && document["period"].IsDouble()) {
            physicalPeriod = document["period"].GetDouble();
            PeriodicPatternDetector::setPhysicalPeriod(physicalPeriod);
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the period is missing or has a wrong format.");
        }

        if (document.HasMember("bitSequence") && document["bitSequence"].IsArray()) {
            bitSequence.resize(1, document["bitSequence"].Size());

            for (rapidjson::SizeType row = 0; row < bitSequence.cols(); row++) {
                const rapidjson::Value& value = document["bitSequence"][row];
                if (value.IsInt()) {
                    bitSequence(0, row) = value.GetInt();
                } else {
                    throw Exception("The file is not a valid bitmap pattern file, the row " + toString(row) + " of the bitmap has a wrong format");
                }
            }
        } else if (document.HasMember("codeSize") && document["codeSize"].IsInt()) {
            int codeSize = document["codeSize"].GetInt();
            if (codeSize == 8) {
                initializeBitSequence8(bitSequence);
            } else if (codeSize == 12) {
                initializeBitSequence12(bitSequence);
            } else {
                throw Exception("The file is not a valid megarena pattern file, the code size must be 8 or 12.");
            }
        } else {
            throw Exception("The file is not a valid bitmap pattern file, the bitmap is missing or has a wrong format.");
        }

        decoding.resize(bitSequence);

        //        if (document.HasMember("approxPeriod") && document["approxPeriod"].IsDouble()) {
        //            this->approxPixelPeriod = document["approxPeriod"].GetDouble();
        //        }

    }

    void MegarenaPatternDetector::removeNanFromArray(Eigen::ArrayXXd& image) {
        for (int i = 0; i < image.rows(); i++) {
            for (int j = 0; j < image.cols(); j++) {
                if (isnan(image(i, j))) {
                    image(i, j) = 0;
                }
            }
        }
    }

    int MegarenaPatternDetector::getInt(const std::string & attribute) {
        if (attribute == "codePosition1") {
            return codePosition1;
        } else if (attribute == "codePosition2") {
            return codePosition2;
        } else {
            return PeriodicPatternDetector::getInt(attribute);
        }
    }

    void* MegarenaPatternDetector::getObject(const std::string & attribute) {
        if (attribute == "bitSequence") {
            return &bitSequence;
        } else if (attribute == "decoding") {
            return &decoding;
        } else if (attribute == "thumbnail") {
            return &thumbnail;
        } else {
            return PatternDetector::getObject(attribute);
        }
    }



}