#include "preview.h"

// A class that controls the animation preview.

// The constructor for the animation preview.
Preview::Preview(QWidget *parent, QLabel *previewLabel) : QWidget(parent), previewLabel(previewLabel)
{
    curFrameIdx = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Preview::nextFrame);
    timer->setInterval(1000); // this rate doesn't matter, gets immediately overwritten by updatePreviewSpeed
    timer->start();
}

// Updates the speed of the preview based on the value given by the user.
void Preview::updatePreviewSpeed(int updateRateMillis)
{
    timer->setInterval(updateRateMillis);
}

// Updates the frames looped through in the animation .
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

    timer->start();
}

// A helper method to move to the next frame in the list.
void Preview::nextFrame()
{
    if ( frameImgs.isEmpty())
    {
        return;
    }

    // Check needed for frame deleteion when preview is at end of cycle to prevent out of index
    if (curFrameIdx >= frameImgs.size())
    {
        curFrameIdx = 0;
    }

    QPixmap pixmap = QPixmap::fromImage(frameImgs[curFrameIdx]).scaled(previewLabel->size(), Qt::KeepAspectRatio);
    previewLabel->setPixmap(pixmap);
    curFrameIdx = (curFrameIdx+1) % frameImgs.size();
}
