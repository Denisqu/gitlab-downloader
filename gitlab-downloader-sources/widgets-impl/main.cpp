#include "../core/lib/corelib/gitlab_handler.hpp"
#include "main_window.hpp"
// #include <ConsoleAppender.h>
// #include <FileAppender.h>
// #include <Logger.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <memory>
#include <qapplication.h>

#include "../external/simplelogger/logger.h"

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
  auto logger = new Logger();
  QApplication app{argc, argv};
  MainWindow mainWindow{};
  mainWindow.show();
  auto gitlabHandler = new GitlabHandler(&app);

  qInstallMessageHandler(simpleMessageHandler);
  qInfo() << "Starting the application!";
  auto result = app.exec();
  qInfo() << "Closing application!";

  // TODO: перенести логгер в другой поток
  logger->setParent(&app);

  if (result)
    qWarning() << "Something went wrong."
               << "Result code is" << result;
  return result;
}
