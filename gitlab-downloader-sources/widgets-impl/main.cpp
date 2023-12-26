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

  // Logger
  /*
  auto consoleAppender = std::make_unique<ConsoleAppender>();
  consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
  cuteLogger->registerAppender(consoleAppender.get());
  auto fileAppender = std::make_unique<FileAppender>("widgets-impl.log");
  cuteLogger->registerAppender(fileAppender.get());
  */

  QApplication app{argc, argv};
  MainWindow mainWindow{};
  mainWindow.show();
  auto gitlabHandler = new GitlabHandler(&app);

  qInfo() << "Starting the application!";
  auto result = app.exec();
  qInfo() << "Closing application!";

  if (result)
    qWarning() << "Something went wrong."
               << "Result code is" << result;
  return result;
}
