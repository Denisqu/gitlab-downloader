#include "main_table_model.hpp"

namespace Gitlab {

MainTableModel::MainTableModel(QObject *parent) : QAbstractTableModel(parent) {}

int MainTableModel::rowCount(const QModelIndex &parent) const { return 2; }

int MainTableModel::columnCount(const QModelIndex &parent) const { return 4; }

QVariant MainTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole)
    return QString("Row%1, Column%2")
        .arg(index.row() + 1)
        .arg(index.column() + 1);

  switch (role) {
  case Qt::CheckStateRole:
    if (index.row() == 1 && index.column() == 0) // add a checkbox to cell(1,0)
      return Qt::Checked;
    break;
  }

  return QVariant();
}

QVariant MainTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return QString("Artifacts");
    case 1:
      return QString("Project");
    case 2:
      return QString("Date");
    case 3:
      return QString("Status");
    }
  }
  return QVariant();
}

} // namespace Gitlab