#include "gitlab_handler.hpp"
#include <QCoroNetworkReply>
#include <QFuture>
#include <QNetworkReply>
#include <QtConcurrent>
#include <qfuture.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

namespace {
QCoro::Task<> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();
  auto *reply =
      manager->get(QNetworkRequest{QStringLiteral("http://www.http2demo.io")});
  const auto awaited_reply = co_await reply;
  const auto data = awaited_reply->readAll();
  qDebug() << data;

  reply = manager->get(QNetworkRequest({"http://www.http2demo.io"}));
  const auto awaited_reply_2 = co_await reply;
  const auto data_2 = awaited_reply_2->readAll();
  qDebug() << data;

  reply->deleteLater();
}
} // namespace

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {

  qDebug() << "test!";
  testTask();
}

void GitlabHandler::processTestReply(QNetworkReply *reply) {}

// TODO: возможно нужно удлаить эту функцию
void GitlabHandler::onResult(QNetworkReply *reply) {
  qInfo() << "read reply from onResult:";
  qInfo() << reply->readAll();
}

QNetworkReply *GitlabHandler::sendRequest(const QNetworkRequest &request) {
  // auto reply = co_await m_networkManager->get(request);
  return nullptr;
}
