#ifndef TIMELINE_H
#define TIMELINE_H
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QScrollArea>
#include "clickablelabel.h"

/*
 * Sydney Burt (Team Control-Alt-Elite)
 * November 13, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: sydneyB6577
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx
*/
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
    void slot_handleThumbnailClicked();

private:
    QHBoxLayout *layout;
    QList<ClickableLabel*> thumbnails;
    int selectedFrameIndex = 0;
};

#endif // TIMELINE_H
