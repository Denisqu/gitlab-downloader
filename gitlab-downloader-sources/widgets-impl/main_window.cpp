#include "main_window.hpp"
#include "../core/lib/DBLib/db_manager.hpp"
#include "widgets/main_screen_widget.hpp"
#include <QLabel>
#include <QStackedWidget>
#include <QThread>
#include <QVBoxLayout>
#include <qabstractitemmodel.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qobject.h>
#include <qstackedwidget.h>
#include <qthread.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : secondThread(new QThread(this)), handler(new Gitlab::Handler(this)) {
  init();
  setCentralWidget(new QStackedWidget());
  QStackedWidget *centralStackedWidget =
      qobject_cast<QStackedWidget *>(centralWidget());

  const auto mainScreenWidget = new MainScreenWidget(this);
  mainScreenWidget->setMainModel(
      static_cast<QAbstractItemModel *>(handler->getModel()));

  // only for debug
  connect(mainScreenWidget, &MainScreenWidget::settingsButtonClicked,
          centralStackedWidget, [centralStackedWidget]() {
            centralStackedWidget->setCurrentIndex(1);
          });
  centralStackedWidget->addWidget(mainScreenWidget);
  centralStackedWidget->addWidget(new QWidget);
}

MainWindow::~MainWindow() {
  secondThread->quit();
  secondThread->wait();
}

void MainWindow::init() {
  DatabaseManager::init();
  DatabaseManager::instance().moveToThread(secondThread);
  handler->moveToThread(secondThread);
  secondThread->start();

  // ONLY FOR DEBUG:
  QMetaObject::invokeMethod(
      handler, [this]() { handler->processTestReply(nullptr); },
      Qt::QueuedConnection, static_cast<QNetworkReply *>(nullptr));
}
