#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "dblib-export.hpp"
#include <QCoroTask>
// #include <QCoroNetworkReply>
#include <QObject>
#include <QSqlDatabase>
#include <QString>
#include <memory>
#include <qobject.h>
#include <qtmetamacros.h>

// class QSqlDatabase;

const QString DATABASE_FILENAME = "name.db";

/*
Класс, инкапсулирующий логику работы с БД
*/
class DBLIB_EXPORT DatabaseManager : public QObject {
  Q_OBJECT

public:
  static DatabaseManager &instance();
  static DatabaseManager &init(const QString &path = DATABASE_FILENAME);

  // TODO: потом переделать через data access object (DAO)
  QCoro::Task<QString> getGitlabPrivateKey();

protected:
  explicit DatabaseManager(const QString &path);

private:
  static DatabaseManager *m_instance;
  std::unique_ptr<QSqlDatabase> m_database;
};

#endif // __DBMANAGER_H__