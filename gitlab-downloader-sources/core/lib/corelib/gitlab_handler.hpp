#ifndef __GITLAB_HANDLER_H__
#define __GITLAB_HANDLER_H__

// #include <QNetworkAccessManager>
#include <QObject>
#include <memory>

class QNetworkAccessManager;

class GitlabHandler : public QObject {
  Q_OBJECT

public:
  explicit GitlabHandler(QObject *parent = nullptr);

private:
  std::unique_ptr<QNetworkAccessManager> m_networkManager;
};

#endif // __GITLAB_HANDLER_H__