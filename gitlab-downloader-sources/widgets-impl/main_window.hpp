#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "../core/lib//corelib/gitlab_handler.hpp"
#include <QMainWindow>
#include <QThread>
#include <QWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  void init();

  QThread *secondThread = nullptr;
  Gitlab::Handler *handler = nullptr;
};

#endif // __MAIN_WINDOW_H__