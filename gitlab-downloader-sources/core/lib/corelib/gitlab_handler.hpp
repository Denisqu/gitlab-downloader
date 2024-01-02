#ifndef __GITLAB_HANDLER_H__
#define __GITLAB_HANDLER_H__

#include "corelib-export.hpp"
#include "main_table_model.hpp"
#include "request_formatter.hpp"
#include <QCoroNetworkReply>
#include <QJsonDocument>
#include <QObject>

class QJsonDocument;
class QNetworkReply;

namespace Gitlab {

enum class JobScope { Success };

// TODO: Переделать как здесь: https://www.qt.io/blog/asynchronous-apis-in-qt-6
class CORELIB_EXPORT Handler : public QObject {
  Q_OBJECT

public:
  explicit Handler(QObject *parent = nullptr);
  ~Handler();
  MainTableModel *getModel();
  Q_SLOT QCoro::Task<void> processTestReply(QNetworkReply *reply);
  Q_SLOT QCoro::Task<QJsonDocument>
  getJobsList(QString baseUrl, qint64 projectId, QSet<JobScope> scope);
  Q_SLOT QCoro::Task<QJsonDocument>
  downloadArtifacts(QString baseUrl, qint64 projectId, qint64 jobId);

private:
  std::unique_ptr<QNetworkAccessManager> m_networkManager;
  RequestFormatter formatter;
  MainTableModel model;
};

} // namespace Gitlab

#endif // __GITLAB_HANDLER_H__