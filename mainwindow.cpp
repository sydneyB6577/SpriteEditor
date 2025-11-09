#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timeline.h"
#include "canvasframe.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QWidget>
#include <QImage>

MainWindow::MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize the timeline and frame list
    timeline = new Timeline(this);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }

    // Add timeline widget to the bottom of the main layout.
    mainLayout->addWidget(timeline);

    // Set up the first frame
    currentCanvas = ui->canvasFrame; // your existing canvas in the UI
    frames.append(currentCanvas);

    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::addFrame);

    connect(ui->penTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::penTool); /// should reset to previous color if eraserTool was clicked before penTool
    /// TODO connect this to UI/add way for UI to change values
    ///connect(ui->penColor, &QPushButton::clicked, this, [this](){canvasFrame->setColor();}); /// should connect the selection of the penColor to a qRgb value to set CanvasFrame's color
    connect(ui->eraserTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::eraseColor); /// set the color of the canvas frame to white, ie. eraser
    // Save & open project.
    connect(ui->saveProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::saveProject);
    connect(ui->openProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::openProject);

    // Connect timeline frame selection
    connect(timeline, &Timeline::frameSelected, this, [this](int index) {
        if (index >= 0 && index < frames.size()) {
            currentCanvas = frames[index];
        }
    });
}

// Adds a frame.
void MainWindow::addFrame()
{
    // 1. Save thumbnail
    QImage currentImage = currentCanvas->getImage();
    timeline->addFrameThumbnail(currentImage);

    // 2. Create new canvas
    CanvasFrame *newFrame = new CanvasFrame(this);
    newFrame->setFixedSize(currentCanvas->size());
    frames.append(newFrame);

    // 3. Replace the widget properly inside the container
    QLayout *layout = ui->canvasFrame->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->canvasFrame);
        ui->canvasFrame->setLayout(layout);
    }

    // Remove old and add new
    QLayoutItem *oldItem;
    while ((oldItem = layout->takeAt(0)) != nullptr) {
        delete oldItem->widget();
        delete oldItem;
    }
    layout->addWidget(newFrame);

    // 4. Update the current canvas pointer
    currentCanvas = newFrame;

    // 5. Connect tools
    connect(ui->penTool, &QPushButton::clicked, newFrame, &CanvasFrame::penTool);
    connect(ui->eraserTool, &QPushButton::clicked, newFrame, &CanvasFrame::eraseColor);
}


MainWindow::~MainWindow()
{
    delete ui;
}
