#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>



#include <QAction>
#include <QApplication>

// ----- -----
#include <QDir>
#include <QFile>
#include <QFileDialog>

// ----- Q  -----
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QStandardPaths>

// ----- OPENCV IMAGING LIBRARIES -----
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// ----- QCUSTOMPLOT -----
#include "qcustomplot.h"

// ----- MY CLASSES -----
#include "myimage.h"

// ----- PRELOADED CLASSES ----- //may not be necessary?
class QAction;
class QActionGroup;
class QLabel;
class QMenu;

// ----- MAINWINDOW CLASS -----
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // --- FILE MENU SLOTS ---
    void close();
    void open();
    void openDefault();
    void save();
    void saveAs();
    void quit();

    // --- IMAGE MENU SLOTS---
    void imageCopy();
    void imageReset();
    void imageUndo();

    // --- PROCESS MENU SLOTS ---
    void processBitShift();
    void processEqualization();
    void processNegative();
    void processNonLinear();
    void processPositive();
    void processScale();

    // --- HELP MENU SLOTS ---
    void about();
    void aboutQt();
    void help1();

private:
    // --- MAIN WINDOW ---
    Ui::MainWindow *ui;

    QDir inputDirectory;
    QDir outputDirectory;

    // --- MENUS AND GUI ---
    void addActionsToMenu();
    void createActions();
    void createButtonGroups();
    void createDefaultImageComboBox();
    void createMenus();
    void createHistograms();

    // --- GRAPHICS ---
    void clearGraphics();
    void createGraphics();
    void initializeGraphics();
    void loadGraphics(QString filePath);
    void loadGraphicsDefault();
    void setGraphicsToGUI();
    void updateGraphics();

    // --- BUFFER ---
    void forwardBuffer();
    void reverseBuffer();
    void resetBuffer();

    // --- FILE IO ---
    void openCommands();
    void saveCommands(QString filePath);
    void setDefaultImageList();
    QList<QString> defaultImageList;

    // --- MENUS ---
    QMenu *fileMenu;
    QMenu *imageMenu;
    QMenu *processMenu;
    QMenu *helpMenu;

    // --- FILE MENU ACTIONS ---
    QAction *openAction;
    QAction *openDefaultAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *exitAction;

    // --- IMAGE MENU ACTIONS ---
    QAction *imageResetAction;
    QAction *imageUndoAction;
    QAction *imageCopyAction;

    // --- PROCESS MENU ACTIONS ---
    QAction *processPositiveAction;
    QAction *processNegativeAction;
    QAction *procesBitShiftAction;
    QAction *processScaleAction;
    QAction *processNonLinearAction;
    QAction *processEqualizationAction;

    // --- HELP MENU ACTIONS ---
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *help1Action;

    // --- BUTTON GROUPS ---
    QButtonGroup bitShiftButtonGroup;
    QButtonGroup scalingButtonGroup;
    QButtonGroup nonLinearButtonGroup;

    // --- GRAPHICS PIXMAPS ---
    QGraphicsPixmapItem *item;

    // --- GRAPHICS SCENES ---
    QGraphicsScene *inputScene;
    QGraphicsScene *outputScene;
    QGraphicsScene *bufferScene;

    // --- HISTOGRAMS ----
    QCPBars *inputDistributionBars;
    QCPBars *outputDistributionBars;
    QCPBars *outputPDFBars;
    QCPBars *outputCDFBars;
    QCPBars *outputTransformBars;
    QCPBars *outputEqualizedBars;

    // --- MYIMAGE CLASS OBJECTS ---
    MyImage inputImage;
    MyImage bufferImage;
    MyImage outputImage;

};

#endif // MAINWINDOW_H
