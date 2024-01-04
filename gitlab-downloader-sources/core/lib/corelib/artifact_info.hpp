#ifndef __ARTIFACTS_INFO_H__
#define __ARTIFACTS_INFO_H__

#include <QObject>

namespace Gitlab {

struct ArtifactInfo {
  qint64 size = 0;
  QString name = "None";
};

} // namespace Gitlab

#endif // __ARTIFACTS_INFO_H__