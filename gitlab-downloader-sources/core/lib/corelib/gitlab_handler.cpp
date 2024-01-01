#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSet>
#include <QtConcurrent>
#include <chrono>
#include <qfuture.h>
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>
#include <qtypes.h>

namespace Gitlab {

Handler::Handler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {}

QCoro::Task<void> Handler::processTestReply(QNetworkReply *reply) {
  QNetworkAccessManager *manager = new QNetworkAccessManager();

  const auto firstRequest = [this]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatter.formatRequestJobs(
        "https://gitlab.com", 470007, {JobScope::Success});
    auto request = QNetworkRequest{requestString};
    const auto gitlabKey =
        co_await DatabaseManager::instance().getGitlabPrivateKey();
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
    co_return request;
  }();
  auto *firstReply = manager->get(co_await firstRequest);

  // Получаем ответ на запрос на последние джобы
  firstReply = co_await firstReply;

  if (firstReply->error()) {
    qCritical() << firstReply->error();
    co_return;
  }

  const auto json = QJsonDocument::fromJson(firstReply->readAll());

  qInfo() << "STATUS_CODE = "
          << firstReply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          << "jobs count: " << json.array().count()
          << " first job artifacts: " << json.array()[0][QString("artifacts")];

  // Качаем артефакт, пишем артефакт в файл:
  const auto jobId = json[0][QString("id")];
  qInfo() << "jobId = " << jobId << "jobId.toInteger() = " << jobId.toInteger();

  const auto artifactsDownloadRequest =
      [this, jobId]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatter.formatRequestJobArtifacts(
        "https://gitlab.com", 470007, jobId.toInteger());
    auto request = QNetworkRequest{requestString};
    const auto gitlabKey =
        co_await DatabaseManager::instance().getGitlabPrivateKey();
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
    co_return request;
  }();

  QFile file("downloadFile.zip");
  file.open(QIODevice::ReadWrite);
  auto *secondReply = manager->get(co_await artifactsDownloadRequest);

  while (true) {
    const auto data =
        co_await qCoro(secondReply).read(1024, std::chrono::seconds(120));
    if (data.isEmpty())
      break;
    qDebug() << "bytes written: " << data.size();
    file.write(data);
  }

  qDebug() << "file downloading is finished!";
  file.close();

  firstReply->deleteLater();
  secondReply->deleteLater();
  co_return;
}

QCoro::Task<QJsonDocument>
Handler::getJobsList(QString baseUrl, qint64 projectId, QSet<JobScope> scope) {
  const auto request = [baseUrl, projectId, scope,
                        this]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString =
        formatter.formatRequestJobs(baseUrl, projectId, scope);
    auto request = QNetworkRequest{requestString};
    const auto gitlabKey =
        co_await DatabaseManager::instance().getGitlabPrivateKey();
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
    co_return request;
  }();
  auto *reply = m_networkManager->get(co_await request);

  reply = co_await reply;

  if (reply->error()) {
    qCritical() << reply->error();
    co_return QJsonDocument();
  }

  const auto json = QJsonDocument::fromJson(reply->readAll());

  qInfo() << "STATUS_CODE = "
          << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          << "jobs count: " << json.array().count();

  reply->deleteLater();
  co_return json;
}

Q_SLOT QCoro::Task<QJsonDocument>
Handler::downloadArtifacts(QString baseUrl, qint64 projectId, qint64 jobId) {
  co_return QJsonDocument();
}

} // namespace Gitlab