#ifndef CANVASFRAME_H
#define CANVASFRAME_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>

namespace Ui {
class CanvasFrame;
}

class CanvasFrame : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasFrame(QWidget *parent = nullptr);
    void setImage(const QImage &image);
    QImage getImage() const { return img; } // returns the canvas as an image
    QRgb getPenColor() const { return penColor; }
    int getCanvasSizeX() { return imgSizeX; } // returns the canvas width
    int getCanvasSizeY() { return imgSizeY; } // returns the canvas height
    bool isEraserActive() const { return color == eraserColor; }
    ~CanvasFrame();

public slots:
    void drawOnCanvas(int, int); // changes the pixel at x,y with the CanvasFrame's internal color.
    void setColor(QRgb); // when called change the color of the CanvasFrame.
    void penTool(); // used after eraseButton is pressed to reset back to previous color
    void eraseColor(); // sets color to default white, rgb(255, 255, 255)
    void changeCanvasSize(int, int); /// TODO
    void changeScale(int); /// TODO. Changing the scale should change the amount of pixels for the image, as well as the lines drawn.

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::CanvasFrame *ui;
    QImage img;
    QLabel* canvas;
    QPixmap pixmap;
    QRgb penColor = qRgb(255,0,0);  // pen color (starts as red)
    QRgb color; // current color used for drawing
    QRgb eraserColor = qRgb(255,255,255);
    QRgb previousColor;
    int imgSizeX = 32;
    int imgSizeY = 32;
    int scale = 10; // need to scale it by 10x to display properly
    void updateDisplay();
};

#endif // CANVASFRAME_H
