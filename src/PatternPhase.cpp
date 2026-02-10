/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PatternPhase.hpp"

namespace vernier {

    PatternPhase::PatternPhase() {
        setSigma(3);
    }

    PatternPhase::PatternPhase(int nRows, int nCols) : PatternPhase() {
        resize(nRows, nCols);
    }

    void PatternPhase::resize(int nRows, int nCols) {
        ASSERT_MSG(nCols > 0 && nRows > 0, "The image is empty.")
        if (nRows != spectrum.rows() || nCols != spectrum.cols()) {
            fft.resize(nRows, nCols, FFTW_FORWARD);
            ifft.resize(nRows, nCols, FFTW_BACKWARD);
            regressionPlane.resize(nRows, nCols);
            spectrum.resize(nRows, nCols);
            spectrumShifted.resize(nRows, nCols);
            spectrumFiltered1.resize(nRows, nCols);
            spectrumFiltered1.resize(nRows, nCols);
            phase1.resize(nRows, nCols);
            phase2.resize(nRows, nCols);
            unwrappedPhase1.resize(nRows, nCols);
            unwrappedPhase2.resize(nRows, nCols);
            spatial.resize(nRows, nCols);
        }
    }

    void PatternPhase::compute(const Eigen::ArrayXXd& image) {
        resize(image.rows(), image.cols());
        spatial.setZero();
        spatial.real() = image;
        compute();
    }

    void PatternPhase::compute(const cv::Mat& image) {
        resize(image.rows, image.cols);
        Eigen::ArrayXXd array;
        image2arrayXXd(image, array);
        spatial.setZero();
        spatial.real() = array;
        compute();
    }

    void PatternPhase::compute() {
        fft.compute(spatial, spectrum);

        shift(spectrum, spectrumShifted);
        spectrumFiltered1 = spectrumShifted;
        spectrumFiltered2 = spectrumShifted;
        
        Eigen::ArrayXXd magnitude = spectrumShifted.abs();
        peaksSearch(magnitude, mainPeak1, mainPeak2);

        // Compute first plane phase from peak 1
        applyGaussianFilter(spectrumFiltered1, mainPeak1(1), mainPeak1(0), sigma);

        ifft.compute(spectrumFiltered1, phase1);
        shift(phase1);

        unwrappedPhase1 = phase1.arg();

        quartersUnwrapPhase(unwrappedPhase1);

        plane1 = regressionPlane.compute(unwrappedPhase1);

        // Compute second plase from peak 2
        applyGaussianFilter(spectrumFiltered2, mainPeak2(1), mainPeak2(0), sigma);
        ifft.compute(spectrumFiltered2, phase2);
        shift(phase2);

        unwrappedPhase2 = phase2.arg();

        quartersUnwrapPhase(unwrappedPhase2);

        plane2 = regressionPlane.compute(unwrappedPhase2);
    }
    
    void PatternPhase::peaksSearch(Eigen::ArrayXXd& source, Eigen::Vector3d& mainPeak1, Eigen::Vector3d& mainPeak2) {
        mainPeak1.setConstant(-1);
        mainPeak2.setConstant(-1);
        
        applyBandPassCut(source, MIN_FREQUENCY, MAX_FREQUENCY);
        
        // Filtre moyenneur 3x3 -> à faire avant dans le domaine spatial
        // puis utiliser source.maxCoeff(&row, &col);
        
        double maxValue = -1.0;
        for (int col = 1; col < source.cols() - 1; col++) {
            for (int row = source.rows() / 2; row < source.rows() - 1; row++) {
                double mean = (source(row, col) + source(row - 1, col) + source(row, col - 1) + source(row + 1, col) + source(row, col + 1)) / 5.0;
                if (mean > maxValue) {
                    maxValue = mean;
                    mainPeak1.x() = col;
                    mainPeak1.y() = row;
                    mainPeak1.z() = mean / source.cols() / source.rows();
                }
            }
        }
        
        if (mainPeak1.z() > MIN_PEAK_POWER) {
            double vx = mainPeak1.x() - source.cols() / 2;
            double vy = mainPeak1.y() - source.rows() / 2;
            double distance = std::hypot(vx,vy);
            
            double centerAngle = std::atan2(vy, vx);
            double widthAngle = 2.0 * std::atan2(3.0 * sigma, distance);
            applyAngularCut(source, centerAngle, widthAngle);
            
            maxValue = -1;
            for (int col = 1; col < source.cols() - 1; col++) {
                for (int row = source.rows() / 2; row < source.rows() - 1; row++) {
                    double mean = (source(row, col) + source(row - 1, col) + source(row, col - 1)+ source(row + 1, col) + source(row, col + 1)) / 5.0;
                    if (mean > maxValue) {
                        maxValue = mean;
                        mainPeak2.x() = col;
                        mainPeak2.y() = row;
                        mainPeak2.z() = mean / source.cols() / source.rows();
                    }
                }
            }
            
            if (mainPeak1.x() < mainPeak2.x()) {
                std::swap(mainPeak1, mainPeak2);
            }

        }      
    }

