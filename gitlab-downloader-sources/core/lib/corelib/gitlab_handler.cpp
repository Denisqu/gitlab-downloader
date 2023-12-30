#include "gitlab_handler.hpp"
#include "../DBLib/db_manager.hpp"
#include <QFuture>
#include <QNetworkReply>
#include <QSet>
#include <QtConcurrent>
#include <qfuture.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

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

  qInfo() << "STATUS_CODE = "
          << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
          << "DATA = " << reply->readAll();

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
