#include "myimage.h"

// ----- CONSTRUCTOR / DESTRUCTOR -----------------------------------------------------------------
MyImage::MyImage(QString input)
{
    setTitle(input);
}

MyImage::~MyImage()
{
    // destructor call goes here
}

// ----- INITIALIZATION ---------------------------------------------------------------------------
void MyImage::setImageFromPath(std::string image_path)
{
    cv::destroyAllWindows();
    image.release();

    image = cv::imread(image_path, intensityColorMap);
    setIntensityHistograms();
}

void MyImage::setImageMatchZero(MyImage input)
{
    setImageToZero(input.getRows(),input.getCols(),input.getType());
}

void MyImage::setImageToZero(uint32_t rows, uint32_t cols, int type)
{
    image.release();
    image = cv::Mat::zeros(rows, cols, type);
    setIntensityHistograms();
}

void MyImage::setImageToDefault(QString filePath)
{
    QFile file(filePath);
    cv::destroyAllWindows();
    image.release();

    if(file.open(QIODevice::ReadOnly))
    {
        qint64 imageFileSize = file.size();
        std::vector<uchar> buf(imageFileSize);

        file.read((char*)buf.data(), imageFileSize);

        image = cv::imdecode(buf, intensityColorMap);
    }
    setIntensityHistograms();
}

void MyImage::setTitle(QString input)
{
    qTitle = input;
}

// ------ GET IMAGE MATRIX INFO -------------------------------------------------------------------
uint32_t MyImage::getCols()
{
    return image.cols;
}

uint32_t MyImage::getRows()
{
    return image.rows;
}

uint64_t MyImage::getSize()
{
    return image.rows * image.cols;
}

int MyImage::getType()
{
    return image.type();
}

uint8_t MyImage::getIntensity(uint32_t row, uint32_t col)
{
    return image.at<uchar>(row,col);
}

// ------ HISTOGRAM -------------------------------------------------------------------------------
void MyImage::resetIntensityHistograms()
{
    intensityBins.clear();
    intensityDistribution.clear();
    intensityPDF.clear();
    intensityCDF.clear();
    intensityTransform.clear();
    intensityEqualized.clear();

    intensityBins.fill(0,numberBins);
    intensityDistribution.fill(0,numberBins);
    intensityPDF.fill(0,numberBins);
    intensityCDF.fill(0,numberBins);
    intensityTransform.fill(0,numberBins);
    intensityEqualized.fill(0,numberBins);
}

void MyImage::setIntensityHistograms()
{
    resetIntensityHistograms();

    buildIntensityBins();
    buildIntensityDistribution();
    buildIntensityPDF();
    buildIntensityCDF();
    buildIntensityTransform();
    buildIntensityEqualized();
}

void MyImage::buildIntensityBins()
{
    for(uint16_t i=0; i<numberBins; i++)
    {
        intensityBins.replace(i,i);
    }
}

void MyImage::buildIntensityDistribution()
{
    int i = -1;

    for(uint16_t row=0; row < image.rows; row++) {
        for (uint16_t col=0; col < image.cols; col++)
        {
            i = getIntensity(row,col);
            intensityDistribution.replace(i,intensityDistribution.at(i)+1);
        }
    }
}

void MyImage::buildIntensityPDF()
{
    double tmp;
    for(uint16_t i=0; i<numberBins; i++)
    {
        tmp = intensityDistribution.at(i) / getSize();
        intensityPDF.replace(i,tmp);
    }
}

void MyImage::buildIntensityCDF()
{
    double tmp = 0;
    for(uint16_t i=0; i<numberBins; i++)
    {
        tmp = tmp + intensityPDF.at(i);
        intensityCDF.replace(i,tmp);
    }
}

void MyImage::buildIntensityTransform()
{
    double tmp = 0;
    for(uint16_t i=0; i<numberBins; i++)
    {
        tmp = intensityCDF.at(i) * maxBin;
        intensityTransform.replace(i,tmp);
    }
}

void MyImage::buildIntensityEqualized()
{
    int i = -1;
    for(uint16_t row=0; row < image.rows; row++) {
        for (uint16_t col=0; col < image.cols; col++)
        {
            i = intensityTransform.at(getIntensity(row,col));
            intensityEqualized.replace(i,intensityEqualized.at(i)+1);
        }
    }
}

