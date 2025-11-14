#include "saveandopen.h"
#include <QFileDialog>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

// A class that controls the buttons to save a proejct and open a previously created project.

// The constructor for save and open.
SaveAndOpen::SaveAndOpen(QObject *parent) : QObject(parent)
{
}

// A helper method to access the frames in a project.
void SaveAndOpen::accessFrames(QVector<CanvasFrame*> *framesReference)
{
    frames = framesReference;
}

// Saves the file on the computer of the user.
void SaveAndOpen::slot_saveProject()
{
    if (!frames || frames -> isEmpty())
    {
        return; // prevents crashing
    }

    // Choose where to save the JSON file
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Project", "", "JSON file (*.json)");
    if (fileName.isEmpty())
    {
        return;
    }

    QJsonObject serializedProject;
    QJsonArray jsonFramesArray;

    // Serialize all frames in this project.
    for (CanvasFrame *frame : *frames)
    {
        if (!frame)
        {
            continue;
        }

        QImage frameImage = frame -> getImage();
        int frameWidth = frameImage.width();
        int frameHeight = frameImage.height();

        QJsonObject jsonFrame;
        jsonFrame["width"] = frameWidth;
        jsonFrame["height"] = frameHeight;
        jsonFrame["penColor"] = static_cast<int>(frame -> getPenColor().rgba());
        jsonFrame["isEraserActive"] = frame -> isEraserActive();

        QJsonArray pixelArray;
        for (int y = 0; y < frameHeight; y++)
        {
            for (int x = 0; x < frameWidth; x++)
            {
                pixelArray.append(static_cast<int>(frameImage.pixel(x, y)));
            }
        }
        jsonFrame["pixels"] = pixelArray;

        jsonFramesArray.append(jsonFrame);
    }
    serializedProject["frames"] = jsonFramesArray;

    // Save the JSON file to the direction specified above.
    QJsonDocument finalJsonDocument(serializedProject);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    file.write(finalJsonDocument.toJson(QJsonDocument::Indented));
    file.close();
}

// Opens a previously created project saved on the computer of the user.
void SaveAndOpen::slot_openProject()
{
    // Open a JSON file.
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Project", "", "JSON file (*.json)");
    if (fileName.isEmpty()) return;

    // Process & expand the opened JSON file, ready for serialization.
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray projectRawData = file.readAll();
    file.close();

    QJsonDocument projectJsonData = QJsonDocument::fromJson(projectRawData);
    if (!projectJsonData.isObject())
    {
        return;
    }
    QJsonObject projectJsonObject = projectJsonData.object();
    QJsonArray frames = projectJsonObject["frames"].toArray();
    QVector<CanvasFrame*> newFrames;

    // Deserialize all frames in the JSON file.
    for (QJsonValue frame : frames)
    {
        QJsonObject jsonFrame = frame.toObject();

        int width = jsonFrame["width"].toInt();
        int height = jsonFrame["height"].toInt();
        QImage frameImage(width, height, QImage::Format_ARGB32);
        QJsonArray pixels = jsonFrame["pixels"].toArray();

        int index = 0;
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (index < pixels.size())
                {
                    frameImage.setPixel(x, y, static_cast<QRgb>(pixels[index].toInt()));
                    ++index;
                }
            }
        }

        CanvasFrame *newFrame = new CanvasFrame();

        if (jsonFrame.contains("penColor"))
        {
            newFrame -> slot_setColor(static_cast<QRgb>(jsonFrame["penColor"].toInt()));
        }

        if (jsonFrame.contains("isEraserActive") && jsonFrame["isEraserActive"].toBool())
        {
            newFrame -> slot_eraseColor();
        }
        else
        {
            newFrame -> slot_penTool();
        }

        newFrame -> slot_changeCanvasSize(width, height);
        newFrame -> setImage(frameImage);
        newFrames.append(newFrame);
    }

    // Restore the project in the editor.
    emit projectLoaded(newFrames);
}
