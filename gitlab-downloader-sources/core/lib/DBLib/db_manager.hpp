#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#include "dblib-export.hpp"
#include <QString>
#include <memory>

class QSqlDatabase;

const QString DATABASE_FILENAME = "name.db";

/*
Класс, инкапсулирующий логику работы с БД
*/
class DBLIB_EXPORT DatabaseManager {
public:
  static DatabaseManager &instance();
  static DatabaseManager &init(const QString &path);

protected:
  explicit DatabaseManager(const QString &path);

private:
  static DatabaseManager *m_instance;
  std::unique_ptr<QSqlDatabase> m_database;
};

#endif // __DBMANAGER_H__