    void PatternPhase::computePhaseGradients(int& betaSign, int& gammaSign) {

        // Direction 1
        int sideOffset = regressionPlane.getColOffset();
        Eigen::ArrayXXd phaseCropped = unwrappedPhase1.block(sideOffset, sideOffset, unwrappedPhase1.rows() - 2 * sideOffset, unwrappedPhase1.cols() - 2 * sideOffset);

        cv::Mat phase1img(phaseCropped.rows(), phaseCropped.cols(), CV_64FC1, phaseCropped.data());
        cv::Mat phaseResult(phase1img.rows, phase1img.cols, CV_64FC1);

        double a = plane1.getA();
        double b = plane1.getB();

        cv::Mat phaseDerived(phase1img.rows - 1, phase1img.cols - 1, CV_64FC1);
        cv::Mat sobelX, sobelY;

        for (int i = 0; i < phase1img.rows - 1; i++) {
            for (int j = 0; j < phase1img.cols - 1; j++) {
                double dXdU = -(phaseCropped(i + 1, j) - phaseCropped(i, j)) * a / (pow(a, 2) + pow(b, 2));
                double dYdU = (phaseCropped(i, j + 1) - phaseCropped(i, j)) * b / (pow(a, 2) + pow(b, 2));
                phaseDerived.at<double>(i, j) = dXdU + dYdU;
            }
        }

        cv::Sobel(phaseDerived, sobelX, CV_64F, 1, 0);
        double mean = cv::mean(sobelX)[0];
        betaSign = (mean > 0) - (mean < 0);

        //std::cout << "mean first direction SOBEL : " << mean << std::endl;
        //cv::normalize(phaseDerived, phaseDerived, 1, 0, cv::NORM_MINMAX);
        //cv::imshow("phase 1 derived", phaseDerived);

        // Direction 2
        phaseCropped = unwrappedPhase2.block(sideOffset, sideOffset, unwrappedPhase2.rows() - 2 * sideOffset, unwrappedPhase2.cols() - 2 * sideOffset);
        cv::Mat phase2img(phaseCropped.rows(), phaseCropped.cols(), CV_64FC1, phaseCropped.data());
        cv::Mat phase2HSV;
        cv::Mat phase2BGR;
        cv::normalize(phase2img, phase2img, 255, 0, cv::NORM_MINMAX);
        phase2img.convertTo(phase2BGR, CV_8UC3);
        cv::cvtColor(phase2BGR, phase2BGR, cv::COLOR_GRAY2BGR);

        a = plane2.getA();
        b = plane2.getB();

        //cv::applyColorMap(phase2BGR, phase2HSV, cv::COLORMAP_VIRIDIS);
        //cv::line(phase2HSV, cv::Point(phase2HSV.cols / 4, phase2HSV.rows / 2), cv::Point(3 * phase2HSV.cols / 4, phase2HSV.rows / 2), cv::Scalar(255, 255, 255), 1);
        //cv::line(phase2HSV, cv::Point(phase2HSV.cols / 2, phase2HSV.rows / 4), cv::Point(phase2HSV.cols / 2, 3 * phase2HSV.rows / 4), cv::Scalar(255, 255, 255), 1);
        //cv::arrowedLine(phase2HSV, cv::Point(phase2HSV.cols / 2, phase2HSV.rows / 2), cv::Point(3 * phase2HSV.cols / 4, phase2HSV.rows / 2), cv::Scalar(255, 255, 255), 3);
        //cv::arrowedLine(phase2HSV, cv::Point(phase2HSV.cols / 2, phase2HSV.rows / 2), cv::Point(phase2HSV.cols / 2, 3 * phase2HSV.rows / 4), cv::Scalar(255, 255, 255), 3);
        //cv::arrowedLine(phase2HSV, cv::Point(phase2HSV.cols / 2, phase2HSV.rows / 2), cv::Point(phase2HSV.cols / 2 + 30.0 / a, phase2HSV.rows / 2 + 30.0 / b), cv::Scalar(0, 0, 255), 3);
        //cv::arrowedLine(phase2HSV, cv::Point(phase2HSV.cols / 2, phase2HSV.rows / 2), cv::Point(phase2HSV.cols / 2 - 30.0 / b, phase2HSV.rows / 2 + 30.0 / a), cv::Scalar(0, 0, 255), 3);
        //cv::imshow("phase 2", phase2HSV);
        //cv::imwrite("D:/Nextcloud2/classic3Dpatterns/analysis_scripts/perspectivePhaseEvolution/phaseMap.png", phase2HSV);

        for (int i = 0; i < phase1img.rows - 1; i++) {
            for (int j = 0; j < phase1img.cols - 1; j++) {
                double dXdV = -(phaseCropped(i + 1, j) - phaseCropped(i, j)) * a / (pow(a, 2) + pow(b, 2));
                double dYdV = (phaseCropped(i, j + 1) - phaseCropped(i, j)) * b / (pow(a, 2) + pow(b, 2));
                phaseDerived.at<double>(i, j) = dXdV + dYdV;
            }
        }

        cv::Sobel(phaseDerived, sobelY, CV_64F, 0, 1);
        double mean2 = cv::mean(sobelY)[0];

        //std::cout << "mean second direction SOBEL : " << mean2 << std::endl;
        gammaSign = (mean2 > 0) - (mean2 < 0);
    }

