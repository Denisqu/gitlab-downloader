#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QNetworkReply>
#include <QtConcurrent>
#include <qfuture.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

namespace {
QCoro::Task<void> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();

  auto firstRequest = QNetworkRequest{QStringLiteral(
      "https://gitlab.com/api/v4/projects/470007/jobs?scope[]=success")};

  // co_await getGitlabPrivateKey()
  const auto gitlabKey =
      co_await DatabaseManager::instance().getGitlabPrivateKey();
  firstRequest.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());

  auto *reply = manager->get(firstRequest);
  const auto awaited_reply = co_await reply;

  if (reply->error()) {
    qCritical() << reply->error();
  }

  qDebug() << "STATUS_CODE = "
           << awaited_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
           << "DATA = " << awaited_reply->readAll();

  reply->deleteLater();
}
} // namespace

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {
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
