#include <QQuickView>
#include <QScreen>
#include <QGuiApplication>
#include <asteroidapp.h>

#include "audiorecorder.h"

int main(int argc, char *argv[])
{
    //qmlRegisterType<PieChart>("Charts", 1, 0, "PieChart");
    qmlRegisterType<AudioRecorder>("org.asteroid.recorder", 1, 0, "AudioRecorder");

    QScopedPointer<QGuiApplication> app(AsteroidApp::application(argc, argv));
    QScopedPointer<QQuickView> view(AsteroidApp::createView());
    view->setSource(QUrl("qrc:/main.qml"));
    view->resize(app->primaryScreen()->size());
    view->show();
    return app->exec();
}