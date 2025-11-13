#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "timeline.h"
#include "canvasframe.h"
#include "preview.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QImage>
#include <QSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QColorDialog>

static int currentRotationAngle = 0;

MainWindow::MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Make the scroll area’s widget the container for thumbnails.
    QWidget *scrollWidget = new QWidget();
    scrollWidget->setLayout(new QHBoxLayout());
    scrollWidget->layout()->setSpacing(4);
    scrollWidget->layout()->setContentsMargins(4, 4, 4, 4);

    ui->scrollArea_2->setWidget(scrollWidget);
    ui->scrollArea_2->setWidgetResizable(true);

    // Create timeline inside scroll area.
    timeline = new Timeline(scrollWidget);
    scrollWidget->layout()->addWidget(timeline);

    // Set up the first frame.
    currentCanvas = ui->canvasFrame; // existing canvas in the UI
    frames.append(currentCanvas);

    // Preview the setup.
    preview = new Preview(this, ui->previewArea);
    preview->updatePreviewSpeed(ui->animationSpeedSpinBox->value());
    connect(ui->animationSpeedSpinBox, &QSpinBox::valueChanged, this, [this](int newSpeed){ preview->updatePreviewSpeed(newSpeed); });
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::addFrame);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::deleteFrame);
    connect(ui->duplicateFrameButton, &QPushButton::clicked, this, &MainWindow::duplicateCurrentFrame);

    connect(ui->penTool, &QPushButton::clicked, this, [this](){
        if (currentCanvas) currentCanvas->penTool();
    });
    connect(ui->eraserTool, &QPushButton::clicked, this, [this](){
        if (currentCanvas) currentCanvas->eraseColor();
    });

    connect(ui->penColor,&QPushButton::clicked, this, &MainWindow::chooseColor);

    // Choose the canvas size.
    connect(ui->newProject, &QPushButton::clicked, this, &MainWindow::chooseCanvasSize);

    // Save & open project.
    connect(ui->saveProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::saveProject);
    connect(ui->openProject, &QPushButton::clicked, saveAndOpen, &SaveAndOpen::openProject);

    // Connect timeline frame selection
    connect(timeline, &Timeline::frameSelected, this, [this](int index) {
        if (index >= 0 && index < frames.size()) {
            currentCanvas = frames[index];
        }
    });

    // Gives saveAndOpen a pointer to `frames` so it can access all frames during saving/loading.
    saveAndOpen -> accessFrames(&frames);

    // Restores all frames when open a project.
    connect(saveAndOpen, &SaveAndOpen::projectLoaded, this, &MainWindow::restoreFramesFromOpenedProject);

    // Connect move frame left/right buttons.
    connect(ui->moveFrameLeft, &QPushButton::clicked, this, &MainWindow::moveFrameLeft);
    connect(ui->moveFrameRight, &QPushButton::clicked, this, &MainWindow::moveFrameRight);

    // Rotate the canvas.
    connect(ui->rotateLeft, &QPushButton::clicked, this, &MainWindow::rotateCanvasLeft);
    connect(ui->rotateRight, &QPushButton::clicked, this, &MainWindow::rotateCanvasRight);
    connect(ui->resetCanvasOrientation, &QPushButton::clicked, this, &MainWindow::resetCanvasOrientation);
}

// Adds a frame.
void MainWindow::addFrame()
{
    // Update preview with newly finished frame.
    preview->updatePreviewFrames(frames);

    // Save thumbnail of the current canvas.
    QImage currentImage = currentCanvas->getImage();
    timeline->addFrameThumbnail(currentImage);

    // Create a new CanvasFrame.
    CanvasFrame *newFrame = new CanvasFrame(ui->canvasFrame);

    // Make it fill the container exactly.
    newFrame->changeCanvasSize(ui->canvasFrame->getCanvasSizeX(), ui->canvasFrame->getCanvasSizeY());

    // Replace the old canvas in the container.
    QLayout *layout = ui->canvasFrame->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->canvasFrame);
        layout->setContentsMargins(0, 0, 0, 0); // no spacing/padding
        layout->setSpacing(0);
        ui->canvasFrame->setLayout(layout);
    }

    // Remove old canvas.
    QLayoutItem *oldItem;
    while ((oldItem = layout->takeAt(0)) != nullptr) {
        QWidget *w = oldItem->widget();
        if (w) {
            w->hide();
        }
        delete oldItem;
    }

    // Add new canvas to the container.
    layout->addWidget(newFrame);

    // Update current canvas pointer.
    currentCanvas = newFrame;
    frames.append(newFrame);
    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline -> addFrameThumbnail(frame -> getImage());
    }

    // Connect tools to the new canvas.
    connect(ui->penTool, &QPushButton::clicked, newFrame, &CanvasFrame::penTool);
    connect(ui->eraserTool, &QPushButton::clicked, newFrame, &CanvasFrame::eraseColor);
    connect(ui->resetCanvasOrientation, &QPushButton::clicked, this, &MainWindow::resetCanvasOrientation);
}

