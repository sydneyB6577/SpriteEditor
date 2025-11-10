#include "saveandopen.h"
#include <QFileDialog>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

SaveAndOpen::SaveAndOpen(QObject *parent) : QObject(parent)
{
}

void SaveAndOpen::setFrames(QVector<CanvasFrame*> *framesReference)
{
    frames = framesReference;
}

void SaveAndOpen::saveProject()
{
    if (!frames || frames->isEmpty()) {
        return; // prevents crashing
    }

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Project", "", "JSON file (*.json)");
    if (fileName.isEmpty()) {
        return;
    }

    QJsonObject serializedProject;
    QJsonArray jsonFramesArray;

    for (CanvasFrame *frame : *frames) {
        if (!frame) {
            continue;
        }

        QImage frameImage = frame -> getImage();
        int frameWidth = frameImage.width();
        int frameHeight = frameImage.height();

        QJsonObject jsonFrame;
        jsonFrame["width"] = frameWidth;
        jsonFrame["height"] = frameHeight;
        jsonFrame["penColor"] = static_cast<int>(frame -> getPenColor());
        jsonFrame["isEraserActive"] = frame -> isEraserActive();

        QJsonArray pixelArray;
        for (int y = 0; y < frameHeight; ++y) {
            for (int x = 0; x < frameWidth; ++x) {
                pixelArray.append(static_cast<int>(frameImage.pixel(x, y)));
            }
        }
        jsonFrame["pixels"] = pixelArray;

        jsonFramesArray.append(jsonFrame);
    }
    serializedProject["frames"] = jsonFramesArray;

    QJsonDocument finalJsonDocument(serializedProject);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }
    file.write(finalJsonDocument.toJson(QJsonDocument::Indented));
    file.close();
}

void SaveAndOpen::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Project", "", "Sprite Project (*.json)");
    // TODO: dealing with deserializtion, restore the project content on mainwindow
}