    void PatternPhase::showControlImages() {
        cv::imshow("Found peaks (red = direction 1, green = direction 2)", getPeaksImage());
        cv::moveWindow("Found peaks (red = direction 1, green = direction 2)", 0, 0);
        cv::imshow("Phase fringes (red = direction 1, green = direction 2)", getFringesImage());
        cv::moveWindow("Phase fringes (red = direction 1, green = direction 2)", getNCols(), 0);
    }

    cv::Mat PatternPhase::getPeaksImage() {
        int offsetMin = 10.0;
        double max = spectrumShifted.block(spectrumShifted.rows() / 2 - offsetMin / 2, spectrumShifted.cols() / 2 - offsetMin / 2, offsetMin, offsetMin).abs().maxCoeff();
        spectrumShifted.block(spectrumShifted.rows() / 2 - offsetMin / 2, spectrumShifted.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) /= max;

        cv::Mat image;
        array2image8UC4(spectrumShifted, image);

        cv::ellipse(image, cv::Point2d(spectrumShifted.cols() / 2, spectrumShifted.rows() / 2), cv::Size(MIN_FREQUENCY, MIN_FREQUENCY), 0, 0, 360, cv::Scalar(255, 0, 0, 128));
        cv::ellipse(image, cv::Point2d(spectrumShifted.cols() / 2, spectrumShifted.rows() / 2), cv::Size(MAX_FREQUENCY, MAX_FREQUENCY), 0, 0, 360, cv::Scalar(255, 0, 0, 128));
        
        cv::line(image, cv::Point(image.cols / 2, image.rows / 2), cv::Point(mainPeak1.x(), mainPeak1.y()), cv::Scalar(0, 0, 255, 128));
        cv::line(image, cv::Point(image.cols / 2, image.rows / 2), cv::Point(mainPeak2.x(), mainPeak2.y()), cv::Scalar(0, 255, 0, 128));

        double radius = 3 * sigma;
        if (radius < 1.0) {
            radius = 1.0;
        }
        cv::ellipse(image, cv::Point(mainPeak1.x(), mainPeak1.y()), cv::Size(radius, radius), 0, 0, 360, cv::Scalar(0, 0, 255, 128));
        cv::ellipse(image, cv::Point(mainPeak2.x(), mainPeak2.y()), cv::Size(radius, radius), 0, 0, 360, cv::Scalar(0, 255, 0, 128));

        return image;
    }

