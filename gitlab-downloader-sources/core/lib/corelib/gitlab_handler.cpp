#include "gitlab_handler.hpp"
#include "../../../external/asyncfuture/asyncfuture.h"
#include <QCoroNetworkReply>
#include <QFuture>
#include <QNetworkReply>
#include <QtConcurrent>
#include <qfuture.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>

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

namespace {
QCoro::Task<> testTask() {
  QNetworkAccessManager *manager = new QNetworkAccessManager();
  auto *reply =
      manager->get(QNetworkRequest{QStringLiteral("http://www.http2demo.io")});
  const auto awaited_reply = co_await reply;
  const auto data = awaited_reply->readAll();
  qDebug() << data;
  reply->deleteLater();
}
} // namespace

// TODO: переписать красиво, проверить на утечки
/*
void GitlabHandler::processTestReply(QNetworkReply *reply) {
  // см. https://github.com/vpicaver/asyncfuture
  auto future = QFuture<void>(
      AsyncFuture::observe(reply, &QNetworkReply::finished).future());

  QNetworkRequest secondRequest;
  secondRequest.setUrl({"http://www.http2demo.io"});

  auto onCompletedFuture2 = [reply, this, secondRequest]() {
    if (reply->error()) {
      qCritical() << "Error while processing http request:";
      qCritical() << reply->errorString();
    }
    qInfo() << reply->readAll();

    auto nextReply = sendRequest(secondRequest);
    auto future3 = new QFuture<void>(
        AsyncFuture::observe(nextReply, &QNetworkReply::finished)
            .subscribe([nextReply]() { qDebug() << nextReply->readAll(); })
            .future());

    return future3;
  };

  auto future2 =
      AsyncFuture::observe(future).subscribe(onCompletedFuture2).future();

  future.then([]() { qDebug() << "test"; });
} */

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>(this)) {

  // connect(reply, &QNetworkReply::finished,
  //         [this, reply]() { processTestReply(reply); });
  /*
  QtFuture::connect(reply, &QNetworkReply::finished)
      .then([reply, this] {
        return sendRequest(
            QNetworkRequest({"http://www.testingmcafeesites.com/index.html"}));
      })
      .then([](QNetworkReply *reply) {

      });
  */
  qDebug() << "test!";
  /*
  auto request = [] {
    QUrl url("http://www.testingmcafeesites.com/index.html");
    QNetworkRequest request;
    request.setUrl(url);
    return request;
  }();
  */
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
