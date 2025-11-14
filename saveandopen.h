#ifndef SAVEANDOPEN_H
#define SAVEANDOPEN_H
#include "qobject.h"
#include <QObject>
#include <QVector>
#include "canvasframe.h"

/*
 * Zijia Xie (Team Control-Alt-Elite)
 * November 13, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx
*/

// A class that controls the buttons to save a proejct and open a previously created project.
class SaveAndOpen : public QObject
{
    Q_OBJECT

public:
    SaveAndOpen(QObject *parent = nullptr);
    void accessFrames(QVector<CanvasFrame*> *framesReference);

public slots:
    void slot_saveProject();
    void slot_openProject();

signals:
    void projectLoaded(QVector<CanvasFrame*> newFrames);

private:
    QVector<CanvasFrame*> *frames = nullptr;
};

#endif // SAVEANDOPEN_H
