// Qt Doodle Drawing Application
// Author: Zaid Alnemer

#ifndef DOODLESHEET_H
#define DOODLESHEET_H

// Including important libraries that we will need to use
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class DoodleSheet : public QWidget {
    // Declares our class as a QObject which is the base class for all Qt objects
    Q_OBJECT // QObjects handle events

public:
    DoodleSheet(QWidget *parent = nullptr);

    // Handles all events
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setInkColour(const QColor &newColour);
    void setInkThickness(int newThickness);

    // Has the image been edited since last save?
    bool isEdited() const { return edited; }
    QColor inkColour() const { return myInkColour; }
    int inkThickness() const { return myInkThickness; }

public slots:
    // Events to handle
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    // Updates the doodle sheet where we are drawing
    void paintEvent(QPaintEvent *event) override;

    // Makes sure the area we are drawing on remains as large as the widget
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    // Will be marked true or false depending on if we have saved after a change
    bool edited;

    // Will be Marked true or false depending on if the user is currently drawing
    bool doodling;

    // Holds the current ink width & colour
    int myInkThickness;
    QColor myInkColour;

    // Stores the image being drawn
    QImage image;

    // Stores the location at the current mouse event
    QPoint lastPoint;
};

#endif
