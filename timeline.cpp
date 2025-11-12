#include "timeline.h"
#include <QPixmap>
#include <QMouseEvent>

Timeline::Timeline(QWidget *parent)
    : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(4);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);
}

void Timeline::addFrameThumbnail(const QImage &frameImage)
{
    QLabel *thumb = new QLabel(this);
    thumb->setPixmap(QPixmap::fromImage(frameImage.scaled(64, 64, Qt::KeepAspectRatio)));
    thumb->setFixedSize(70, 70);
    thumb->setStyleSheet("border: 1px solid gray;");
    layout->addWidget(thumb);
    thumbnails.append(thumb);

    // Capture index for click signal
    int index = thumbnails.size() - 1;
    thumb->installEventFilter(this);
    thumb->setProperty("frameIndex", index);
}

void Timeline::clearTimeline()
{
    qDeleteAll(thumbnails);
    thumbnails.clear();
}

void Timeline::handleThumbnailClicked()
{
    QLabel *thumb = qobject_cast<QLabel*>(sender());
    if (!thumb) return;
    int index = thumb->property("frameIndex").toInt();
    emit frameSelected(index);
}

int Timeline::getSelectedFrameIndex() const {
    return selectedFrameIndex;
}

void Timeline::updateSelectedFrameIndex(int index) {
    selectedFrameIndex = index;
}
