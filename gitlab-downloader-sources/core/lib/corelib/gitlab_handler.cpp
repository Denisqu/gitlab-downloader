#include "gitlab_handler.hpp"
#include "../../../external/asyncfuture/asyncfuture.h"
#include <QFuture>
#include <QNetworkReply>

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
  auto request = [] {
    QUrl url("http://www.testingmcafeesites.com/index.html");
    QNetworkRequest request;
    request.setUrl(url);
    return request;
  }();
  auto reply = m_networkManager->get(request);
  // см. https://github.com/vpicaver/asyncfuture
  auto future = QFuture<void>(
      AsyncFuture::observe(reply, &QNetworkReply::finished).future());
  AsyncFuture::observe(future).subscribe(
      [reply]() {
        if (reply->error()) {
          qCritical() << "Error while processing http request:";
          qCritical() << reply->errorString();
        }
        qInfo() << reply->readAll();
      },
      []() { qCritical() << "onCancel"; });
}

// TODO: возможно нужно удлаить эту функцию
void GitlabHandler::onResult(QNetworkReply *reply) {
  qInfo() << "read reply from onResult:";
  qInfo() << reply->readAll();
}
void GitlabHandler::sendRequest(const QNetworkRequest &request) {
  m_networkManager->get(request);
}
