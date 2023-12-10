#ifndef __MAIN_SCREEN_WIDGET_H__
#define __MAIN_SCREEN_WIDGET_H__

#include <QWidget>
#include <qobjectdefs.h>

class MainScreenWidget : public QWidget {
  Q_OBJECT

public:
  explicit MainScreenWidget(QWidget *parent = nullptr);
};

#endif // __MAIN_SCREEN_WIDGET_H__