#include "canvasframe.h"
#include "ui_canvasframe.h"
///#include <iostream>
#include <QPainter>

CanvasFrame::CanvasFrame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CanvasFrame)
{
    ui->setupUi(this);
    color = qRgb(255, 0, 0); /// red as the default drawing color to start with.
    previousColor = color; /// previousColor used to store a color when we switch from the eraser to pen tool
    img = QImage(imgSizeX, imgSizeY, QImage::Format_RGB32); /// uses imgSizeX/Y so we can change resolution later
    img.setDotsPerMeterX(32); /// supposedly setDotsPerMeterX/Y helps correctly lay out the image.
    img.setDotsPerMeterY(32);
    img.fill(Qt::white); /// default background white
    updateDisplay();
}

void CanvasFrame::setImage(const QImage &image){
    img = image;
    updateDisplay();
}

void CanvasFrame::changeCanvasSize(int x, int y){
    /// TODO connect this to UI/add way for UI to change values
    imgSizeX = x;
    imgSizeY = y;
    updateDisplay();
}

void CanvasFrame::changeScale(int newScale){
    /// Default scale is 10, which means a 32x32 image will become a 320x320 image.
    /// TODO
    scale = newScale;
}

void CanvasFrame::mousePressEvent(QMouseEvent *event){
    /// Take a QMouseEvent position x and y, then call drawOnCanvas to update canvas.
    int x = event->position().x();
    int y = event->position().y();
    drawOnCanvas(x, y);
}

void CanvasFrame::mouseMoveEvent(QMouseEvent *event){
    int x = event->position().x();
    int y = event->position().y();
    drawOnCanvas(x, y);
}

void CanvasFrame::drawOnCanvas(int x, int y){
    int newX = x / scale;
    int newY = y / scale;
    /// uncomment for debugging
    ///std::cout << "Mouse: (" << x << ", " << y << "); " << "Pixel: (" << newX << ", " << newY << ")" << std::endl;
    /// QImage has a valid() function that can check whether or not the mouse is within the Widget rectangle. Use this to figure out if it should draw using valid().
    if (img.valid(newX, newY)) {
        img.setPixel(newX, newY, color);
        updateDisplay();
    }
}

void CanvasFrame::setColor(QRgb newColor){
    /// TODO connect this to UI/add way for UI to change values
    penColor = newColor;
    color = penColor;
}

void CanvasFrame::penTool(){
    color = penColor;
}

void CanvasFrame::eraseColor(){
    color = eraserColor;
}

void CanvasFrame::updateDisplay(){
    QImage scaled = img.scaled(imgSizeX * scale, imgSizeY * scale, Qt::KeepAspectRatio, Qt::FastTransformation);
    QLabel* canvas = ui->canvasLabel;
    QPainter painter(&scaled);
    painter.setPen(QPen(Qt::lightGray));
    /// TODO fix the weird way it's being rendered. The lines aren't perfect/look good enough. it should display the accurate amount of pixels no matter the sizex/y or scale.
    for (int x = 0; x <= imgSizeX; x++) {
        painter.drawLine(x * scale, 0, x * scale, imgSizeX * scale);
    }
    for (int y = 0; y <= imgSizeY; y++) {
        painter.drawLine(0, y * scale, imgSizeY * scale, y * scale);
    }
    QPixmap pixmap = QPixmap::fromImage(scaled);
    canvas->setPixmap(pixmap);
    canvas->show();
}

CanvasFrame::~CanvasFrame()
{
    delete ui;
}