void MainWindow::deleteFrame()
{
    int selectedFrameIndex = timeline->getSelectedFrameIndex();

    if (timeline->isEmpty() || selectedFrameIndex <= 0 || selectedFrameIndex >= frames.size()) {
        return; // prevents crash
    }

    // Delete the selected frame
    CanvasFrame* frameToDelete = frames.takeAt(selectedFrameIndex);
    delete frameToDelete;

    // Refresh the timeline
    timeline -> clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline -> addFrameThumbnail(frame -> getImage());
    }

    // Replace canvas image with next frame; empty canvas if no frames left
    if (!frames.isEmpty()) {
        int nextFrameIndex = qMin(selectedFrameIndex, frames.size()-1);
        currentCanvas = frames[nextFrameIndex];

        QLayout* layout = ui -> canvasFrame -> layout();
        if (!layout) {
            layout = new QVBoxLayout(ui -> canvasFrame);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            ui -> canvasFrame -> setLayout(layout);
        }

        QLayoutItem* oldItem;
        while ((oldItem = layout -> takeAt(0)) != nullptr) {
            QWidget* w = oldItem -> widget();
            if (w) {
                w -> hide();
            }
            delete oldItem;
        }

        layout->addWidget(currentCanvas);
    } else {
        QLayout* layout = ui->canvasFrame->layout();
        if (!layout) {
            layout = new QVBoxLayout(ui -> canvasFrame);
            layout -> setContentsMargins(0, 0, 0, 0);
            layout -> setSpacing(0);
            ui->canvasFrame->setLayout(layout);
        }

        QLayoutItem* oldItem;
        while ((oldItem = layout -> takeAt(0)) != nullptr) {
            QWidget* w = oldItem -> widget();
            if (w) {
                w -> hide();
            }
            delete oldItem;
        }

        currentCanvas = new CanvasFrame(ui -> canvasFrame);
        currentCanvas -> setFixedSize(ui -> canvasFrame -> size());
        layout -> addWidget(currentCanvas);
    }

    preview->updatePreviewFrames(frames);
}

void MainWindow::duplicateCurrentFrame()
{
    // Update preview with newly finished frame.
    preview->updatePreviewFrames(frames);

    if (!currentCanvas) return;

    // Save thumbnail of current canvas.
    QImage currentImage = currentCanvas->getImage();
    timeline->addFrameThumbnail(currentImage);

    // Create new CanvasFrame with the same parent (container).
    CanvasFrame* newFrame = new CanvasFrame(this);

    // Copy the image from the current canvas.
    QImage copiedImage = currentCanvas->getImage().copy();
    newFrame->changeCanvasSize(copiedImage.width(), copiedImage.height());
    // Copy pen/eraser state.
    newFrame->setColor(currentCanvas->getPenColor());
    if (currentCanvas->isEraserActive()) {
        newFrame->eraseColor();
    } else {
        newFrame->penTool();
    }
    // Fill newFrame with copied image.
    QPainter p(&copiedImage);
    newFrame->setImage(currentCanvas->getImage());  // direct assignment

    // Add to container layout.
    QLayout* layout = ui->canvasFrame->layout();
    if (!layout) {
        layout = new QVBoxLayout(ui->canvasFrame);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        ui->canvasFrame->setLayout(layout);
    }

    // Remove old canvas.
    QLayoutItem* oldItem;
    while ((oldItem = layout->takeAt(0)) != nullptr) {
        QWidget* w = oldItem->widget();
        if (w) w->hide(); // keep the old one in memory (frames vector)
        delete oldItem;
    }

    layout->addWidget(newFrame);

    // Update currentCanvas pointer and frame list.
    currentCanvas = newFrame;
    frames.append(newFrame);

    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline -> addFrameThumbnail(frame -> getImage());
    }

    // Connect the tools. (need to connect the pen color tool too?)
    connect(ui->penTool, &QPushButton::clicked, newFrame, &CanvasFrame::penTool);
    connect(ui->eraserTool, &QPushButton::clicked, newFrame, &CanvasFrame::eraseColor);
}

void MainWindow::chooseColor()
{
    if (!currentCanvas) {
        return; // prevent crash
    }

    QColor currentPenColor = currentCanvas -> getPenColor();
    QColor newPenColor = QColorDialog::getColor(currentPenColor, this, "Choose pen color");

    if (newPenColor.isValid()) {
        currentCanvas -> setColor(newPenColor);
        currentCanvas -> penTool();
    }
}

