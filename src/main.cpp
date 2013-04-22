#include <QtGui/QApplication>

#include "./qt/main_window.h"

int main( int argc, char** argv )
{
  QApplication app(argc, argv);

  // Make Xlib and GLX thread safe under X11
  QApplication::setAttribute(Qt::AA_X11InitThreads);

  QCoreApplication::setOrganizationName("nghk.nl");
  QCoreApplication::setOrganizationDomain("www.nghk.nl");
  QCoreApplication::setApplicationName("Skin surface viewer.");
  QCoreApplication::setApplicationVersion("0.0");

  Q_INIT_RESOURCE(resource);

  MainWindow *window = new MainWindow();
  if (argc > 1)
      window->open(argv[1]);
  window->show();

  return app.exec();
}
