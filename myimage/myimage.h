#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <iostream>
#include <math.h>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class MyImage
{

public:
    // --- IMAGE DATA TYPE SETTINGS ---
    static const int intensityColorMap = CV_8UC1; // color mapping for all image
    static const uint16_t numberBins = 256; // data type size
    static const uint8_t maxBin = 255; // data type size

    // --- CONSTRUCTOR / DESTRUCTOR ---
    MyImage(QString input);
    ~MyImage();

    // --- INITIALIZATION ---
    void setImageFromPath(std::string image_path);
    void setImageMatchZero(MyImage input);
    void setImageToZero(uint32_t rows, uint32_t cols, int type);
    void setImageToDefault(QString filePath);
    void setTitle(QString input);

    // --- GET IMAGE MATRIX INFO ---
    uint32_t getCols();
    uint32_t getRows();
    uint64_t getSize();
    int getType();
    uint8_t getIntensity(uint32_t row, uint32_t col);

    // --- HISTOGRAM ---
    QVector<double> intensityBins;
    QVector<double> intensityDistribution;
    QVector<double> intensityPDF;
    QVector<double> intensityCDF;
    QVector<double> intensityTransform;
    QVector<double> intensityEqualized;

    void resetIntensityHistograms();
    void setIntensityHistograms();
    void buildIntensityBins();
    void buildIntensityDistribution();
    void buildIntensityPDF();
    void buildIntensityCDF();
    void buildIntensityTransform();
    void buildIntensityEqualized();

    // --- IMAGE PROCESSING FUNCTIONS ---
    QVector<double> intensityCalculation;

    void buildIntensityCalculation(MyImage input, uint8_t toolSwitch, double value);
    void setIntensityCalculation(MyImage input);
    void rebinIntensityCalculation(int minBin, int maxBin);
    void processBaseLog(MyImage input, double base);
    void processBitShiftLeft(MyImage input, int numberBits);
    void processBitShiftRight(MyImage input, int numberBits);
    void processEqualize(MyImage input);
    void processExponential(MyImage input);
    void processNaturalLog(MyImage input);
    void processNegative(MyImage input);
    void processPositive(MyImage input);
    void processPowerLaw(MyImage input, double gamma);
    void processScaleDown(MyImage input, double scalingFactor);
    void processScaleUp(MyImage input, double scalingFactor);

    // --- IMAGE OUTPUT ---
    QImage getQImage();
    void saveImageToPNG(QString outputPath);

private:
    // --- OBJECT TITLE ---
    QString qTitle; // title of image matrix

    // --- IMAGE DIMENSIONS ---
    cv::Mat image; // openCV matrix file containing image data

    // --- IMAGE STATISTICS ---
    double intensityMin; // minimum intensity value in image matrix
    double intensityMax; // maximum intensity value in image matrix

};

#endif // MYIMAGE_H
