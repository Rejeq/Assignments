#include <Renderer/GL.h>
#include <Log.h>

#include <QApplication>
#include <QOpenglWidget>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
#ifdef NDEBUG
  Log::Create();
  Log::Get().SetLevel(Log::LogBase::Level::Info);
  Log::Get().SetDefaultPattern();
#else
  Log::CreateDefault();
  Log::Get().SetLevel(Log::LogBase::Level::Trace);
#endif

  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
