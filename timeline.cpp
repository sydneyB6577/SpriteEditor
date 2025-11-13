#include "timeline.h"
#include <QPixmap>
#include <QMouseEvent>
#include "clickablelabel.h".h"

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
    ClickableLabel *thumb = new ClickableLabel(this);
    thumb->setPixmap(QPixmap::fromImage(frameImage.scaled(64, 64, Qt::KeepAspectRatio)));
    thumb->setFixedSize(70, 70);
    thumb->setStyleSheet("border: 1px solid gray;");
    layout->addWidget(thumb);
    thumbnails.append(thumb);

    // Capture index for click signal
    int index = thumbnails.size() - 1;
    thumb->installEventFilter(this);
    thumb->setProperty("frameIndex", index);

    // adds handleClicked to each thumbnail
    connect(thumb, &ClickableLabel::clicked, this, &Timeline::handleThumbnailClicked);
}

void Timeline::clearTimeline()
{
    qDeleteAll(thumbnails);
    thumbnails.clear();
}

void Timeline::handleThumbnailClicked()
{
    ClickableLabel *thumb = qobject_cast<ClickableLabel*>(sender());
    if (!thumb) return;
    int index = thumb->property("frameIndex").toInt();
    emit frameSelected(index);
    selectedFrameIndex = index;
    // qDebug() << index;

    // reset needed for multiple clicks without any actions
    for (auto t : thumbnails) {
        t->setStyleSheet("");
    }
    thumbnails[index]->setStyleSheet("border: 2px solid red; ");
}

int Timeline::getSelectedFrameIndex() const {
    return selectedFrameIndex;
}

void Timeline::updateSelectedFrameIndex(int index) {
    selectedFrameIndex = index;

    for (auto t : thumbnails) {
        t->setStyleSheet("");
    }

    thumbnails[selectedFrameIndex]->setStyleSheet("border: 2px solid red; ");
}

bool Timeline::isEmpty() {
    return thumbnails.isEmpty();
}
