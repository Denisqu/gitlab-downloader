#include "gitlab_handler.hpp"
#include <QNetwrokAccessManager>

GitlabHandler::GitlabHandler(QObject *parent)
    : m_networkManager(std::make_unique<QNetworkAccessManager>()) {}
