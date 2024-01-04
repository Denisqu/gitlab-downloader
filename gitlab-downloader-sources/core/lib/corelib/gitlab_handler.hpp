#ifndef __GITLAB_HANDLER_H__
#define __GITLAB_HANDLER_H__

#include "artifact_info.hpp"
#include "corelib-export.hpp"
#include "main_table_model.hpp"
#include "reply_handler.hpp"
#include "request_formatter.hpp"
#include <QCoroNetworkReply>
#include <QJsonDocument>
#include <QObject>

class QJsonDocument;
class QNetworkReply;

namespace Gitlab {

enum class JobScope { Success };

class CORELIB_EXPORT Handler : public QObject {
  Q_OBJECT

public:
  explicit Handler(QObject *parent = nullptr,
                   IReplyHandler *replyHandler = new ReplyHandler());
  ~Handler();
  MainTableModel *getModel();
  Q_SLOT QCoro::Task<void> processTestReply(QNetworkReply *reply);
  Q_SLOT QCoro::Task<QJsonDocument>
  getJobsList(QString baseUrl, qint64 projectId, QSet<JobScope> scope);
  Q_SLOT QCoro::Task<bool> downloadArtifacts(QString baseUrl, qint64 projectId,
                                             qint64 jobId,
                                             const ArtifactInfo &info);

private:
  std::unique_ptr<QNetworkAccessManager> m_networkManager;
  RequestFormatter formatter;
  MainTableModel model;

  // TODO: изменить на std::unique_ptr
  IReplyHandler *m_replyHandler;
};

} // namespace Gitlab

#endif // __GITLAB_HANDLER_H__