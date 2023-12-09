#include "main_window.hpp"
#include <QApplication>
#include <QDebug>
#include <iostream>
#include <qapplication.h>
#include <string>



#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv) {
// attach console on win32
#ifdef _WIN32
if (AttachConsole(ATTACH_PARENT_PROCESS)) {

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}
#endif

  QApplication app{argc, argv};
  
  MainWindow mainWindow{};
  mainWindow.show();
 
  return app.exec();
}
