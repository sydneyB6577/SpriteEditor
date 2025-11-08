#ifndef SAVEANDOPEN_H
#define SAVEANDOPEN_H

/* Zijia Xie (Team Control-Alt-Elite)
 * November 6, 2025
 * A8: Sprite Editor Implementation
 * GitHub Username: homeofhx
 * GitHub Repository: https://github.com/University-of-Utah-CS3505/a5-sprite-editor-f25-homeofhx.git
*/
#include "qobject.h"
class SaveAndOpen : public QObject
{
    Q_OBJECT

public:
    SaveAndOpen(QObject *parent = nullptr);

    /**
     * @brief saveProject Saves the current project as JSON file
     */
    void saveProject();

    /**
     * @brief openProject Open a new project from hard drive
     */
    void openProject();
};

#endif // SAVEANDOPEN_H
