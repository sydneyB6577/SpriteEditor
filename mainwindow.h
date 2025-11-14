#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>
#include "saveandopen.h"
#include "timeline.h"
#include "canvasframe.h"
#include "preview.h"

/*
 * Zijia Xie, Sydney Burt, Deya Tallman (Team Control-Alt-Elite)
 * November 13, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx, sydneyB6577, deyatallman
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx
*/

// Class that operates the main window where the canvas, toolbar, and timeline reside.
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent = nullptr);
    ~MainWindow();

// Slots for all the buttons.
private slots:
    void slot_addFrame();
    void slot_duplicateCurrentFrame();
    void slot_chooseColor();
    void slot_chooseCanvasSize();
    void slot_restoreFramesFromOpenedProject(QVector<CanvasFrame*> newFrames);
    void slot_deleteFrame();
    void slot_moveFrameLeft();
    void slot_moveFrameRight();
    void slot_rotateCanvasLeft();
    void slot_rotateCanvasRight();
    void slot_resetCanvasOrientation();

private:
    Ui::MainWindow *ui;
    Timeline *timeline;
    QVector<CanvasFrame*> frames; // store all frame objects
    CanvasFrame *currentCanvas; // the currently active canvas
    Preview *preview;
};
#endif // MAINWINDOW_H
