#include "clickablelabel.h"

// A class that allows for the labels to be clicked.

// Constructor for the clickable label.
ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
{
}

ClickableLabel::~ClickableLabel() {}

// Activates when the mouse is pressed/clicked.
void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
