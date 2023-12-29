#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QNetworkReply>
#include <QtConcurrent>
#include <qfuture.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

namespace {

QString formatRequestJobs(QString projectId, QString scope) {
  return QString("https://gitlab.com/api/v4/projects/%1/jobs%2")
      .arg(projectId)
      .arg(scope);
}

QCoro::Task<void> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();
  auto firstRequest =
      QNetworkRequest{formatRequestJobs("470007", "?scope[]=success")};
  const auto gitlabKey =
      co_await DatabaseManager::instance().getGitlabPrivateKey();
  firstRequest.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
  auto *reply = manager->get(firstRequest);

  const auto awaited_reply = co_await reply;

  if (reply->error()) {
    qCritical() << reply->error();
    co_return;
  }

  qDebug() << "STATUS_CODE = "
           << awaited_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
           << "DATA = " << awaited_reply->readAll();

  reply->deleteLater();
  co_return;
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
