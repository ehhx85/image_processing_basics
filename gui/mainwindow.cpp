#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdlib>

// ----- MAIN WINDOW ------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    inputImage(MyImage("input")),
    bufferImage(MyImage("buffer")),
    outputImage(MyImage("output"))
{
    ui->setupUi(this);

    item = new QGraphicsPixmapItem();

    createActions();
    createMenus();
    addActionsToMenu();

    createGraphics();
    setGraphicsToGUI();

    createButtonGroups();
    createDefaultImageComboBox();

    createHistograms();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ----- MENU AND GUI -----------------------------------------------------------------------------
void MainWindow::createActions()
{
    // --- File Menu Actions ---
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    openDefaultAction = new QAction(tr("Open &Default Image"), this);
    openDefaultAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_O));
    connect(openDefaultAction, SIGNAL(triggered()), this, SLOT(openDefault()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAction = new QAction(tr("&Close"), this);
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));

    // --- Image Menu Actions ---
    imageUndoAction = new QAction(tr("&Undo to Buffer"), this);
    imageUndoAction->setShortcut(QKeySequence::Undo);
    connect(imageUndoAction, SIGNAL(triggered()), this, SLOT(imageUndo()));

    imageCopyAction = new QAction(tr("&Copy to Buffer"), this);
    imageCopyAction->setShortcut(QKeySequence::Copy);
    connect(imageCopyAction, SIGNAL(triggered()), this, SLOT(imageCopy()));

    imageResetAction = new QAction(tr("&Reset"), this);
    imageResetAction->setShortcut(QKeySequence::Refresh);
    connect(imageResetAction, SIGNAL(triggered()), this, SLOT(imageReset()));

    // --- Process Menu Actions ---
    processPositiveAction = new QAction(tr("&Positive"), this);
    processPositiveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    processPositiveAction->setStatusTip(tr("edit status tip"));
    connect(processPositiveAction, SIGNAL(triggered()), this, SLOT(processPositive()));

    processNegativeAction = new QAction(tr("&Negative"), this);
    processNegativeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    connect(processNegativeAction, SIGNAL(triggered()), this, SLOT(processNegative()));

    procesBitShiftAction = new QAction(tr("&Bit Shift"), this);
    procesBitShiftAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    connect(procesBitShiftAction, SIGNAL(triggered()), this, SLOT(processBitShift()));

    processScaleAction = new QAction(tr("&Scaling Function"), this);
    processScaleAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_4));
    connect(processScaleAction, SIGNAL(triggered()), this, SLOT(processScale()));

    processNonLinearAction = new QAction(tr("&NonLinear Function"), this);
    processNonLinearAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_5));
    connect(processNonLinearAction, SIGNAL(triggered()), this, SLOT(processNonLinear()));

    processEqualizationAction = new QAction(tr("&Equalize"), this);
    processEqualizationAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    connect(processEqualizationAction, SIGNAL(triggered()), this, SLOT(processEqualization()));

    // --- Help Menu Actions ---
    aboutAction = new QAction(tr("&About This Application"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction = new QAction(tr("&About Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    help1Action = new QAction(tr("&Help Function"), this);
    connect(help1Action, SIGNAL(triggered()), this, SLOT(help1()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    imageMenu = menuBar()->addMenu(tr("&Image"));
    processMenu = menuBar()->addMenu(tr("&Process"));
    helpMenu = menuBar()->addMenu(tr("&Help"));
}

void MainWindow::addActionsToMenu()
{
    fileMenu->addAction(openAction);
    fileMenu->addAction(openDefaultAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);

    imageMenu->addAction(imageUndoAction);
    imageMenu->addAction(imageCopyAction);
    imageMenu->addAction(imageResetAction);

    processMenu->addAction(processPositiveAction);
    processMenu->addAction(processNegativeAction);
    processMenu->addAction(procesBitShiftAction);
    processMenu->addAction(processScaleAction);
    processMenu->addAction(processNonLinearAction);
    processMenu->addAction(processEqualizationAction);

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(help1Action);
}

void MainWindow::createButtonGroups()
{
    bitShiftButtonGroup.addButton(ui->buttonRadio_bitShiftLeft, 0);
    bitShiftButtonGroup.addButton(ui->buttonRadio_bitShiftRight, 1);

    scalingButtonGroup.addButton(ui->radioButton_scaleUp, 0);
    scalingButtonGroup.addButton(ui->radioButton_scaleDown, 1);

    nonLinearButtonGroup.addButton(ui->buttonRadio_exponential, 0);
    nonLinearButtonGroup.addButton(ui->buttonRadio_naturalLog, 1);
    nonLinearButtonGroup.addButton(ui->buttonRadio_power, 2);
    nonLinearButtonGroup.addButton(ui->buttonRadio_logarithm, 3);
}

void MainWindow::createDefaultImageComboBox()
{
    setDefaultImageList();
    ui->comboBoxDefaultFileSelection->addItems(defaultImageList);
}

void MainWindow::createHistograms()
{
    inputDistributionBars = new QCPBars(ui->qCustomPlotHistogram1->xAxis, ui->qCustomPlotHistogram1->yAxis);
    outputDistributionBars = new QCPBars(ui->qCustomPlotHistogram2->xAxis, ui->qCustomPlotHistogram2->yAxis);
    outputPDFBars = new QCPBars(ui->qCustomPlotHistogram3->xAxis, ui->qCustomPlotHistogram3->yAxis);
    outputCDFBars = new QCPBars(ui->qCustomPlotHistogram4->xAxis, ui->qCustomPlotHistogram4->yAxis);
    outputTransformBars = new QCPBars(ui->qCustomPlotHistogram5->xAxis, ui->qCustomPlotHistogram5->yAxis);
    outputEqualizedBars = new QCPBars(ui->qCustomPlotHistogram6->xAxis, ui->qCustomPlotHistogram6->yAxis);

    inputDistributionBars->setName("Original Histogram");
    outputDistributionBars->setName("Current Histogram");
    outputPDFBars->setName("PDF Histogram");
    outputCDFBars->setName("CDF Histogram");
    outputTransformBars->setName("Transformation Histogram");
    outputEqualizedBars->setName("Equalized Histogram");

    inputDistributionBars->setPen(QColor("#000000"));
    outputDistributionBars->setPen(QColor("#000000"));
    outputPDFBars->setPen(QColor("#000000"));
    outputCDFBars->setPen(QColor("#000000"));
    outputTransformBars->setPen(QColor("#000000"));
    outputEqualizedBars->setPen(QColor("#000000"));

    ui->qCustomPlotHistogram1->xAxis->setRange(0,256);
    ui->qCustomPlotHistogram2->xAxis->setRange(0,256);
    ui->qCustomPlotHistogram3->xAxis->setRange(0,256);
    ui->qCustomPlotHistogram4->xAxis->setRange(0,256);
    ui->qCustomPlotHistogram5->xAxis->setRange(0,256);
    ui->qCustomPlotHistogram6->xAxis->setRange(0,256);
}

// ----- GRAPHICS ---------------------------------------------------------------------------------
void MainWindow::clearGraphics()
{
    inputScene->clear();
    bufferScene->clear();
    outputScene->clear();
}

void MainWindow::createGraphics()
{
    inputScene = new QGraphicsScene(this);
    bufferScene = new QGraphicsScene(this);
    outputScene = new QGraphicsScene(this);
}

void MainWindow::initializeGraphics()
{
    outputImage.setImageMatchZero(inputImage);
    bufferImage.setImageMatchZero(inputImage);
}

void MainWindow::loadGraphics(QString filePath)
{
    inputImage.setImageFromPath(filePath.toStdString());
}

void MainWindow::loadGraphicsDefault()
{
    inputImage.setImageToDefault(QString(":/") + ui->comboBoxDefaultFileSelection->currentText());
}

void MainWindow::setGraphicsToGUI()
{
    ui->graphicsView_image_input->setScene(inputScene);
    ui->graphicsView_image_buffer->setScene(bufferScene);
    ui->graphicsView_image_output->setScene(outputScene);
}

void MainWindow::updateGraphics()
{
    clearGraphics();

    inputScene->addPixmap(QPixmap::fromImage(inputImage.getQImage()));
    bufferScene->addPixmap(QPixmap::fromImage(bufferImage.getQImage()));
    outputScene->addPixmap(QPixmap::fromImage(outputImage.getQImage()));

    inputDistributionBars->setData(inputImage.intensityBins, inputImage.intensityDistribution);
    outputDistributionBars->setData(outputImage.intensityBins, outputImage.intensityDistribution);
    outputPDFBars->setData(outputImage.intensityBins, outputImage.intensityPDF);
    outputCDFBars->setData(outputImage.intensityBins, outputImage.intensityCDF);
    outputTransformBars->setData(outputImage.intensityBins, outputImage.intensityTransform);
    outputEqualizedBars->setData(outputImage.intensityBins, outputImage.intensityEqualized);

    ui->qCustomPlotHistogram1->yAxis->rescale();
    ui->qCustomPlotHistogram2->yAxis->rescale();
    ui->qCustomPlotHistogram3->yAxis->rescale();
    ui->qCustomPlotHistogram4->yAxis->rescale();
    ui->qCustomPlotHistogram5->yAxis->rescale();
    ui->qCustomPlotHistogram6->yAxis->rescale();

    ui->qCustomPlotHistogram1->replot();
    ui->qCustomPlotHistogram2->replot();
    ui->qCustomPlotHistogram3->replot();
    ui->qCustomPlotHistogram4->replot();
    ui->qCustomPlotHistogram5->replot();
    ui->qCustomPlotHistogram6->replot();
 }

// ----- BUFFER -----------------------------------------------------------------------------------
void MainWindow::forwardBuffer()
{
    bufferImage.processPositive(outputImage);
}

void MainWindow::reverseBuffer()
{
    outputImage.processPositive(bufferImage);
}

void MainWindow::resetBuffer()
{
    bufferImage.processPositive(inputImage);
}

// ----- FILE MENU SLOTS --------------------------------------------------------------------------
void MainWindow::open()
{
    inputDirectory = QFileDialog::getOpenFileName(this,
        tr("Select Image File to Process"),
        QDir::homePath(),
        tr("Image Files (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"));

    if(inputDirectory.path().isNull())
    {
        return;
    }

    loadGraphics(inputDirectory.path());
    openCommands();
}

void MainWindow::openDefault()
{
    loadGraphicsDefault();
    openCommands();
}

void MainWindow::openCommands()
{
    initializeGraphics();
    resetBuffer();
    updateGraphics();
}

void MainWindow::save()
{
    outputDirectory = QDir::home();
    QString defaultName = "_tmp";
    saveCommands(outputDirectory.path() + QDir::toNativeSeparators("/") + defaultName);
}

void MainWindow::saveAs()
{
    outputDirectory = QFileDialog::getSaveFileName(this,
        tr("Save image file as"),
        QDir::homePath(),
        tr("(.png)"));

    saveCommands(outputDirectory.path());
}

void MainWindow::saveCommands(QString filePath)
{
    if(filePath.isNull())
    {
        return;
    }

    // TODO : check for empty data
    inputImage.saveImageToPNG(filePath);
    outputImage.saveImageToPNG(filePath);
    bufferImage.saveImageToPNG(filePath);
}

void MainWindow::close()
{
    clearGraphics();
}

void MainWindow::quit()
{
    QApplication::quit();
}

// ----- IMAGE MENU SLOTS -------------------------------------------------------------------------
void MainWindow::imageReset()
{
    initializeGraphics();
    resetBuffer();
    updateGraphics();
}

void MainWindow::imageUndo()
{
    reverseBuffer();
    updateGraphics();
}

void MainWindow::imageCopy()
{
    forwardBuffer();
    updateGraphics();
}

// ----- PROCESS MENU SLOTS -----------------------------------------------------------------------
void MainWindow::processPositive()
{
    outputImage.processPositive(bufferImage);
    updateGraphics();
}

void MainWindow::processNegative()
{
    outputImage.processNegative(bufferImage);
    updateGraphics();
}

void MainWindow::processBitShift()
{
    switch(bitShiftButtonGroup.checkedId()) {
    case 0:
        outputImage.processBitShiftLeft(bufferImage,ui->spinBox_bitShift->value());
        break;
    case 1:
        outputImage.processBitShiftRight(bufferImage,ui->spinBox_bitShift->value());
        break;
    default:
        break;
    }

    updateGraphics();
}

void MainWindow::processScale()
{

    switch (scalingButtonGroup.checkedId()) {
    case 0:
        outputImage.processScaleUp(bufferImage,ui->spinBox_scalingFactor->value());
        break;
    case 1:
        outputImage.processScaleDown(bufferImage,ui->spinBox_scalingFactor->value());
        break;
    default:
        break;
    }

    updateGraphics();
}

void MainWindow::processNonLinear()
{
    switch (nonLinearButtonGroup.checkedId()) {
    case 0:
        outputImage.processExponential(bufferImage);
        break;
    case 1:
        outputImage.processNaturalLog(bufferImage);
        break;
    case 2:
        outputImage.processPowerLaw(bufferImage,ui->spinBox_nonLinear->value());
        break;
    case 3:
        outputImage.processBaseLog(bufferImage,ui->spinBox_nonLinear->value());
        break;
    default:
        break;
    }

    updateGraphics();
}

void MainWindow::processEqualization()
{
    outputImage.processEqualize(outputImage);
    updateGraphics();
}

// ----- HELP MENU SLOTS -----------------------------------------------------------------------
void MainWindow::about()
{
    // TODO: add a description about this software
}

void MainWindow::aboutQt()
{
    // Qt auto generated About Page
}

void MainWindow::help1()
{
    // TODO: add some generic help menu item
}

void MainWindow::setDefaultImageList()
{
    defaultImageList.append("apple.jpg");
    defaultImageList.append("astronaut.jpg");
    defaultImageList.append("chart_blue.bmp");
    defaultImageList.append("chart_color.bmp");
    defaultImageList.append("chart_gray.bmp");
    defaultImageList.append("chart_green.bmp");
    defaultImageList.append("chart_red.bmp");
    defaultImageList.append("elephants.jpg");
    defaultImageList.append("field.jpg");
    defaultImageList.append("fountain.jpg");
    defaultImageList.append("lady.jpg");
    defaultImageList.append("Lenna.png");
    defaultImageList.append("Lenna_old.jpeg");
    defaultImageList.append("octopus.jpg");
    defaultImageList.append("pollen.jpg");
    defaultImageList.append("rectangle.bmp");
    defaultImageList.append("rectangle.png");
    defaultImageList.append("rectangle.tif");
    defaultImageList.append("rectangle51.png");
    defaultImageList.append("rose.jpg");
    defaultImageList.append("snails.jpg");
    defaultImageList.append("strawberries.bmp");
    defaultImageList.append("strawberries.jpg");
    defaultImageList.append("strawberries.png");
    defaultImageList.append("test_pattern.png");
    defaultImageList.append("test_pattern.tif");
    defaultImageList.append("woman.bmp");
    defaultImageList.append("woman.png");
    defaultImageList.append("woman.tif");
}
