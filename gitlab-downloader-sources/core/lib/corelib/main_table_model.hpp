#ifndef __MAIN_TABLE_MODEL_H__
#define __MAIN_TABLE_MODEL_H__

#include <QAbstractTableModel>

namespace Gitlab {

// см. https://doc.qt.io/qt-6/modelview.html
class MainTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit MainTableModel(QObject *parent);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
};

} // namespace Gitlab
#endif // __MAIN_TABLE_MODEL_H__