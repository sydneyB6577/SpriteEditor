#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QStackedWidget>
#include <QVector>
#include "saveandopen.h"
#include "timeline.h"
#include "canvasframe.h"
#include "preview.h"

/* Zijia Xie, Sydney Burt (Team Control-Alt-Elite)
 * November 9, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx, sydneyB6577
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx.git
*/
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SaveAndOpen *saveAndOpen, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addFrame();
    void duplicateCurrentFrame();
    void chooseColor();

private:
    Ui::MainWindow *ui;
    Timeline *timeline;
    QVector<CanvasFrame*> frames;    // store all frame objects
    CanvasFrame *currentCanvas;   // the currently active canvas
    Preview *preview;
};
#endif // MAINWINDOW_H
