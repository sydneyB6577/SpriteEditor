#ifndef TIMELINE_H
#define TIMELINE_H
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QScrollArea>
#include "clickablelabel.h"


class Timeline : public QWidget
{
    Q_OBJECT

public:
    explicit Timeline(QWidget *parent = nullptr);
    void addFrameThumbnail(const QImage &frameImage);
    void clearTimeline();
    int frameCount() const { return thumbnails.size(); }
    int getSelectedFrameIndex() const;
    void updateSelectedFrameIndex(int index);
    bool isEmpty();

signals:
    void frameSelected(int index);  // emitted when user clicks a thumbnail

private slots:
    void handleThumbnailClicked();

private:
    QHBoxLayout *layout;
    QList<ClickableLabel*> thumbnails;
    int selectedFrameIndex = 0;
};

#endif // TIMELINE_H
