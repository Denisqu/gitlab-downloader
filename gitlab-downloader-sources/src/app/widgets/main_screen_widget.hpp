#ifndef __MAIN_SCREEN_WIDGET_H__
#define __MAIN_SCREEN_WIDGET_H__

#include <QWidget>
#include <qlayout.h>
#include <qobjectdefs.h>

class MainScreenWidget : public QWidget {
  Q_OBJECT

public:
  explicit MainScreenWidget(QWidget *parent = nullptr);

signals:
  void settingsButtonClicked();

private:
  QLayout *createRootLayout();
  QLayout *createTopLayout();
  QLayout *createBottomLayout();
};

#endif // __MAIN_SCREEN_WIDGET_H__