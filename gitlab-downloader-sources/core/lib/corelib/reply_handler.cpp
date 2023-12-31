#include "reply_handler.hpp"
#include <QCoroFuture>
#include <QFile>
#include <QFuture>
#include <QJsonDocument>
#include <QtConcurrent>

namespace Gitlab {
QCoro::Task<QVariant>
ReplyHandler::handleDownloadArtifactsReply(QNetworkReply *reply,
                                           const ArtifactInfo &info) {
  QFile file(info.name);
  file.open(QIODevice::ReadWrite);

  qint64 bytesWritten = 0;
  while (true) {
    const auto data =
        co_await qCoro(reply).read(1024, std::chrono::seconds(120));
    if (data.isEmpty())
      break;
    bytesWritten += data.size();
    qInfo() << "KiB written: " << bytesWritten / 1024 << "/"
            << info.size / 1024;
    co_await QtConcurrent::run([&file, &data]() mutable { file.write(data); });
  }
  file.close();

  bool downloadResultStatus = info.size == bytesWritten ? true : false;
  qInfo() << "File downloading is finished. Status = " << downloadResultStatus;

  if (!downloadResultStatus) {
    qWarning() << "File downloading wasn't successful. Deleting file...";
    file.remove();
  }

  co_return downloadResultStatus;
}

QCoro::Task<QVariant>
ReplyHandler::handleGetJobListReply(QNetworkReply *reply) {
  reply = co_await reply;

  if (reply->error()) {
    qCritical() << reply->error();
    co_return QJsonDocument();
  }

  const auto json = QJsonDocument::fromJson(reply->readAll());
  co_return json;
}
} // namespace Gitlab