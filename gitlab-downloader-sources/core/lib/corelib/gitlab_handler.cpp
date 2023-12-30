#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSet>
#include <QtConcurrent>
#include <chrono>
#include <numeric>
#include <qfuture.h>
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>
#include <qtypes.h>

namespace {

using namespace Gitlab;

QString formatRequestJobs(QString baseUrl, qint64 projectId,
                          QSet<JobScope> scope) {
  QString formatedScope = "";
  for (const auto &val : scope) {
    switch (val) {
    case JobScope::Success:
      formatedScope += "?scope[]=success";
      break;
    default:
      break;
    }
  }

  const auto requestString = QString("%1/api/v4/projects/%2/jobs%3")
                                 .arg(baseUrl)
                                 .arg(projectId)
                                 .arg(formatedScope);
  qInfo() << "requestString = " << requestString;

  return requestString;
}

QString formatRequestJobArtifacts(QString baseUrl, qint64 projectId,
                                  qint64 jobId) {
  const auto requestString = QString("%1/api/v4/projects/%2/jobs/%3/artifacts")
                                 .arg(baseUrl)
                                 .arg(projectId)
                                 .arg(jobId);
  qInfo() << "requestString = " << requestString;

  return requestString;
}

// TODO: убрать testTask, перенести всё в функции Handler
QCoro::Task<void> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();

  const auto firstRequest = []() -> QCoro::Task<QNetworkRequest> {
    const auto requestString =
        formatRequestJobs("https://gitlab.com", 470007, {JobScope::Success});
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
      [jobId]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatRequestJobArtifacts(
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

} // namespace

namespace Gitlab {

Handler::Handler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {}

QCoro::Task<void> Handler::processTestReply(QNetworkReply *reply) {
  co_await testTask();
}

QCoro::Task<QJsonDocument>
Handler::getJobsList(QString baseUrl, qint64 projectId, QSet<JobScope> scope) {
  const auto request = [baseUrl, projectId,
                        scope]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatRequestJobs(baseUrl, projectId, scope);
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