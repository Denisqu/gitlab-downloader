#include "db_manager.hpp"
#include <QSqlDatabase>

DatabaseManager *DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager(const QString &path)
    : m_database(std::make_unique<QSqlDatabase>(
          QSqlDatabase::addDatabase("QSQLITE"))) {
  m_database->setDatabaseName(path);
  m_database->open();
}

DatabaseManager &
DatabaseManager::init(const QString &path = DATABASE_FILENAME) {
  if (m_instance == nullptr) {
    m_instance = new DatabaseManager(path);
    return *m_instance;
  }

  return *m_instance;
}

DatabaseManager &DatabaseManager::instance() {
  return m_instance ? *m_instance : init();
}
