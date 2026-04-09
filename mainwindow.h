// Qt Doodle Drawing Application
// Author: Zaid Alnemer

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Including important libraries that we will need to use
#include <QList>
#include <QMainWindow>

// DoodleSheet used to ink up the image
class DoodleSheet;

class MainWindow : public QMainWindow {
    // Declares our class as a QObject which is the base class for all Qt objects
    Q_OBJECT // QObjects handle events

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override; // Function used to close an event

// The events that can be triggered
private slots:
    void open();
    void save();
    void inkColour();
    void inkThickness();
    void about();
    void directions();

private:
    // Will tie user actions to functions
    void createActions();
    void createMenus();

    // Will check if changes have occurred since last save
    bool wasChanged();

    // Opens the Save dialog and saves files
    bool saveFile(const QByteArray &fileFormat);

    // What we'll paint on
    DoodleSheet *doodleSheet;

    // The menu widgets
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    // All the actions that can occur
    QAction *openAct;
    QList<QAction *> saveAsActs;
    QAction *exitAct;
    QAction *inkColourAct;
    QAction *inkThicknessAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *directionsAct;
};

#endif
