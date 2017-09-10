#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "saengineview.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  qmlRegisterType<SaEngineView>("sa", 1, 0, "EngineView");
  QQmlApplicationEngine engine;
  engine.load(QUrl(QLatin1String("qrc:/main.qml")));

  return app.exec();
}
