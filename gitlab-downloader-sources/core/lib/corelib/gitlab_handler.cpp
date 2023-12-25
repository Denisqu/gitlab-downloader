#include "gitlab_handler.hpp"
#include "../../../external/asyncfuture/asyncfuture.h"
#include <QFuture>
#include <QNetworkReply>
#include <qfuture.h>
#include <qnetworkreply.h>

/* TODO:
QNetworkReply *reply = manager.get(QNetworkRequest(url));

auto future = QtFuture::connect(reply, &QNetworkReply::finished)
        .then([reply] {
            return reply->readAll();
        })
        .then(QtFuture::Launch::Async, createImage)
        .then(processImage)
        .then(show)
        ...
*/

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {
  QUrl url("http://www.testingmcafeesites.com/index.html");
  QNetworkRequest request; // Отправляемый запрос

  request.setUrl(url); // Устанавлвиваем URL в запрос
  auto reply = m_networkManager->get(request);
  auto future = new QFuture<void>(
      AsyncFuture::observe(reply, &QNetworkReply::finished).future());
  AsyncFuture::observe(*future).subscribe(
      [reply]() {
        if (reply->error()) {
          qCritical() << "Error while processing http request:";
          qCritical() << reply->errorString();
        }

        qInfo() << reply->readAll();
      },
      []() { qCritical() << "onCancel"; });
}

void GitlabHandler::onResult(QNetworkReply *reply) {

  qInfo() << "read reply from onResult:";
  qInfo() << reply->readAll();
}
void GitlabHandler::sendRequest(const QNetworkRequest &request) {
  m_networkManager->get(request);
}
