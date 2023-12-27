#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "simplelogger-export.h"
class QObject;
#include <QtGlobal>
class QMessageLogContext;
class QString;

void SIMPLELOGGER_EXPORT simpleMessageHandler(QtMsgType type,
                                              const QMessageLogContext &context,
                                              const QString &msg);

class SIMPLELOGGER_EXPORT Logger {
public:
  explicit Logger();
  void setParent(QObject *parent);
};

#endif // __LOGGER_H__