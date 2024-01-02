#include "main_table_model.hpp"

namespace Gitlab {

MainTableModel::MainTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int MainTableModel::rowCount(const QModelIndex &parent) const { return 2; }

int MainTableModel::columnCount(const QModelIndex &parent) const { return 3; }

QVariant MainTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole)
    return QString("Row%1, Column%2")
        .arg(index.row() + 1)
        .arg(index.column() + 1);

  return QVariant();
}

} // namespace Gitlab