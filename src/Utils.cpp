/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "Utils.hpp"

namespace Vernier {

#ifdef USE_OPENCV

    void drawCameraFrame(cv::Mat& image) {
        int cx = image.cols / 2;
        int cy = image.rows / 2;
        cv::line(image, cv::Point(cx, cy), cv::Point(cx + cx / 5, cy), cv::Scalar(0, 0, 255));
        cv::line(image, cv::Point(cx, cy), cv::Point(cx, cy + cy / 5), cv::Scalar(0, 255, 0));
    }

    cv::Mat array2image(Eigen::ArrayXXd & array) {
        array -= array.minCoeff();
        array /= array.maxCoeff();
        Eigen::MatrixXd matrix;
        matrix = array.array();

        cv::Mat image64d;
        cv::eigen2cv(matrix, image64d);
        cv::Mat image8u(image64d.rows, image64d.cols, CV_8UC4);

        for (int row = 0; row < image64d.rows; ++row) {
            double* src = image64d.ptr<double>(row);
            uchar* dst = image8u.ptr<uchar>(row);
            for (int col = 0; col < image64d.cols; ++col) {
                uchar value;
                if (src[col] >= 1.0) {
                    value = (uchar) 255;
                } else if (src[col] <= 0.0) {
                    value = (uchar) 0;
                } else {
                    value = (uchar) (src[col]*255);
                } 
                dst[4 * col] = dst[4 * col + 1] = dst[4 * col + 2] = value;
                dst[4 * col + 3] = (uchar) 255;
            }
        }
        return image8u;
    }

    cv::Mat array2image(Eigen::ArrayXXcd & array) {
        Eigen::ArrayXXd spectrumAbs;
        spectrumAbs = array.abs();
        return array2image(spectrumAbs);
    }
    
    Eigen::ArrayXXd image2array(cv::Mat & image) {
        cv::Mat grayImage;
        if (image.channels() > 1) {
            cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);
        } else {
            grayImage = image;
        }

        grayImage.convertTo(grayImage, CV_64F);
        cv::normalize(grayImage, grayImage, 1.0, 0, cv::NORM_MINMAX);
        Eigen::MatrixXd patternMatrix;
        cv::cv2eigen(grayImage, patternMatrix);
        Eigen::ArrayXXd patternArray;
        patternArray = patternMatrix.array();
        return patternArray;
    }
#endif // USE_OPENCV

    void arrayShow(const std::string windowTitle, Eigen::ArrayXXd & array) {
#ifdef USE_OPENCV     
        cv::imshow(windowTitle, array2image(array));
        cv::waitKey();
#else
        std::cout << "OpenCV is required to show arrays." << std::endl;
#endif // USE_OPENCV
    }

    Eigen::ArrayXXd readPGMData(std::string filename, int& numrows, int& numcols) {
        int row = 0, col = 0;
        numrows = 0, numcols = 0;
        std::ifstream infile(filename);
        std::stringstream ss;
        std::string inputLine = "";

        // First line : version
        getline(infile, inputLine);
        //if (inputLine.compare("P2") != 0) std::cerr << "Version error" << std::endl;
        //else std::cout << "Version : " << inputLine << std::endl;

        // Second line : comment
        getline(infile, inputLine);
        //std::cout << "Comment : " << inputLine << std::endl;

        // Continue with a stringstream
        ss << infile.rdbuf();
        // Third line : size
        ss >> numcols >> numrows;
        //std::cout << numcols << " columns and " << numrows << " rows" << std::endl;

        if (numcols != 0 && numrows != 0) {
            // Following lines : data
            Eigen::ArrayXXd eigenArray(numrows, numcols);
            for (row = 0; row < numrows; ++row) {
                for (col = 0; col < numcols; ++col) {
                    //ss >> mArray[row][col];
                    ss >> eigenArray(row, col);
                }
            }

            eigenArray.transposeInPlace();
            return eigenArray;
        } else {
            throw Exception("can't read file");
        }
    }

    Eigen::ArrayXXd readPGM(std::string filename) {
        int row = 0, col = 0;
        int numrows = 0, numcols = 0;
        std::ifstream infile(filename);
        std::stringstream ss;
        std::string inputLine = "";

        // First line : version
        getline(infile, inputLine);
        //if (inputLine.compare("P2") != 0) std::cerr << "Version error" << std::endl;
        //else std::cout << "Version : " << inputLine << std::endl;

        // Second line : comment
        getline(infile, inputLine);
        //std::cout << "Comment : " << inputLine << std::endl;

        // Continue with a stringstream
        ss << infile.rdbuf();
        // Third line : size
        ss >> numcols >> numrows;
        //std::cout << numcols << " columns and " << numrows << " rows" << std::endl;

        if (numcols != 0 && numrows != 0) {
            // Following lines : data
            Eigen::ArrayXXd eigenArray(numrows, numcols);
            for (row = 0; row < numrows; ++row) {
                for (col = 0; col < numcols; ++col) {
                    //ss >> mArray[row][col];
                    ss >> eigenArray(row, col);
                }
            }

            eigenArray.transposeInPlace();
            return eigenArray;
        } else {
            throw Exception("can't read file");
        }
    }

    void removeNanFromArray(Eigen::ArrayXXd& array) {
        for (int i = 0; i < array.rows(); i++) {
            for (int j = 0; j < array.cols(); j++) {
                if (isnan(array(i, j))) {
                    array(i, j) = 0;
                }
            }
        }
    }
}