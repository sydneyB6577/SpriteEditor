#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "saveandopen.h"
#include "canvasframe.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QList>

/* Zijia Xie (Team Control-Alt-Elite)
 * November 7, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx
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

private:
    Ui::MainWindow *ui;
    QList<CanvasFrame*> frames; // stores all frames
    QWidget *frameStripWidget; // container for thumbnails
    QHBoxLayout *frameStripLayout; // layout for thumbnails
    CanvasFrame *currentCanvas; // current active canvas
};
#endif // MAINWINDOW_H
