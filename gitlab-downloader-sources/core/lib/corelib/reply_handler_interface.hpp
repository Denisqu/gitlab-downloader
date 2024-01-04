#ifndef __REPLY_HANDLER_INTERFACE_H__
#define __REPLY_HANDLER_INTERFACE_H__

#include "artifact_info.hpp"
#include <QCoroNetworkReply>
#include <QObject>
#include <QVariant>

class QNetworkReply;

namespace Gitlab {

class IReplyHandler {
public:
  virtual ~IReplyHandler() = default;
  virtual QCoro::Task<QVariant> handleGetJobListReply(QNetworkReply *reply) = 0;
  virtual QCoro::Task<QVariant>
  handleDownloadArtifactsReply(QNetworkReply *reply,
                               const ArtifactInfo &info) = 0;
};

} // namespace Gitlab

#endif // __REPLY_HANDLER_INTERFACE_H__