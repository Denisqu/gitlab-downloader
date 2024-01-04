#ifndef __REPLY_HANDLER_H__
#define __REPLY_HANDLER_H__

#include "corelib-export.hpp"
#include "reply_handler_interface.hpp"

namespace Gitlab {

class CORELIB_EXPORT ReplyHandler : public IReplyHandler {
public:
  ~ReplyHandler() = default;
  QCoro::Task<QVariant>
  handleDownloadArtifactsReply(QNetworkReply *reply,
                               const ArtifactInfo &info) override;
  QCoro::Task<QVariant> handleGetJobListReply(QNetworkReply *reply) override;
};

} // namespace Gitlab
#endif // __REPLY_HANDLER_H__