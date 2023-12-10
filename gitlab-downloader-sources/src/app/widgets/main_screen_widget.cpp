#include "main_screen_widget.hpp"
#include <qboxlayout.h>
#include <qlabel.h>

MainScreenWidget::MainScreenWidget(QWidget *parent) : QWidget(parent) {

  const auto layout = new QHBoxLayout();
  layout->addWidget(new QLabel("This is label from MainScreenWidget"));

  setLayout(layout);
}
