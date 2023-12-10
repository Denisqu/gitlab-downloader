#include "main_window.hpp"
#include <QApplication>
#include <QDebug>
#include <qapplication.h>

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv) {
// attach console on win32
#if defined(_WIN32) && defined(DEBUG)
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
  }
  qDebug() << "RUNNING APP IN DEBUG MODE...";
#endif

  QApplication app{argc, argv};
  MainWindow mainWindow{};
  mainWindow.show();

  return app.exec();
}
