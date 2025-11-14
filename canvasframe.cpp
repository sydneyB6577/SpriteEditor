#include "canvasframe.h"
#include "ui_canvasframe.h"
#include <QPainter>

// A class that controls the canvas that users can draw on.

// The constructor for the canvas.
CanvasFrame::CanvasFrame(QWidget *parent) : QWidget(parent), ui(new Ui::CanvasFrame)
{
    ui->setupUi(this);
    color = QColor(255, 0, 0, 255); // red as the default drawing color to start with.
    previousColor = color; // previousColor used to store a color when we switch from the eraser to pen tool
    img = QImage(imgSizeX, imgSizeY, QImage::Format_ARGB32); // uses imgSizeX/Y so we can change resolution later
    img.setDotsPerMeterX(imgSizeX); // supposedly setDotsPerMeterX/Y helps correctly lay out the image.
    img.setDotsPerMeterY(imgSizeY);
    img.fill(Qt::white); // default background white
    updateDisplay();
}

// Sets the image on the canvas.
void CanvasFrame::setImage(const QImage &image)
{
    img = image.copy();
    imgSizeX = img.width();
    imgSizeY = img.height();
    updateDisplay();
}

// Changes the size of the drawable canvas.
void CanvasFrame::slot_changeCanvasSize(int x, int y)
{

    imgSizeX = x;
    imgSizeY = y;

    float scale = (float)mainDisplaySize / (float)imgSizeX; // float cast to ensure it doesn't do int division

    // Creates a new image with the new dimesions.
    QImage newImg(imgSizeX, imgSizeY, QImage::Format_ARGB32);
    newImg.fill(Qt::white);
    img = newImg;
    setFixedSize(imgSizeX * scale, imgSizeY * scale);
    updateDisplay();
}


// Draws on the canvas when the mouse is clicked and/or held.
void CanvasFrame::mousePressEvent(QMouseEvent *event)
{
    // Take a QMouseEvent position x and y, then call drawOnCanvas to update canvas.
    int x = event->position().x();
    int y = event->position().y();
    slot_drawOnCanvas(x, y);
}

// Draws on the canvas when the mouse is clicked and/or held.
void CanvasFrame::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->position().x();
    int y = event->position().y();
    slot_drawOnCanvas(x, y);
}

// Helper method to set the pixel to a new color when clicked on.
void CanvasFrame::slot_drawOnCanvas(int x, int y)
{
    float scale = (float)mainDisplaySize / (float)imgSizeX; // float cast to ensure it doesn't do int division
    int newX = x / scale;
    int newY = y / scale;

    if(img.valid(newX, newY))
    {
        img.setPixel(newX, newY, color.rgba());
        updateDisplay();
    }
}

// Sets the pen to a color.
void CanvasFrame::slot_setColor(QColor newColor)
{
    penColor = newColor;
    color = penColor;
}

// Activates the pen tool.
void CanvasFrame::slot_penTool()
{
    color = penColor;
}

// Activates the eraser tool.
void CanvasFrame::slot_eraseColor()
{
    color = eraserColor;
}

// Updates the canvas to a new image.
void CanvasFrame::updateDisplay()
{
    QImage scaled = img.scaled(mainDisplaySize, mainDisplaySize, Qt::KeepAspectRatio, Qt::FastTransformation);
    float scale = (float)mainDisplaySize / (float)imgSizeX; // float cast to ensure it doesn't do int division
    QPainter painter(&scaled);
    painter.setPen(QPen(Qt::lightGray));

    for (int x = 0; x <= imgSizeX; x++)
    {
        painter.drawLine(x * scale, 0, x * scale, imgSizeX * scale);
    }
    for (int y = 0; y <= imgSizeY; y++)
    {
        painter.drawLine(0, y * scale, imgSizeY * scale, y * scale);
    }

    ui->canvasLabel->setFixedSize(scaled.size());
    ui->canvasLabel->setPixmap(QPixmap::fromImage(scaled));
    ui->canvasLabel->show();
    setFixedSize(scaled.size());
}

// Destructor for the drawable canvas.
CanvasFrame::~CanvasFrame()
{
    delete ui;
}
