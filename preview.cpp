#include "preview.h"

Preview::Preview(QWidget *parent, QLabel *previewLabel) : QWidget(parent), previewLabel(previewLabel)
{
    curFrameIdx = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Preview::nextFrame);
    timer->setInterval(1000); // this rate doesn't matter, gets immediately overwritten by updatePreviewSpeed
    timer->start();
}

void Preview::updatePreviewSpeed(int updateRateMillis)
{
    timer->setInterval(updateRateMillis);
}

void Preview::updatePreviewFrames(const QVector<CanvasFrame*>& frames)
{
    timer->stop();
    frameImgs.clear();

    for (CanvasFrame* frame : frames)
    {
        QImage img = frame->getImage();

        if (!img.isNull())
        {
            frameImgs.append(img);
        }
    }

    curFrameIdx = 0;
    timer->start();
}

void Preview::nextFrame()
{
    if ( frameImgs.isEmpty())
    {
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(frameImgs[curFrameIdx]).scaled(previewLabel->size(), Qt::KeepAspectRatio);
    previewLabel->setPixmap(pixmap);
    curFrameIdx = (curFrameIdx+1) % frameImgs.size();
}
