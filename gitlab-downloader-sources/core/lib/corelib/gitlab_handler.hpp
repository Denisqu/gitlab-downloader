#ifndef __GITLAB_HANDLER_H__
#define __GITLAB_HANDLER_H__

#include "corelib-export.hpp"
#include <QNetworkAccessManager>
#include <QObject>

// TODO: Переделать как здесь: https://www.qt.io/blog/asynchronous-apis-in-qt-6
class CORELIB_EXPORT GitlabHandler : public QObject {
  Q_OBJECT

public:
  explicit GitlabHandler(QObject *parent = nullptr);

private slots:
  void onResult(QNetworkReply *reply);

private:
  std::unique_ptr<QNetworkAccessManager> m_networkManager;
  QNetworkReply *sendRequest(const QNetworkRequest &request);
  void processTestReply(QNetworkReply *reply);
};

#endif // __GITLAB_HANDLER_H__