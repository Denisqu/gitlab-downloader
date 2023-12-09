#pragma once
#include <QMainWindow>
#include <qwidget.h>

class MainWindow : public QMainWindow {
private:
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  QLayout *createRootLayout();
};
