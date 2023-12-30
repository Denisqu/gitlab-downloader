#include "../core/lib/DBLib/db_manager.hpp"
#include "../core/lib/corelib/gitlab_handler.hpp"
#include "main_window.hpp"
#include <../external/cutelogger6/include/ConsoleAppender.h>
#include <../external/cutelogger6/include/FileAppender.h>
#include <../external/cutelogger6/include/Logger.h>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QThread>
#include <QtConcurrent>
#include <qnamespace.h>
#include <qnetworkreply.h>
#include <qobjectdefs.h>

#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv) {
// attach console on win32
#if defined(_WIN32) /*&& defined(DEBUG)*/
  if (AttachConsole(ATTACH_PARENT_PROCESS)) {
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
  }
  qDebug() << "RUNNING APP IN DEBUG MODE...";
#endif

  ConsoleAppender *consoleAppender = new ConsoleAppender;
  consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
  FileAppender *fileAppender = new FileAppender("logs.log");
  cuteLogger->registerAppender(consoleAppender);
  cuteLogger->registerAppender(fileAppender);

  qInfo() << "Configuring application...";

  QApplication app{argc, argv};
  MainWindow mainWindow{};
  mainWindow.show();

  DatabaseManager::init();
  auto *gitlabHandler = new Gitlab::Handler(&app);
  QThread databaseManagerThread{&app};
  DatabaseManager::instance().moveToThread(&databaseManagerThread);
  gitlabHandler->moveToThread(&databaseManagerThread);
  databaseManagerThread.start();
  QMetaObject::invokeMethod(
      gitlabHandler,
      [gitlabHandler]() { gitlabHandler->processTestReply(nullptr); },
      Qt::QueuedConnection, static_cast<QNetworkReply *>(nullptr));

  qInfo() << "Starting the application!";
  auto result = app.exec();
  qInfo() << "Closing application!";

  databaseManagerThread.quit();
  databaseManagerThread.wait();

  if (result)
    qWarning() << "Something went wrong."
               << "Result code is" << result;
  return result;
}
