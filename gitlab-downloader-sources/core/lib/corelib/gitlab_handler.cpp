#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSet>
#include <QtConcurrent>
#include <qfuture.h>
#include <qjsondocument.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qstringview.h>

namespace {

enum class JobScope : unsigned int { Success = 1 };

QString formatRequestJobs(int projectId, QSet<JobScope> scope) {
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

  return QString("https://gitlab.com/api/v4/projects/%1/jobs%2")
      .arg(projectId)
      .arg(formatedScope);
}

QCoro::Task<void> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();
  const auto requestString = formatRequestJobs(470007, {JobScope::Success});
  auto firstRequest = QNetworkRequest{requestString};
  const auto gitlabKey =
      co_await DatabaseManager::instance().getGitlabPrivateKey();
  firstRequest.setRawHeader(QByteArray("PRIVATE-TOKEN"), gitlabKey.toUtf8());
  auto *reply = manager->get(firstRequest);

  // Получаем ответ на запрос на последние джобы
  reply = co_await reply;

  if (reply->error()) {
    qCritical() << reply->error();
    co_return;
  }

  const auto json = QJsonDocument::fromJson(reply->readAll());

  qInfo() << "STATUS_CODE = "
          << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          << "jobs count: " << json.array().count()
          << " first job artifacts: " << json.array()[0][QString("artifacts")];

  reply->deleteLater();
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
