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

enum class JobScope : unsigned int { Success = 1 };

QString formatRequestJobs(qint64 projectId, QSet<JobScope> scope) {
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

  const auto requestString =
      QString("https://gitlab.com/api/v4/projects/%1/jobs%2")
          .arg(projectId)
          .arg(formatedScope);
  qInfo() << "requestString = " << requestString;

  return requestString;
}

QString formatRequestJobArtifacts(qint64 projectId, qint64 jobId) {
  const auto requestString =
      QString("https://gitlab.com/api/v4/projects/%1/jobs/%2/artifacts")
          .arg(projectId)
          .arg(jobId);
  qInfo() << "requestString = " << requestString;

  return requestString;
}

QCoro::Task<void> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();

  const auto firstRequest = []() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatRequestJobs(470007, {JobScope::Success});
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
    const auto requestString =
        formatRequestJobArtifacts(470007, jobId.toInteger());
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

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {}

QCoro::Task<void> GitlabHandler::processTestReply(QNetworkReply *reply) {
  co_await testTask();
}

// TODO: возможно нужно удлаить эту функцию
void GitlabHandler::onResult(QNetworkReply *reply) {
  qInfo() << "read reply from onResult:";
  qInfo() << reply->readAll();
}

QNetworkReply *GitlabHandler::sendRequest(const QNetworkRequest &request) {
  // auto reply = co_await m_networkManager->get(request);
  return nullptr;
}
