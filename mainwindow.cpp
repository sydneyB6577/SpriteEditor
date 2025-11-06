#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QWidget>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage img(32, 32, QImage::Format_RGB32);

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            img.setPixel(i, j, (i%2==0 ? qRgb(255, 0, 0) : qRgb(0,255,0)));
        }
    }

    QImage scaled = img.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
    QLabel* canvas = ui->canvasLabel;
    QPixmap pixmap = QPixmap::fromImage(scaled);
    canvas->setPixmap(pixmap);
    canvas->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
