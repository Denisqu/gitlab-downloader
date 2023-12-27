#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "simplelogger-export.h"
class QObject;

class SIMPLELOGGER_EXPORT Logger {
public:
  explicit Logger();
  void setParent(QObject *parent);
};

#endif // __LOGGER_H__