#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "saveandopen.h"

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
