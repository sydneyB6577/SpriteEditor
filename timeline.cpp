#include "timeline.h"
#include <QPixmap>
#include <QMouseEvent>
#include "clickablelabel.h"

// A class that controls the timeline with the thumbnails of all the canvases.

// The constructor for the timeline.
Timeline::Timeline(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setSpacing(4);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);
}

// Adds a thumbnail on the timeline for a frame.
void Timeline::addFrameThumbnail(const QImage &frameImage)
{
    ClickableLabel *thumb = new ClickableLabel(this);
    thumb->setPixmap(QPixmap::fromImage(frameImage.scaled(64, 64, Qt::KeepAspectRatio)));
    thumb->setFixedSize(70, 70);
    thumb->setStyleSheet("border: 1px solid gray;");
    layout->addWidget(thumb);
    thumbnails.append(thumb);

    // Capture index for click signal.
    int index = thumbnails.size() - 1;
    thumb->installEventFilter(this);
    thumb->setProperty("frameIndex", index);

    // Add handleClicked to each thumbnail.
    connect(thumb, &ClickableLabel::clicked, this, &Timeline::slot_handleThumbnailClicked);
}

// Erases the timeline.
void Timeline::clearTimeline()
{
    qDeleteAll(thumbnails);
    thumbnails.clear();
}

// Selects a thumbnail when it is clicked once.
void Timeline::slot_handleThumbnailClicked()
{
    ClickableLabel *thumb = qobject_cast<ClickableLabel*>(sender());
    if (!thumb)
    {
        return;
    }

    int index = thumb->property("frameIndex").toInt();
    emit frameSelected(index);
    selectedFrameIndex = index;

    // Reset needed for multiple clicks without any actions.
    for (auto thumbnail : thumbnails)
    {
        thumbnail->setStyleSheet("");
    }

    thumbnails[index]->setStyleSheet("border: 2px solid red;");
}

// A helper method that gets the place index of where the thumbnail is in the timeline.
int Timeline::getSelectedFrameIndex() const
{
    return selectedFrameIndex;
}

// A helper method that sets the place index of where the the thumbnail is in the timeline.
void Timeline::updateSelectedFrameIndex(int index)
{
    selectedFrameIndex = index;

    for (auto thumbnail : thumbnails)
    {
        thumbnail->setStyleSheet("");
    }

    thumbnails[selectedFrameIndex]->setStyleSheet("border: 2px solid red;");
}

// A helper method that determines if the timeline has no frames in it.
bool Timeline::isEmpty()
{
    return thumbnails.isEmpty();
}
