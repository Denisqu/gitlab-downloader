#include "logger.h"
#include "simpleQtLogger.h"
#include <QApplication>
#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>
#include <QtLogging>

SIMPLELOGGER_EXPORT Logger::Logger() {
  // enable sinks
  simpleqtlogger::ENABLE_LOG_SINK_FILE = true;
  simpleqtlogger::ENABLE_LOG_SINK_CONSOLE = true;
  simpleqtlogger::ENABLE_LOG_SINK_QDEBUG = false;
  simpleqtlogger::ENABLE_LOG_SINK_SIGNAL = true;
#ifdef Q_OS_LINUX
  simpleqtlogger::ENABLE_LOG_SINK_SYSLOG = true;
  simpleqtlogger::NAME_LOG_SINK_SYSLOG = "";
  simpleqtlogger::FACILITY_LOG_SINK_SYSLOG = LOG_USER;
#endif
  // set log-features
  simpleqtlogger::ENABLE_FUNCTION_STACK_TRACE = true;
  simpleqtlogger::ENABLE_CONSOLE_COLOR = true;
  simpleqtlogger::ENABLE_CONSOLE_TRIMMED = true;
  simpleqtlogger::ENABLE_CONSOLE_LOG_FILE_STATE = true;
  // set log-levels (global; all enabled)
  simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_DEBUG = true;
  simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_FUNCTION = true;
  // set log-levels (specific)
  simpleqtlogger::EnableLogLevels enableLogLevels_file =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  simpleqtlogger::EnableLogLevels enableLogLevels_console =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  simpleqtlogger::EnableLogLevels enableLogLevels_qDebug =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  simpleqtlogger::EnableLogLevels enableLogLevels_signal =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  simpleqtlogger::EnableLogLevels enableLogLevels_syslog =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  enableLogLevels_console.logLevel_FUNCTION = false;
  simpleqtlogger::EnableLogLevels enableLogLevels_fileWarn =
      simpleqtlogger::ENABLE_LOG_LEVELS;
  enableLogLevels_fileWarn.logLevel_NOTE = false;
  enableLogLevels_fileWarn.logLevel_INFO = false;
  enableLogLevels_fileWarn.logLevel_DEBUG = false;
  enableLogLevels_fileWarn.logLevel_FUNCTION = false;
  enableLogLevels_syslog.logLevel_FUNCTION = false;

  // initialize SimpleQtLogger (step 1/2)
  simpleqtlogger::SimpleQtLogger::createInstance(qApp);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_file(
      enableLogLevels_file);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogFileName(
      "testSimpleQtLogger.log", 10 * 1000, 10);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_console(
      enableLogLevels_console);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_qDebug(
      enableLogLevels_qDebug);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_signal(
      enableLogLevels_signal);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_syslog(
      enableLogLevels_syslog);
  // simpleqtlogger::SimpleQtLogger::getInstance()->addLogFilter_file(QRegularExpression("_id="));
  // simpleqtlogger::SimpleQtLogger::getInstance()->addLogFilter_file(QRegularExpression("n="));
  //  2nd file-log (levels: warn..fatal)
  simpleqtlogger::SimpleQtLogger::getInstance()->addSinkFileLog("warn");
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_file(
      "warn", enableLogLevels_fileWarn);
  simpleqtlogger::SimpleQtLogger::getInstance()->setLogFileName(
      "warn", "testSimpleQtLoggerWarn.log", 10 * 1000, 10);
}

void SIMPLELOGGER_EXPORT Logger::setParent(QObject *parent) {
  simpleqtlogger::SimpleQtLogger::getInstance()->setParent(parent);
}

void SIMPLELOGGER_EXPORT simpleMessageHandler(QtMsgType type,
                                              const QMessageLogContext &context,
                                              const QString &msg) {
  Q_UNUSED(context);

  switch (type) {
  case QtDebugMsg:
    L_DEBUG(msg);
    break;
  case QtWarningMsg:
    L_WARN(msg);
    break;
  case QtCriticalMsg:
    L_ERROR(msg);
    break;
  case QtFatalMsg:
    L_FATAL(msg);
    break;
  }
}
