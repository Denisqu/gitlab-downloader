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

Handler::Handler(QObject *parent, IReplyHandler *replyHandler)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)),
      m_replyHandler(replyHandler), model(this) {}

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

  const auto json =
      (co_await m_replyHandler->handleGetJobListReply(reply)).toJsonDocument();

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

  auto *reply = m_networkManager->get(co_await artifactsDownloadRequest);
  const auto result =
      (co_await m_replyHandler->handleDownloadArtifactsReply(reply, info))
          .toBool();
  reply->deleteLater();

  co_return result;
}

} // namespace Gitlab