// -----  IMAGE PROCESSING FUNCTIONS --------------------------------------------------------------
void MyImage::buildIntensityCalculation(MyImage input, uint8_t toolSwitch, double value)
{
    intensityCalculation.clear();
    intensityCalculation.fill(0,numberBins);

    intensityMin = maxBin;
    intensityMax = 0;

    double tmp = 0;

    for (int i=0; i<numberBins; i++)
    {
        switch (toolSwitch){
        case 1:
            tmp = i;
            break;
        case 2:
            tmp = maxBin - i;
            break;
        case 3:
            tmp = i << (unsigned int)(value);
            break;
        case 4:
            tmp = i >> (unsigned int)(value);
            break;
        case 5:
            tmp = i * value;
            break;
        case 6:
            tmp = i / value;
            break;
        case 7:
            tmp = exp(1.0*i/255.0);
            break;
        case 8:
            tmp = log(1.0+i);
            break;
        case 9:
            tmp = pow(i,value);
            break;
        case 10:
            tmp = log(i+1)/log(value+1);
            break;
        case 11:
            tmp = input.intensityTransform.at(i);
            break;
        default:
            tmp = i;
            break;
        }

        if (tmp>intensityMax)
        {
            intensityMax = tmp;
        }

        if (tmp<intensityMin)
        {
            intensityMin = tmp;
        }

        intensityCalculation.replace(i,tmp);
    }

    rebinIntensityCalculation(0,maxBin);
    setIntensityCalculation(input);
    setIntensityHistograms();
}

void MyImage::rebinIntensityCalculation(int tmpMIN, int tmpMAX)
{
    int tmpRange = abs(tmpMAX - tmpMIN);
    double tmp = 0;

    for(int i=0; i<=tmpRange; i++)
    {
        tmp = (tmpMAX / intensityMax) * (intensityCalculation.at(i) - intensityMin + tmpMIN);
        intensityCalculation.replace(i,tmp);
    }
}

void MyImage::setIntensityCalculation(MyImage input)
{
    for (uint16_t row=0; row < image.rows; row++) {
        for (uint16_t col=0; col < image.cols; col++)
        {
            image.at<uchar>(row,col) = round(intensityCalculation.at(input.getIntensity(row,col)));
        }
    }
}

void MyImage::processPositive(MyImage input)
{
    buildIntensityCalculation(input,1,0);
}

void MyImage::processNegative(MyImage input)
{
    buildIntensityCalculation(input,2,0);
}

void MyImage::processBitShiftLeft(MyImage input, int numberBits)
{
    buildIntensityCalculation(input,3,numberBits);
}

void MyImage::processBitShiftRight(MyImage input, int numberBits)
{
    buildIntensityCalculation(input,4,numberBits);
}

void MyImage::processScaleUp(MyImage input, double scalingFactor)
{
    buildIntensityCalculation(input,5,scalingFactor);
}

void MyImage::processScaleDown(MyImage input, double scalingFactor)
{
    buildIntensityCalculation(input,6,scalingFactor);
}

void MyImage::processExponential(MyImage input)
{
    buildIntensityCalculation(input,7,0);
}

void MyImage::processNaturalLog(MyImage input)
{
    buildIntensityCalculation(input,8,0);
}

void MyImage::processPowerLaw(MyImage input, double gamma)
{
    buildIntensityCalculation(input,9,gamma);
}

void MyImage::processBaseLog(MyImage input, double base)
{
    buildIntensityCalculation(input,10,base);
}

void MyImage::processEqualize(MyImage input)
{
    buildIntensityCalculation(input,11,0);
}

// ----- IMAGE OUTPUT -----------------------------------------------------------------------------
QImage MyImage::getQImage()
{
    QImage dest((const uchar *) image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
    dest.bits(); // enforce deep copy, see documentation
    return dest;
}

void MyImage::saveImageToPNG(QString outputPath)
{
    std::vector<int> compression_parameters;
    compression_parameters.push_back(cv::IMWRITE_PNG_COMPRESSION); // Image file type
    compression_parameters.push_back(3); // Compression rate

    QString fileType = ".png";
    QString filePath = outputPath + "_" + qTitle + fileType;

    cv::imwrite(filePath.toStdString(), image, compression_parameters);
}
