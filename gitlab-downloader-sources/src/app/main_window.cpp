#include "main_window.hpp"
#include "widgets/main_screen_widget.hpp"
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qobject.h>
#include <qstackedwidget.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget *parent) {
  setCentralWidget(new QStackedWidget());
  QStackedWidget *centralStackedWidget =
      qobject_cast<QStackedWidget *>(centralWidget());

  const auto mainScreenWidget = new MainScreenWidget();

  // only for debug
  connect(mainScreenWidget, &MainScreenWidget::settingsButtonClicked,
          centralStackedWidget, [centralStackedWidget]() {
            centralStackedWidget->setCurrentIndex(1);
          });
  centralStackedWidget->addWidget(mainScreenWidget);
  centralStackedWidget->addWidget(new QWidget);
}
