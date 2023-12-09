#include "main_window.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qobject.h>
#include <qwidget.h>

MainWindow::MainWindow(QWidget *parent) {
  setCentralWidget(new QWidget());
  centralWidget()->setLayout(createRootLayout());
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
