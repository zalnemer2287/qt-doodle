// Qt Doodle Drawing Application
// Author: Zaid Alnemer

#include <QtWidgets>

#include "mainwindow.h"
#include "doodlesheet.h"

// MainWindow constructor
MainWindow::MainWindow() {
    doodleSheet = new DoodleSheet; // Creating the DoodleSheet widget on which we will be drawing on
    setCentralWidget(doodleSheet); // Making the doodleSheet the central widget

    // Create actions and menus
    createActions();
    createMenus();

    setWindowTitle(tr("Doodle")); // Setting the window title

    setWindowIcon(QIcon(":/Icon.png")); // Setting the window icon

    resize(1000, 750); // Sizing the app

    // Set the icons for the menu actions
    saveAsMenu->setIcon((QIcon(":/SaveAs.png")));
    printAct->setIcon(QIcon(":/Print.png"));
    openAct->setIcon(QIcon(":/Open.png"));
    clearScreenAct->setIcon(QIcon(":/ClearScreen.png"));
    exitAct->setIcon(QIcon(":/Exit.png"));
    inkColourAct->setIcon((QIcon(":/ColourPalette.png")));
    inkThicknessAct->setIcon(QIcon(":/InkDrop.png"));
    aboutAct->setIcon(QIcon(":/About.png"));
    aboutQtAct->setIcon(QIcon(":/Qt.png"));
    directionsAct->setIcon(QIcon(":/Help.png"));
}

// User tried to close the app
void MainWindow::closeEvent(QCloseEvent *event) {
    // If they try to close wasChanged() returns true
    // If no changes have been made and the app closes
    if (wasChanged()) {
        event->accept();
    } else {

        // If there have been changes ignore the event
        event->ignore();
    }
}

// Check if the current image has been changed and then open a dialog to open a file
void MainWindow::open() {
    // Check if changes have been made since last save
    if (wasChanged()) { // wasChanged() returns true if no changes have been made

        // Get the file to open from a dialog
        // tr sets the window title to Open File
        // QDir opens the current dirctory
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());

        // If we have a file name load the image and place it in the doodle sheet
        if (!fileName.isEmpty()) doodleSheet->openImage(fileName);
    }
}

// Called when the user clicks Save As in the menu
void MainWindow::save() {
    // A QAction represents the action of the user clicking
    QAction *action = qobject_cast<QAction *>(sender());

    // Stores the array of bytes of the users data
    QByteArray fileFormat = action->data().toByteArray();

    // Pass it to be saved
    saveFile(fileFormat);
}

// Opens a dialog to change the ink colour
void MainWindow::inkColour() {
    // Store the chosen color from the dialog
    QColor newColour = QColorDialog::getColor(doodleSheet->inkColour(), this);

    // If a valid color set it
    if (newColour.isValid()) doodleSheet->setInkColour(newColour);
}

// Opens a dialog that allows the user to change the ink thickness
void MainWindow::inkThickness() {
    // Stores button value
    bool ok;

    int newThickness = QInputDialog::getInt(this, tr("Doodle"), tr("Select thickness of ink:"), doodleSheet->inkThickness(), 1, 50, 1, &ok);

    // Change the ink thickness
    if (ok) doodleSheet->setInkThickness(newThickness);
}

// Open an about dialog
void MainWindow::about() {
    // Window title and text to display
    QMessageBox::about(this, tr("About Doodle"),
            tr("<p>ICS4U1 Mr.Trink</p>"
               "<p>Unit 11-16 Final Project: <b>Doodle</b></p>"
               "<p>Made by: <b>Zaid Alnemer</b></p>"));
}

