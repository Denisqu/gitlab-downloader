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

  centralStackedWidget->addWidget(new MainScreenWidget);

  // centralWidget()->setLayout(createRootLayout());
}

QLayout *MainWindow::createRootLayout() {
  const auto layout = []() {
    const auto boxLayout = new QVBoxLayout();
    const auto labelTest = new QLabel();
    labelTest->setText("This is a Test Text #0 !");
    boxLayout->addWidget(labelTest);

    return boxLayout;
  }();

  return layout;
}
