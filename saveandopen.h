#ifndef SAVEANDOPEN_H
#define SAVEANDOPEN_H
#include "qobject.h"
#include <QObject>
#include <QVector>
#include "canvasframe.h"

/*
 * Zijia Xie (Team Control-Alt-Elite)
 * November 6, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx.git
*/

/**
 * @brief The SaveAndOpen class Mechanics for save and open this sprite project
 */
class SaveAndOpen : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief SaveAndOpen Default constructor
     * @param parent nullptr
     */
    SaveAndOpen(QObject *parent = nullptr);

    /**
     * @brief setFrames Helper function to give this SaveAndOpen class a reference to access the canvas frames to save/load the sprite project
     * @param framesReference MainWindow's canvas frames' pointer to be dereferenced
     */
    void setFrames(QVector<CanvasFrame*> *framesReference);

public slots:
    /**
     * @brief saveProject Saves the current project as JSON file
     */
    void saveProject();

    /**
     * @brief openProject Open a new project from hard drive
     */
    void openProject();

private:
    /**
     * @brief frames Initial pointer to canvas frame
     */
    QVector<CanvasFrame*> *frames = nullptr;
};

#endif // SAVEANDOPEN_H