// Open a directions dialog
void MainWindow::directions() {
    // Window title and text to display
    QMessageBox::about(this, tr("Directions"),
            tr("<p>This is a program for doodling on images.</p>"
               "<p>How to use program:</p>"
               "<p>\t<ul><li><b>How to Doodle:</b> Just hold the left mousebutton and drag the mouse to make lines and curves.</li>"
               "<li><b>How to change colour of ink (for drawing):</b> In the menu bar goto <i>Options->Ink Colour...</i> and change the ink colour.</li>"
               "<li><b>How to change thickness of ink (for drawing):</b> In the menu bar goto <i>Options->Ink Thickness...</i> and change the ink thickness (can only be a whole number between 1 and 50 inclusive).</li>"
               "<li><b>How to clear/erase your drawings and/or picture:</b> In the menu bar goto <i>Options->Clear Screen</i> or press <i>Ctrl+L</i> on your keyboard.</li>"
               "<li><b>How to open a file (to draw on): </b>In the menu bar goto <i>File->Open</i> or just press the shortcut key <i>Ctrl+O</i>.</li>"
               "<li><b>How to save a file: </b>In the menu bar goto <i>File->Save As->FileType.</i> there are many file types to choose from to save your image as.</li>"
               "<li><b>How to print:</b> In the menu bar goto <i>File->Print </i>or just press <i>Ctrl+P</i> on your keyboard.</li>"
               "<li><b>How to exit program:</b> In the menu bar goto <i>File->Exit</i> or just press <i>Esc</i> on the top left of your keyboard.</li>"
               "<li><b>To access useful help screens and documentations:</b> Go to the <i>Help</i> menu in the menu bar, you have three options: <i>Help->About</i>, <i>Help->About Qt</i>, and <i>Help->Directions</i>. "
               "<i>Help->About</i> can also be accessed with <i>Ctrl+I</i> on your keyboard, the <i>About</i> will provide documentation on the program. "
               "<i>Help->About Qt</i> will provide some documentation on the Qt Framework, which was used to develop this program."
               "<i>Help->Directions</i> can also be accessed with <i>Ctrl+H</i> on your keyboard, it is this very window, and serves as a help screen to help you use this program.</li></p></ul>"));
}

// Define menu actions that call functions
void MainWindow::createActions() {
    // Create the action tied to the menu
    openAct = new QAction(tr("&Open..."), this);

    // Define the associated shortcut key
    openAct->setShortcuts(QKeySequence::Open);

    // Tie the action to MainWindow::open()
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        // Create an action for each file format
        QAction *action = new QAction(text, this);

        // Set an action for each file format
        action->setData(format);

        // When clicked call MainWindow::save()
        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        // Attach each file format option menu item to Save As
        saveAsActs.append(action);
    }

    // Create print action and tie to MainWindow::print()
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), doodleSheet, SLOT(print()));

    // Define the associated shortcut key
    printAct->setShortcut(tr("Ctrl+P"));

    // Create exit action and tie to MainWindow::close()
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Esc")); // Setting shortcut to exitAct
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // Create ink colour action and tie to MainWindow::inkColour()
    inkColourAct = new QAction(tr("&Ink Colour..."), this);
    connect(inkColourAct, SIGNAL(triggered()), this, SLOT(inkColour()));

    // Create ink thickness action and tie to MainWindow::inkThickness()
    inkThicknessAct = new QAction(tr("Ink &Thickness..."), this);
    connect(inkThicknessAct, SIGNAL(triggered()), this, SLOT(inkThickness()));

    // Create clear screen action and tie to MainWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L")); // Setting shortcut to clearScreenAct
    connect(clearScreenAct, SIGNAL(triggered()), doodleSheet, SLOT(clearImage()));

    // Create about action and tie to MainWindow::about()
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setShortcut(tr("Ctrl+I"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    // Create about Qt action and tie to MainWindow::aboutQt()
    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    directionsAct = new QAction(tr("&Directions"), this);
    directionsAct->setShortcut(tr("Ctrl+H"));
    connect(directionsAct, SIGNAL(triggered()), this, SLOT(directions()));
}

// Create the menubar
void MainWindow::createMenus() {
    // Create Save As option and the list of file types
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach (QAction *action, saveAsActs) saveAsMenu->addAction(action);

    // Attach all actions to File Menu
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Attach all actions to Options Menu
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(inkColourAct);
    optionMenu->addAction(inkThicknessAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    // Attach all actions to Help Mneu
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    helpMenu->addSeparator();
    helpMenu->addAction(directionsAct);

    // Add menu items to the menubar
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::wasChanged() {
    // Check for changes since last save
    if (doodleSheet->isEdited()) {
       QMessageBox::StandardButton ret;

       // Doodle is the title
       // Add text and the buttons
       ret = QMessageBox::warning(this, tr("Doodle"),
                          tr("The image has been edited.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);

       // If save button clicked call for file to be saved
        if (ret == QMessageBox::Save) {
            return saveFile("png");

        // If cancel do nothing
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
    // Define path, name and default file type
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Get selected file from dialog
    // Add the proper file formats and extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath, tr("%1 Files (*.%2);;All Files (*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    // If no file do nothing
    if (fileName.isEmpty()) {
        return false;
    } else {

        // Call for the file to be saved
        return doodleSheet->saveImage(fileName, fileFormat.constData());
    }
}
