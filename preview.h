#ifndef PREVIEW_H
#define PREVIEW_H
#include <QWidget>
#include "canvasframe.h"
#include <QTimer>
#include <QVector>
#include <QLabel>
#include <QImage>

/* Sydney Burt, Alex Held (Team Control-Alt-Elite)
 * Last Updated November 10, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: sydneyB6577, ironman356
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx.git
*/

class Preview : public QWidget
{

public:
    Preview(QWidget *parent, QLabel *previewLabel);
    void updatePreviewSpeed(int updateRateMillis);
    void updatePreviewFrames(const QVector<CanvasFrame*>& frames);


private:
    QLabel *previewLabel;
    int curFrameIdx;
    QVector<QImage> frameImgs;
    QTimer *timer;
    void nextFrame();
};

#endif // PREVIEW_H
