#ifndef __REQUEST_FORMATTER_H__
#define __REQUEST_FORMATTER_H__

class QString;
template <typename T> class QSet;
// class qint64;
#include <QtGlobal>

namespace Gitlab {

enum class JobScope;

class RequestFormatter {
public:
  QString formatRequestJobs(QString baseUrl, qint64 projectId,
                            QSet<Gitlab::JobScope> scope);
  QString formatRequestJobArtifacts(QString baseUrl, qint64 projectId,
                                    qint64 jobId);
};

} // namespace Gitlab
#endif // __REQUEST_FORMATTER_H__