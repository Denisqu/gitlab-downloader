#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <QMainWindow>
#include <QWidget>

class MainWindow : public QMainWindow {
private:
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

private:
  QLayout *createRootLayout();
};


#endif // __MAIN_WINDOW_H__