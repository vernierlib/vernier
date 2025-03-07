/* 
 * This file is part of the VERNIER Library.
 *
 * Copyright (c) 2018-2023 CNRS, ENSMM, UFC.
 */

#include "PatternPhase.hpp"

namespace vernier {

    PatternPhase::PatternPhase() {
        this->peaksSearchMethod = 0;
        this->pixelPeriod = 0.0;
        setSigma(3);
    }

    PatternPhase::PatternPhase(int nRows, int nCols) : PatternPhase() {
        resize(nRows, nCols);
    }

    void PatternPhase::resize(int nRows, int nCols) {
        if (nRows != this->getNRows() || nCols != this->getNCols()) {
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
            spectrum.resize(nRows, nCols);
        }
    }

    void PatternPhase::compute(Eigen::ArrayXXd& image) {
        spatial.resize(image.rows(), image.cols());
        spatial.real() = image;
        compute(spatial);
    }

    void PatternPhase::compute(cv::Mat& image) {
        Eigen::ArrayXXd array = image2array(image);
        compute(array);
    }

    void PatternPhase::compute(Eigen::ArrayXXcd& patternArray) {
        resize(patternArray.rows(), patternArray.cols());

        fft.compute(patternArray, spectrum);

        Spectrum::shift(spectrum, spectrumShifted);
        spectrumFiltered1 = spectrumShifted;
        spectrumFiltered2 = spectrumShifted;

        if (pixelPeriod == 0.0) {
            Spectrum::mainPeakHalfPlane(spectrumShifted, mainPeak1, mainPeak2);
        } else {
            switch (peaksSearchMethod) {
                case 0:
                    Spectrum::mainPeakHalfPlane(spectrumShifted, mainPeak1, mainPeak2);
                    break;
                case 1:
                    Spectrum::mainPeak4Search(spectrumShifted, mainPeak1, mainPeak2);
                    break;
                case 2:
                    Spectrum::mainPeakPerimeter(spectrumShifted, mainPeak1, mainPeak2);
                    break;
                default:
                    Spectrum::mainPeakCircle(spectrumShifted, mainPeak1, mainPeak2, pixelPeriod);
                    ;
                    break;
            }
        }

        // Compute first plane phase from peak 1
        gaussianFilter.applyTo(spectrumFiltered1, mainPeak1(1), mainPeak1(0));

        ifft.compute(spectrumFiltered1, phase1);
        Spatial::shift(phase1);

        unwrappedPhase1 = phase1.arg();

        Spatial::quartersUnwrapPhase(unwrappedPhase1);

        plane1 = regressionPlane.compute(unwrappedPhase1);

        this->pixelPeriod = plane1.getPixelicPeriod();

        // Compute second plase from peak 2
        gaussianFilter.applyTo(spectrumFiltered2, mainPeak2(1), mainPeak2(0));
        ifft.compute(spectrumFiltered2, phase2);
        Spatial::shift(phase2);

        unwrappedPhase2 = phase2.arg();

        Spatial::quartersUnwrapPhase(unwrappedPhase2);

        plane2 = regressionPlane.compute(unwrappedPhase2);

#ifndef USE_FFTW
        // plane1.setC(-plane1.getC());   // supprimé le 19/11/2022 quelle différence avec oouda fft ?
        // plane2.setC(-plane2.getC());
#endif 
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

    void PatternPhase::computeWeakPerspective(Eigen::ArrayXXd& patternArray, int& betaSign, int& gammaSign, double approxPixelPeriod) {
        spatial.resize(patternArray.rows(), patternArray.cols());
        spatial.real() = patternArray.array();
        compute(spatial);
    }

    void PatternPhase::computeWeakPerspective(Eigen::ArrayXXcd& patternArray, int& betaSign, int& gammaSign, double approxPixelPeriod) {
        fft.compute(patternArray, spectrum);

        Spectrum::shift(spectrum, spectrumShifted);
        spectrumFiltered1 = spectrumShifted;
        spectrumFiltered2 = spectrumShifted;

        if (approxPixelPeriod != 0) {
            this->pixelPeriod = approxPixelPeriod;
            Spectrum::mainPeakCircle(spectrumShifted, mainPeak1, mainPeak2, approxPixelPeriod);
        } else {
            Spectrum::mainPeakQuarter(spectrumShifted, mainPeak1, mainPeak2);
        }


        // Quarter 1
        gaussianFilter.applyTo(spectrumFiltered1, mainPeak1(1), mainPeak1(0));

        ifft.compute(spectrumFiltered1, phase1);
        Spatial::shift(phase1);

        unwrappedPhase1 = phase1.array().arg();
        Eigen::ArrayXXd phase1Wrapped = unwrappedPhase1;
        Spatial::quartersUnwrapPhase(unwrappedPhase1);


        plane1 = regressionPlane.compute(unwrappedPhase1);

        if (approxPixelPeriod == 0) {
            this->pixelPeriod = plane1.getPixelicPeriod();
        }

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



        // Quarter 2
        gaussianFilter.applyTo(spectrumFiltered2, mainPeak2(1), mainPeak2(0));
        ifft.compute(spectrumFiltered2, phase1);
        Spatial::shift(phase1);

        unwrappedPhase2 = phase1.array().arg();
        Spatial::quartersUnwrapPhase(unwrappedPhase2);

        plane2 = regressionPlane.compute(unwrappedPhase2);

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

    void PatternPhase::computeQRCode(Eigen::ArrayXXcd& patternArray) {

        Eigen::ArrayXXcd meanPattern = patternArray - patternArray.mean();
        fft.compute(meanPattern, spectrum);
        Spectrum::shift(spectrum, spectrumShifted);
        spectrumFiltered1 = spectrumShifted;
        spectrumFiltered2 = spectrumShifted;

        Spectrum::mainPeakQuarter(spectrumShifted, mainPeak1, mainPeak2);


        gaussianFilter.applyTo(spectrumFiltered1, mainPeak1(1), mainPeak1(0));
        ifft.compute(spectrumFiltered1, phase1);
        Spatial::shift(phase1);
        unwrappedPhase1 = phase1.array().arg();

        ////__________
        ////affichage
        //cv::Mat phaseImage;
        //Eigen::MatrixXd intermediaryMatrix;
        //intermediaryMatrix = phase1.matrix();
        //intermediaryMatrix = (intermediaryMatrix + PI * Eigen::MatrixXd::Ones(intermediaryMatrix.rows(), intermediaryMatrix.cols())) / (2 * PI);

        //cv::eigen2cv(intermediaryMatrix, phaseImage);

        //cv::imshow("spectrum inverse fringes direction 1", phaseImage);
        //phaseImage *= 255;
        //cv::imwrite("PhaseImage1.png", phaseImage);
        ////_____________


        Spatial::quartersUnwrapPhase(unwrappedPhase1);
        //        phaseCropped = phase1.block(sideOffset, sideOffset, phase1.rows() - 2 * sideOffset, phase1.cols() - 2 * sideOffset);
        this->plane1 = regressionPlane.compute(unwrappedPhase1);

        gaussianFilter.applyTo(spectrumFiltered2, mainPeak2(1), mainPeak2(0));
        ifft.compute(spectrumFiltered2, phase1);
        Spatial::shift(phase1);
        unwrappedPhase2 = phase1.array().arg();

        //Eigen::MatrixXd intermediaryMatrix;
        //cv::Mat phaseImage;
        //intermediaryMatrix = phase2.matrix();
        //intermediaryMatrix = (intermediaryMatrix + PI * Eigen::MatrixXd::Ones(intermediaryMatrix.rows(), intermediaryMatrix.cols())) / (2 * PI);
        //cv::eigen2cv(intermediaryMatrix, phaseImage);
        //cv::imshow("spectrum inverse fringes direction 2", phaseImage);

        ////_________
        ////affichage
        //intermediaryMatrix = phase2.matrix();
        //intermediaryMatrix = (intermediaryMatrix + PI * Eigen::MatrixXd::Ones(intermediaryMatrix.rows(), intermediaryMatrix.cols())) / (2 * PI);
        //cv::eigen2cv(intermediaryMatrix, phaseImage);

        //cv::imshow("spectrum inverse fringes direction 2", phaseImage);
        //phaseImage *= 255;
        //cv::imwrite("phaseImage2.png", phaseImage);
        ////_________

        Spatial::quartersUnwrapPhase(unwrappedPhase2);
        //        phaseCropped = phase2.block(sideOffset, sideOffset, phase2.rows() - 2 * sideOffset, phase2.cols() - 2 * sideOffset);
        this->plane2 = regressionPlane.compute(unwrappedPhase2);

    }

    double PatternPhase::computeFirst(Eigen::ArrayXXcd& patternArray, double& pixelPeriod) {
        Eigen::ArrayXXcd meanPattern = patternArray - patternArray.mean();
        fft.compute(meanPattern, spectrum);
        Spectrum::shift(spectrum, spectrumShifted);
        spectrumFiltered1 = spectrumShifted;
        Eigen::ArrayXXcd spectrumShift2(patternArray.rows(), patternArray.cols());
        Eigen::ArrayXXcd gaussian2D(patternArray.rows(), patternArray.cols());
        gaussian2D.setConstant(1);
        Eigen::ArrayXXcd Jcriterion(patternArray.rows(), patternArray.cols());

        Spectrum::mainPeakQuarter(spectrumShifted, mainPeak1, mainPeak2);

        //std::cout << "main peak : " << mainPeak1(1) << " ; " << mainPeak1(0) << std::endl;

        double sigma = 0.333;
        spectrumShift2 = spectrumShifted;

        gaussianFilter.setSigma(sigma);
        gaussianFilter.applyTo(gaussian2D, mainPeak1(1), mainPeak1(0));

        gaussian2D *= spectrumShift2(mainPeak1(1), mainPeak1(0));

        gaussian2D = gaussian2D.abs();
        spectrumShift2 = spectrumShift2.abs();

        spectrumShift2 -= gaussian2D;

        Jcriterion = spectrumShift2 * spectrumShift2;

        double meanJPrev = Jcriterion.abs().sum() * 10;

        double meanJ = 0;
        while (true) {
            sigma += 0.001;

            spectrumShift2 = spectrumShifted;
            gaussian2D.setConstant(1);
            gaussianFilter.setSigma(sigma);
            gaussianFilter.applyTo(gaussian2D, mainPeak1(1), mainPeak1(0));

            gaussian2D *= spectrumShift2(mainPeak1(1), mainPeak1(0));

            gaussian2D = gaussian2D.abs();

            spectrumShift2 -= gaussian2D;

            Jcriterion = spectrumShift2 * spectrumShift2;

            meanJ = Jcriterion.abs().sum();

            if (meanJ > meanJPrev) break;
            else {
                meanJPrev = meanJ;
            }

            //std::cout << "mean criterion : " <<  meanJ  << " ; sigma : " << sigma << std::endl;
        }

        gaussianFilter.applyTo(spectrumFiltered1, mainPeak1(1), mainPeak1(0));
        ifft.compute(spectrumFiltered1, phase1);
        Spatial::shift(phase1);
        unwrappedPhase1 = phase1.array().arg();
        Spatial::quartersUnwrapPhase(unwrappedPhase1);
        //        phaseCropped = phase1.block(sideOffset, sideOffset, phase1.rows() - 2 * sideOffset, phase1.cols() - 2 * sideOffset);
        this->plane1 = regressionPlane.compute(unwrappedPhase1);

        pixelPeriod = plane1.getPixelicPeriod();

        return sigma;
    }

    cv::Mat PatternPhase::getPeaksImage() {
        int offsetMin = 10.0;
        double max = spectrumShifted.block(spectrumShifted.rows() / 2 - offsetMin / 2, spectrumShifted.cols() / 2 - offsetMin / 2, offsetMin, offsetMin).abs().maxCoeff();
        spectrumShifted.block(spectrumShifted.rows() / 2 - offsetMin / 2, spectrumShifted.cols() / 2 - offsetMin / 2, offsetMin, offsetMin) /= max;

        cv::Mat image = array2image(spectrumShifted);


        double frequenceColApprox = ((double) spectrumShifted.cols() / (double) pixelPeriod);
        double frequenceRowApprox = ((double) spectrumShifted.rows() / (double) pixelPeriod);
        double frequenceColMin = frequenceColApprox - frequenceColApprox * (sqrt(2) - 1) / 2.0;
        double frequenceRowMin = frequenceRowApprox - frequenceRowApprox * (sqrt(2) - 1) / 2.0;
        double frequenceColMax = frequenceColApprox + frequenceColApprox * (sqrt(2) - 1) / 2.0;
        double frequenceRowMax = frequenceRowApprox + frequenceRowApprox * (sqrt(2) - 1) / 2.0;
        cv::ellipse(image, cv::Point2d(spectrumShifted.cols() / 2, spectrumShifted.rows() / 2), cv::Size(frequenceColMin, frequenceRowMin), 0, 0, 360, cv::Scalar(255, 0, 0, 128));
        cv::ellipse(image, cv::Point2d(spectrumShifted.cols() / 2, spectrumShifted.rows() / 2), cv::Size(frequenceColMax, frequenceRowMax), 0, 0, 360, cv::Scalar(255, 0, 0, 128));

        cv::line(image, cv::Point(image.cols / 2, image.rows / 2), cv::Point(mainPeak1.x(), mainPeak1.y()), cv::Scalar(0, 0, 255, 128));
        cv::line(image, cv::Point(image.cols / 2, image.rows / 2), cv::Point(mainPeak2.x(), mainPeak2.y()), cv::Scalar(0, 255, 0, 128));

        double radius = 3 * gaussianFilter.getSigma();
        if (radius < 1.0) {
            radius = 1.0;
        }
        cv::ellipse(image, cv::Point(mainPeak1.x(), mainPeak1.y()), cv::Size(radius, radius), 0, 0, 360, cv::Scalar(0, 0, 255, 128));
        cv::ellipse(image, cv::Point(mainPeak2.x(), mainPeak2.y()), cv::Size(radius + 1, radius), 0, 0, 360, cv::Scalar(0, 255, 0, 128));

        return image;
    }

    cv::Mat PatternPhase::getFringesImage() {
        cv::Mat image = array2image(spatial);

        for (int row = 0; row < image.rows; ++row) {
            uchar *dst = image.ptr<uchar>(row);
            for (int col = 0; col < image.cols; ++col) {
                uchar intensity = (uchar) (40 * std::powf((cos(std::arg(phase1(row, col))) + 1), 2));
                uchar red = dst[4 * col + 2];
                if (intensity > red)
                    dst[4 * col + 2] = intensity;

                intensity = (uchar) (40 * std::powf((cos(std::abs(std::arg(phase2(row, col)))) + 1), 2));
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
        cv::Mat image = array2image(spatial);
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

    Plane PatternPhase::getPlane1() {
        return plane1;
    }

    Plane PatternPhase::getPlane2() {
        return plane2;
    }

    void PatternPhase::setCropFactor(double cropFactor) {
        regressionPlane.setCropFactor(cropFactor);
    }

    void PatternPhase::setPeaksSearchMethod(int methodNumber) {
        this->peaksSearchMethod = methodNumber;
    }

    int PatternPhase::getPeaksSearchMethod() {
        return peaksSearchMethod;
    }

    void PatternPhase::setSigma(double sigma) {
        gaussianFilter.setSigma(sigma);
    }

    double PatternPhase::getSigma() {
        return gaussianFilter.getSigma();
    }

    void PatternPhase::setPixelPeriod(double periodLengthInPixel) {
        this->peaksSearchMethod = 4;
        this->pixelPeriod = periodLengthInPixel;
    }

    double PatternPhase::getPixelPeriod() {
        return pixelPeriod;
    }

    int PatternPhase::getNRows() {
        return spectrum.rows();
    }

    int PatternPhase::getNCols() {
        return spectrum.cols();
    }

}