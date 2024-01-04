#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QCoroFuture>
#include <QFuture>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
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
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)),
      model(this) {}

Handler::~Handler() = default;

QCoro::Task<void> Handler::processTestReply(QNetworkReply *reply) {

  auto joblistJson = co_await this->getJobsList("https://gitlab.com", 470007,
                                                {JobScope::Success});

  const auto jobId = joblistJson[0][QString("id")];
  qInfo() << "jobId = " << jobId << "jobId.toInteger() = " << jobId.toInteger();
  const auto &artifactsFileJson = joblistJson[0][QString("artifacts_file")];
  ArtifactInfo info{artifactsFileJson["size"].toInteger(),
                    artifactsFileJson["filename"].toString()};
  auto isDownloaded = co_await this->downloadArtifacts(
      "https://gitlab.com", 470007, jobId.toInteger(), info);

  co_return;
}

MainTableModel *Handler::getModel() { return &model; }

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

Q_SLOT QCoro::Task<bool> Handler::downloadArtifacts(QString baseUrl,
                                                    qint64 projectId,
                                                    qint64 jobId,
                                                    const ArtifactInfo &info) {
  const auto artifactsDownloadRequest =
      [this, jobId]() -> QCoro::Task<QNetworkRequest> {
    const auto requestString = formatter.formatRequestJobArtifacts(
        "https://gitlab.com", 470007, jobId);
    auto request = QNetworkRequest{requestString};
    const auto gitlabKey =
        co_await DatabaseManager::instance().getGitlabPrivateKey();
    request.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
    co_return request;
  }();

  QFile file(info.name);
  file.open(QIODevice::ReadWrite);
  auto *reply = m_networkManager->get(co_await artifactsDownloadRequest);

  qint64 bytesWritten = 0;
  while (true) {
    const auto data =
        co_await qCoro(reply).read(1024 * 10, std::chrono::seconds(120));
    if (data.isEmpty())
      break;
    bytesWritten += data.size();
    qDebug() << "Bytes written: " << bytesWritten << "/" << info.size;
    co_await QtConcurrent::run([&file, &data]() mutable { file.write(data); });
  }
  file.close();
  reply->deleteLater();

  bool downloadResultStatus = info.size == bytesWritten ? true : false;
  qDebug() << "File downloading is finished. Status = " << downloadResultStatus;

  if (!downloadResultStatus) {
    qInfo() << "File downloading wasn't successful. Deleting file...";
    file.remove();
  }

  co_return downloadResultStatus;
}

} // namespace Gitlab