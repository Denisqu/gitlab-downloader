#ifndef __REPLY_HANDLER_INTERFACE_H__
#define __REPLY_HANDLER_INTERFACE_H__
class QNetworkReply;

class IReplyHandler {
  void handleReply(QNetworkReply reply);
};

#endif // __REPLY_HANDLER_INTERFACE_H__