void MainWindow::chooseCanvasSize()
{
    if(frames.size() > 1) {
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Close the program first", "There are frames already in the timeline. Close the program and repoen it to create a new project", QMessageBox::Ok);

        if(reply == QMessageBox::Ok)
        {
            return;
        }
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Choose canvas size (1-64)");

    QFormLayout *canvasLayout = new QFormLayout(&dlg);
    QSpinBox *cSpin = new QSpinBox(&dlg);

    cSpin->setRange(1,64);

    int currentWidth = currentCanvas->getCanvasSizeX();
    cSpin->setValue(currentWidth);

    canvasLayout->addRow("Size: ",cSpin);

    QDialogButtonBox *canvasButtons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal,&dlg );
    canvasLayout->addRow(canvasButtons);

    QObject::connect(canvasButtons, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    QObject::connect(canvasButtons, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        currentCanvas->changeCanvasSize(cSpin->value(), cSpin->value());
        currentCanvas->penTool();
    }

}

void MainWindow::restoreFramesFromOpenedProject(QVector<CanvasFrame*> newFrames)
{
    frames = newFrames;
    if (!frames.isEmpty()) {
        currentCanvas = frames.first();
    }

    timeline->clearTimeline();
    for (CanvasFrame *frame : frames) {
        timeline -> addFrameThumbnail(frame -> getImage());
    }

    preview -> updatePreviewFrames(frames);
}

void MainWindow::moveFrameLeft()
{
    int selectedFrameIndex = timeline -> getSelectedFrameIndex();
    if (selectedFrameIndex <= 0 || selectedFrameIndex >= frames.size()) {
        return; // prevent crash; reaches boundary
    }

    frames.swapItemsAt(selectedFrameIndex, selectedFrameIndex-1);
    currentCanvas = frames[selectedFrameIndex-1];

    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline -> addFrameThumbnail(frame -> getImage());
    }

    timeline -> updateSelectedFrameIndex(selectedFrameIndex-1);
    preview -> updatePreviewFrames(frames);
}

void MainWindow::moveFrameRight()
{
    int selectedFrameIndex = timeline -> getSelectedFrameIndex();
    if (selectedFrameIndex < 0 || selectedFrameIndex >= frames.size()-1) {
        return; // prevent crash; reaches boundary
    }

    frames.swapItemsAt(selectedFrameIndex, selectedFrameIndex+1);
    currentCanvas = frames[selectedFrameIndex+1];

    timeline -> clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline->addFrameThumbnail(frame -> getImage());
    }

    timeline -> updateSelectedFrameIndex(selectedFrameIndex+1);
    preview -> updatePreviewFrames(frames);
}

void MainWindow::rotateCanvasLeft()
{
    if (!currentCanvas) return;

    currentRotationAngle -= 90; // rotate counter-clockwise
    if (currentRotationAngle <= -360) currentRotationAngle = 0;

    QImage image = currentCanvas->getImage();
    QTransform transform;
    transform.rotate(-90);
    QImage rotated = image.transformed(transform);

    currentCanvas->setImage(rotated);
    currentCanvas->update();

    // Update the current thumbnail in the timeline if needed
    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline->addFrameThumbnail(frame->getImage());
    }

    preview->updatePreviewFrames(frames);
}

void MainWindow::rotateCanvasRight()
{
    if (!currentCanvas) return;

    currentRotationAngle += 90; // rotate clockwise
    if (currentRotationAngle >= 360) currentRotationAngle = 0;

    QImage image = currentCanvas->getImage();
    QTransform transform;
    transform.rotate(90);
    QImage rotated = image.transformed(transform);

    currentCanvas->setImage(rotated);
    currentCanvas->update();

    // Update the current thumbnail in the timeline if needed
    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline->addFrameThumbnail(frame->getImage());
    }

    preview->updatePreviewFrames(frames);
}

void MainWindow::resetCanvasOrientation()
{
    if (!currentCanvas) return;
    if (currentRotationAngle == 0) return; // already normal

    QImage image = currentCanvas->getImage();
    QTransform transform;
    transform.rotate(-currentRotationAngle); // undo previous rotations
    QImage reset = image.transformed(transform);

    currentCanvas->setImage(reset);
    currentCanvas->update();

    currentRotationAngle = 0; // reset tracker

    // Update the current thumbnail in the timeline if needed
    timeline->clearTimeline();
    for (CanvasFrame* frame : frames) {
        timeline->addFrameThumbnail(frame->getImage());
    }

    preview->updatePreviewFrames(frames);
}

MainWindow::~MainWindow()
{
    delete ui;
}
