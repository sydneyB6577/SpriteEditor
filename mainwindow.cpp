#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QImage>

MainWindow::MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->penTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::penTool); /// should reset to previous color if eraserTool was clicked before penTool
    /// TODO connect this to UI/add way for UI to change values
    ///connect(ui->penColor, &QPushButton::clicked, this, [this](){canvasFrame->setColor();}); /// should connect the selection of the penColor to a qRgb value to set CanvasFrame's color
    connect(ui->eraserTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::eraseColor); /// set the color of the canvas frame to white, ie. eraser
    // Save & open project
    connect(ui->saveProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::saveProject);
    connect(ui->openProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::openProject);
}

MainWindow::~MainWindow()
{
    delete ui;
}
