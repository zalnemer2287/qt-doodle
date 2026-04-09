
---

# 🎨 2. Qt Doodle (Paint App) — README

```md
# Qt Doodle

A simple paint/drawing application built with C++ and Qt.

## Features
- Freehand drawing on canvas
- Adjustable ink color
- Adjustable brush thickness
- Clear canvas functionality
- File operations (open, save, print)
- Help and directions menu

## Tech Stack
- C++
- Qt Widgets
- Qt Designer
- Qt Resource System (.qrc)

## Project Structure
- `main.cpp` – application entry point
- `mainwindow.cpp / .h` – UI and app logic
- `doodlesheet.cpp / .h` – drawing logic
- `mainwindow.ui` – UI layout
- `resources.qrc` – icons and assets
- `.pro` file – Qt project configuration

## How to Run

### Option 1 (Recommended)
1. Install Qt and Qt Creator
2. Clone or download this repository
2. Open the `.pro` file
3. Configure project (Desktop Kit)
4. Click Run

### Option 2 (Command Line)

```bash
qmake
make
./QtDoodle
