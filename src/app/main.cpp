// #include <QApplication>
#include <QDebug>
#include <iostream>
// #include <ostream>
#include <string>
// #include <qapplication.h>

/* QCoreApplication *createApplication(int &argc, char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (!qstrcmp(argv[i], "-debug"))
      return new QCoreApplication(argc, argv);
  }
  return new QApplication(argc, argv);
}
*/
#ifdef _WIN32
#include <Windows.h>
#endif

int main(int argc, char **argv) {
#ifdef _WIN32
if (AttachConsole(ATTACH_PARENT_PROCESS)) {

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}
#endif

  std::cout << "this is from cout before qDebug!" << std::endl;
  qDebug() << "qDebug before qapplication...";
  std::cout << "this-is-from-cout" << std::endl;  
  auto a = 11;
  int t = 11;
  std::cin >> t;
  std::cout << t << std::endl;
  std::cin >> t;

  // return app->exec();
  return t;
}
