#include "canvasframe.h"
#include "ui_canvasframe.h"
#include <QPainter>

CanvasFrame::CanvasFrame(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CanvasFrame)
{
    ui->setupUi(this);
    color = qRgb(255, 0, 0); /// red as the default drawing color to start with.
    previousColor = color; /// previousColor used to store a color when we switch from the eraser to pen tool
    img = QImage(imgSizeX, imgSizeY, QImage::Format_RGB32); /// uses imgSizeX/Y so we can change resolution later
    img.setDotsPerMeterX(imgSizeX); /// supposedly setDotsPerMeterX/Y helps correctly lay out the image.
    img.setDotsPerMeterY(imgSizeY);
    img.fill(Qt::white); /// default background white
    updateDisplay();
}

void CanvasFrame::setImage(const QImage &image){
    img = image.copy();
    imgSizeX = img.width();
    imgSizeY = img.height();
    updateDisplay();
    updateDisplay();
}


void CanvasFrame::changeCanvasSize(int x, int y){
    imgSizeX = x;
    imgSizeY = y;

    // Creates a new image with the new dimesions.
    QImage newImg(imgSizeX, imgSizeY, QImage::Format_RGB32);
    newImg.fill(Qt::white);

    img = newImg;

    setFixedSize(imgSizeX * scale, imgSizeY * scale);

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

    if (newX < 0 || newY < 0 || newX >= img.width() || newY >= img.height())
        return;

    img.setPixel(newX, newY, color);
    updateDisplay();
    /// uncomment for debugging
    ///std::cout << "Mouse: (" << x << ", " << y << "); " << "Pixel: (" << newX << ", " << newY << ")" << std::endl;
    /// QImage has a valid() function that can check whether or not the mouse is within the Widget rectangle. Use this to figure out if it should draw using valid().
    //if (img.valid(newX, newY)) {
        //img.setPixel(newX, newY, color);
        //updateDisplay();
    //}
}

void CanvasFrame::setColor(QRgb newColor){
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
    //QLabel* canvas = ui->canvasLabel;
    QPainter painter(&scaled);
    painter.setPen(QPen(Qt::lightGray));
    /// TODO fix the weird way it's being rendered. The lines aren't perfect/look good enough. it should display the accurate amount of pixels no matter the sizex/y or scale.
    for (int x = 0; x <= imgSizeX; x++) {
        painter.drawLine(x * scale, 0, x * scale, imgSizeX * scale);
    }
    for (int y = 0; y <= imgSizeY; y++) {
        painter.drawLine(0, y * scale, imgSizeY * scale, y * scale);
    }
    //QPixmap pixmap = QPixmap::fromImage(scaled);
    //canvas->setPixmap(pixmap);
    //canvas->show();

    ui->canvasLabel->setFixedSize(scaled.size());
    ui->canvasLabel->setPixmap(QPixmap::fromImage(scaled));
    ui->canvasLabel->show();

    setFixedSize(scaled.size());
}

CanvasFrame::~CanvasFrame()
{
    delete ui;
}