    cv::Mat PatternPhase::getFringesImage() {
        cv::Mat image;
        array2image8UC4(spatial, image);

        for (int row = 0; row < image.rows; ++row) {
            uchar *dst = image.ptr<uchar>(row);
            for (int col = 0; col < image.cols; ++col) {
                uchar intensity = (uchar) (40 * std::pow((cos(std::arg(phase1(row, col))) + 1), 2));
                uchar red = dst[4 * col + 2];
                if (intensity > red)
                    dst[4 * col + 2] = intensity;

                intensity = (uchar) (40 * std::pow((cos(std::abs(std::arg(phase2(row, col)))) + 1), 2));
                uchar green = dst[4 * col + 1];
                if (intensity > green)
                    dst[4 * col + 1] = (uchar) intensity;
            }
        }

        cv::rectangle(image, cv::Rect(regressionPlane.getColOffset(), regressionPlane.getRowOffset(), regressionPlane.getNColsCropped(), regressionPlane.getNRowsCropped()), cv::Scalar(255, 0, 0));

        int cx = image.cols / 2;
        int cy = image.rows / 2;
        //        cv::line(image, cv::Point(0, cy), cv::Point(image.cols, cy), cv::Scalar(255, 0, 0));
        //        cv::line(image, cv::Point(cx, 0), cv::Point(cx, image.rows), cv::Scalar(255, 0, 0));
        cv::line(image, cv::Point(cx - 20, cy), cv::Point(cx + 20, cy), cv::Scalar(255, 0, 0));
        cv::line(image, cv::Point(cx, cy - 20), cv::Point(cx, cy + 20), cv::Scalar(255, 0, 0));

        return image;
    }

    bool PatternPhase::peaksFound() {
        return (mainPeak1.z() > MIN_PEAK_POWER && mainPeak2.z() > MIN_PEAK_POWER);
    }

    cv::Mat PatternPhase::getImage() {
        cv::Mat image;
        array2image8UC4(spatial, image);
        return image;
    }

    Eigen::ArrayXXcd & PatternPhase::getSpectrum() {
        return spectrumShifted;
    }

    Eigen::ArrayXXcd & PatternPhase::getSpectrumPeak1() {
        return spectrumFiltered1;
    }

    Eigen::ArrayXXcd & PatternPhase::getSpectrumPeak2() {
        return spectrumFiltered2;
    }

    Eigen::ArrayXXd & PatternPhase::getUnwrappedPhase1() {
        return unwrappedPhase1;
    }

    Eigen::ArrayXXd & PatternPhase::getUnwrappedPhase2() {
        return unwrappedPhase2;
    }

    Eigen::ArrayXXd PatternPhase::getPhase1() {
        return phase1.arg();
    }

    Eigen::ArrayXXd PatternPhase::getPhase2() {
        return phase2.arg();
    }

    PhasePlane PatternPhase::getPlane1() {
        return plane1;
    }

    PhasePlane PatternPhase::getPlane2() {
        return plane2;
    }

    void PatternPhase::setCropFactor(double cropFactor) {
        regressionPlane.setCropFactor(cropFactor);
    }

    void PatternPhase::setSigma(double sigma) {
        this->sigma = sigma;
    }

    double PatternPhase::getSigma() {
        return sigma;
    }

    double PatternPhase::getPixelPeriod() {
        return plane1.getPixelicPeriod();
    }

    int PatternPhase::getNRows() {
        return spectrum.rows();
    }

    int PatternPhase::getNCols() {
        return spectrum.cols();
    }

}