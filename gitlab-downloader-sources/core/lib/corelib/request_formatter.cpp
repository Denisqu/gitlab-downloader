#include "request_formatter.hpp"
#include "gitlab_handler.hpp"
#include <QString>
#include <QtGlobal>

namespace Gitlab {
QString RequestFormatter::formatRequestJobs(QString baseUrl, qint64 projectId,
                                            QSet<Gitlab::JobScope> scope) {
  QString formatedScope = "";
  for (const auto &val : scope) {
    switch (val) {
    case JobScope::Success:
      formatedScope += "?scope[]=success";
      break;
    default:
      break;
    }
  }

  const auto requestString = QString("%1/api/v4/projects/%2/jobs%3")
                                 .arg(baseUrl)
                                 .arg(projectId)
                                 .arg(formatedScope);
  qInfo() << "requestString = " << requestString;

  return requestString;
}

QString RequestFormatter::formatRequestJobArtifacts(QString baseUrl,
                                                    qint64 projectId,
                                                    qint64 jobId) {
  const auto requestString = QString("%1/api/v4/projects/%2/jobs/%3/artifacts")
                                 .arg(baseUrl)
                                 .arg(projectId)
                                 .arg(jobId);
  qInfo() << "requestString = " << requestString;

  return requestString;
}
} // namespace Gitlab