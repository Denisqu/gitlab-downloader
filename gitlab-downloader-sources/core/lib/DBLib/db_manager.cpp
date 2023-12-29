#include "db_manager.hpp"
#include <QCoroIODevice>
#include <QDebug>
#include <QFile>

DatabaseManager *DatabaseManager::m_instance = nullptr;

DatabaseManager::DatabaseManager(const QString &path)
    : m_database(std::make_unique<QSqlDatabase>(
          QSqlDatabase::addDatabase("QSQLITE"))) {
  m_database->setDatabaseName(path);
  m_database->open();
}

DatabaseManager &DatabaseManager::init(const QString &path) {
  if (m_instance == nullptr) {
    m_instance = new DatabaseManager(path);
    return *m_instance;
  }

  return *m_instance;
}

QCoro::Task<QString> DatabaseManager::getGitlabPrivateKey() {
  QFile file;
  file.setFileName("key.txt");
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  qInfo() << "file opened?: " << file.isOpen();
  file.readAll();
  const auto key = co_await file;
  file.close();

  qInfo() << "GitlabPrivateKey = " << key;

  co_return key;
}

DatabaseManager &DatabaseManager::instance() {
  return m_instance ? *m_instance : init();
}
