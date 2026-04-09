// Qt Doodle Drawing Application
// Author: Zaid Alnemer

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "doodlesheet.h"

DoodleSheet::DoodleSheet(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StaticContents); // Roots the widget to the top left even if resized

    // Set defaults for tracked variables
    edited = false;
    doodling = false;
    myInkThickness = 1;
    myInkColour = Qt::blue;
}

// Used to load the image and place it in the widget
bool DoodleSheet::openImage(const QString &fileName) {
    QImage loadedImage; // Holds the image

    // If the image wasn't loaded leave this function
    if (!loadedImage.load(fileName)) return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    edited = false;
    update();
    return true;
}

// Save the current drawing/image
bool DoodleSheet::saveImage(const QString &fileName, const char *fileFormat) {
    // Created to hold the image
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        edited = false;
        return true;
    } else {
        return false;
    }
}

// Used to change the ink colour
void DoodleSheet::setInkColour(const QColor &newColour) {
    myInkColour = newColour;
}

// Used to change the ink thickness
void DoodleSheet::setInkThickness(int newThickness) {
    myInkThickness = newThickness;
}

// Function to clear everything
void DoodleSheet::clearImage() {
    image.fill(qRgb(255, 255, 255)); // Colours over everything with white
    edited = true;
    update();
}

/* If a mouse button is pressed check if it was the left button:
 * if so store the current position */
void DoodleSheet::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        doodling = true; // Set that we are currently drawing
    }
}

/* When the mouse moves if the left button is clicked:
 * we call the drawline function which draws a line from the last position to the current */
void DoodleSheet::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && doodling) drawLineTo(event->pos());
}

// If the button is released we set variables to stop drawing
void DoodleSheet::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && doodling) {
        drawLineTo(event->pos());
        doodling = false;
    }
}

// The QPaintEvent is sent to widgets that need to update themselves
void DoodleSheet::paintEvent(QPaintEvent *event) {
    QPainter painter(this); // QPainter provides functions to draw on the widget

    // Returns the rectangle that needs to be updated
    QRect dirtyRect = event->rect();

    // Draws the rectangle where the image needs to be updated
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// Resize the image to slightly larger then the main window to cut down on the need to resize the image
void DoodleSheet::resizeEvent(QResizeEvent *event) {
    if (width() > image.width() || height() > image.height()) {
        int newThickness = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newThickness, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DoodleSheet::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image); // Used to draw on the widget

    // Set the current settings for the ink
    painter.setPen(QPen(myInkColour, myInkThickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Draw a line from the last registered point to the current
    painter.drawLine(lastPoint, endPoint);

    edited = true; // Set that the image hasn't been saved

    int rad = (myInkThickness / 2) + 2;

    // Call to update the rectangular space where we drew
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    // Update the last position where we left off drawing
    lastPoint = endPoint;
}

// When the app is resized create a new image using the changes made to the image
void DoodleSheet::resizeImage(QImage *image, const QSize &newSize) {
    // Check if we need to redraw the image
    if (image->size() == newSize) return;

    // Create a new image to display and fill it with white
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));

    // Draw the image
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

// Print the image
void DoodleSheet::print() {
    // Check for print dialog availability
#if QT_CONFIG(printdialog)

    // Can be used to print
    QPrinter printer(QPrinter::HighResolution);

    // Open printer dialog and print if asked
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
