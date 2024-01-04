#include "../../core/lib/corelib/reply_handler.hpp"
#include <QCoroFuture>
#include <QCoroNetworkReply>
#include <QCoroTask>
#include <QDebug>
#include <QEventLoop>
#include <QFuture>
#include <QNetworkReply>
#include <QSignalSpy>
#include <QtConcurrent>
#include <gtest/gtest.h>
#include <qeventloop.h>
#include <qfuturewatcher.h>
#include <qnetworkreply.h>
#include <qsignalspy.h>

struct ReplyHanlderTests : public testing::Test {
public:
  QCoreApplication *application;

protected:
  virtual void SetUp() override {
    int argc = 1;
    const char *argv = "test";
    application = new QCoreApplication(argc, const_cast<char **>(&argv));
  }
  virtual void TearDown() override {
    QCoreApplication::instance()->exit();
    delete application;
  }
};

struct ReplyHandlerObject : public QObject, Gitlab::ReplyHandler {};

TEST_F(ReplyHanlderTests, handleGetJobListReply) {
  ReplyHandlerObject handler;
  QNetworkReply *testReply;

  // нужно как-то создать поток, в котором потом запустить event-loop
  auto task = handler.handleGetJobListReply(testReply);
  QCoro::connect(std::move(task), &handler,
                 [this](auto &&result) { EXPECT_EQ(true, true); });

  /*
  QFutureWatcher<void> watcher;
  auto future = QtConcurrent::run([&handler, &testReply]() {
    QCoro::waitFor(handler.handleGetJobListReply(testReply));
  });
  watcher.setFuture(future);
  QSignalSpy spy(&watcher, &QFutureWatcher<void>::finished);
  emit testReply->finished();

  for (int i = 0; i < 1000; ++i) {
    QCoreApplication::processEvents();

    if (i == 0) {
      emit testReply->finished();
      // QCoro::waitFor(handler.handleGetJobListReply(testReply));
    }
  }*/

  // EXPECT_EQ(spy.count(), 1);
  //   emit testReply->finished();
  EXPECT_EQ(true, true);
}