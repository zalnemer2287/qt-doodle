// Qt Doodle Drawing Application
// Author: Zaid Alnemer

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // The main application

    // Create and open the main window
    MainWindow window;
    window.show();

    return app.exec(); // Display the main window
}
