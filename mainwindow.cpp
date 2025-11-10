#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timeline.h"
#include "canvasframe.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QImage>

MainWindow::MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Make the scroll area’s widget the container for thumbnails
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setLayout(new QHBoxLayout());
    scrollWidget->layout()->setSpacing(4);
    scrollWidget->layout()->setContentsMargins(4, 4, 4, 4);

    ui->scrollArea_2->setWidget(scrollWidget);
    ui->scrollArea_2->setWidgetResizable(true);

    // Create timeline inside scroll area
    timeline = new Timeline(scrollWidget);
    scrollWidget->layout()->addWidget(timeline);

    // Set up the first frame
    currentCanvas = ui->canvasFrame; // your existing canvas in the UI
    frames.append(currentCanvas);

    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::addFrame);
    connect(ui->duplicateFrameButton, &QPushButton::clicked, this, &MainWindow::duplicateCurrentFrame);

    connect(ui->penTool, &QPushButton::clicked, this, [this](){
        if (currentCanvas) currentCanvas->penTool();
    });
    connect(ui->eraserTool, &QPushButton::clicked, this, [this](){
        if (currentCanvas) currentCanvas->eraseColor();
    });

    //connect(ui->penTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::penTool); /// should reset to previous color if eraserTool was clicked before penTool
    /// TODO connect this to UI/add way for UI to change values
    ///connect(ui->penColor, &QPushButton::clicked, this, [this](){canvasFrame->setColor();}); /// should connect the selection of the penColor to a qRgb value to set CanvasFrame's color
    //connect(ui->eraserTool, &QPushButton::clicked, ui->canvasFrame, &CanvasFrame::eraseColor); /// set the color of the canvas frame to white, ie. eraser
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
    // 1. Save thumbnail of the current canvas
    QImage currentImage = currentCanvas->getImage();
    timeline->addFrameThumbnail(currentImage);

    // 2. Create a new CanvasFrame
    CanvasFrame *newFrame = new CanvasFrame(ui->canvasFrame);

    // Make it fill the container exactly
    newFrame->setFixedSize(ui->canvasFrame->size());

    // 3. Replace the old canvas in the container
    QLayout *layout = ui->canvasFrame->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->canvasFrame);
        layout->setContentsMargins(0, 0, 0, 0); // no spacing/padding
        layout->setSpacing(0);
        ui->canvasFrame->setLayout(layout);
    }

    // Remove old canvas
    QLayoutItem *oldItem;
    while ((oldItem = layout->takeAt(0)) != nullptr) {
        QWidget *w = oldItem->widget();
        if (w) {
            w->hide();
            delete w; // delete old canvas
        }
        delete oldItem;
    }

    // Add new canvas to the container
    layout->addWidget(newFrame);

    // 4. Update current canvas pointer
    currentCanvas = newFrame;
    frames.append(newFrame);

    // 5. Connect tools to the new canvas
    connect(ui->penTool, &QPushButton::clicked, newFrame, &CanvasFrame::penTool);
    connect(ui->eraserTool, &QPushButton::clicked, newFrame, &CanvasFrame::eraseColor);
}

void MainWindow::duplicateCurrentFrame()
{
    if (!currentCanvas) return;

    // Save thumbnail of current canvas
    QImage currentImage = currentCanvas->getImage();
    timeline->addFrameThumbnail(currentImage);

    // Create new CanvasFrame with the same parent (container)
    CanvasFrame* newFrame = new CanvasFrame(ui->canvasFrame);

    // Copy the image from the current canvas
    QImage copiedImage = currentCanvas->getImage();
    newFrame->changeCanvasSize(copiedImage.width(), copiedImage.height());
    // Copy pen/eraser state
    newFrame->setColor(currentCanvas->getPenColor());
    if (currentCanvas->isEraserActive()) {
        newFrame->eraseColor();
    } else {
        newFrame->penTool();
    }
    // Fill newFrame with copied image
    QPainter p(&copiedImage);
    newFrame->setImage(currentCanvas->getImage());  // direct assignment

    // Add to container layout
    QLayout* layout = ui->canvasFrame->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->canvasFrame);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        ui->canvasFrame->setLayout(layout);
    }

    // Remove old canvas
    QLayoutItem* oldItem;
    while ((oldItem = layout->takeAt(0)) != nullptr) {
        QWidget* w = oldItem->widget();
        if (w) w->hide(); // we keep the old one in memory (frames vector)
        delete oldItem;
    }

    layout->addWidget(newFrame);

    // Update currentCanvas pointer and frame list
    currentCanvas = newFrame;
    frames.append(newFrame);

    // Connect tools
    connect(ui->penTool, &QPushButton::clicked, newFrame, &CanvasFrame::penTool);
    connect(ui->eraserTool, &QPushButton::clicked, newFrame, &CanvasFrame::eraseColor);
}


MainWindow::~MainWindow()
{
    delete ui;
}
