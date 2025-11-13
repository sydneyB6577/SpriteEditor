#ifndef CANVASFRAME_H
#define CANVASFRAME_H
/*
 * Graham Taggart (Team Control-Alt-Elite)
 * November 13, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: grahamtaggart
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx.git
*/
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
    QColor getPenColor() const { return penColor; }
    int getCanvasSizeX() { return imgSizeX; } // returns the canvas width
    int getCanvasSizeY() { return imgSizeY; } // returns the canvas height
    bool isEraserActive() const { return color == eraserColor; }
    ~CanvasFrame();

public slots:
    void drawOnCanvas(int, int); // changes the pixel at x,y with the CanvasFrame's internal color.
    void setColor(QColor); // when called change the color of the CanvasFrame.
    void penTool(); // used after eraseButton is pressed to reset back to previous color
    void eraseColor(); // sets color to default white, rgb(255, 255, 255)
    void changeCanvasSize(int, int);
    void changeScale(int);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::CanvasFrame *ui;
    QImage img;
    QLabel* canvas;
    QPixmap pixmap;
    QColor penColor = qRgba(255,0,0, 255);  // pen color (starts as red)
    QColor color; // current color used for drawing
    QColor eraserColor = qRgba(255,255,255,255);
    QColor previousColor;
    int imgSizeX = 32;
    int imgSizeY = 32;
    int scale = 10; // need to scale it by 10x to display properly
    void updateDisplay();
};

#endif // CANVASFRAME_H
