#ifndef __MAIN_SCREEN_WIDGET_H__
#define __MAIN_SCREEN_WIDGET_H__

#include <QWidget>
#include <qabstractitemmodel.h>
#include <qlayout.h>
#include <qobjectdefs.h>
#include <qpushbutton.h>

class QListView;
class QPushButton;

class MainScreenWidget : public QWidget {
  Q_OBJECT

public:
  explicit MainScreenWidget(QWidget *parent = nullptr);
  void setMainModel(QAbstractItemModel *model);

signals:
  void settingsButtonClicked();

private:
  QLayout *createRootLayout();
  QLayout *createTopLayout();
  QLayout *createBottomLayout();
  QPushButton *createSettingsButton();

  QListView *listView = nullptr;
};

#endif // __MAIN_SCREEN_WIDGET_H__