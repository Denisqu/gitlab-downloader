#ifndef __GITLAB_HANDLER_H__
#define __GITLAB_HANDLER_H__

#include "corelib-export.hpp"
#include <QCoroNetworkReply>
#include <QNetworkAccessManager>
#include <QObject>
#include <qjsondocument.h>

class QJsonDocument;

namespace Gitlab {

enum class JobScope { Success };

// TODO: Переделать как здесь: https://www.qt.io/blog/asynchronous-apis-in-qt-6
class CORELIB_EXPORT Handler : public QObject {
  Q_OBJECT

public:
  explicit Handler(QObject *parent = nullptr);
  Q_SLOT QCoro::Task<void> processTestReply(QNetworkReply *reply);
  Q_SLOT QCoro::Task<QJsonDocument>
  getJobsList(QString baseUrl, qint64 projectId, QSet<JobScope> scope);
  Q_SLOT QCoro::Task<QJsonDocument>
  downloadArtifacts(QString baseUrl, qint64 projectId, qint64 jobId);

private:
  std::unique_ptr<QNetworkAccessManager> m_networkManager;
};

} // namespace Gitlab

#endif // __GITLAB_HANDLER_H__