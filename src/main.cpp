#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickStyle>

#include "core/AnimationEngine.h"
#include "core/ProjectModel.h"
#include "core/PlayoutController.h"
#include "core/SmartDataManager.h"
#include "hardware/DeckLinkController.h"
#include "hardware/SecondaryDisplayManager.h"
#include "network/RossTalkServer.h"
#include "network/BroadcastHttpServer.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    app.setApplicationName("Makasna Broadcast CG");
    app.setOrganizationName("Makasna");
    app.setOrganizationDomain("makasna.tv");
    app.setApplicationVersion("1.0.0");
    app.setWindowIcon(QIcon(":/makasna-logo.png"));

    QQuickStyle::setStyle("Basic");

    // Initialize Native C++ Broadcast Controllers
    auto *animationEngine = new AnimationEngine(&app);
    auto *projectModel = new ProjectModel(&app);
    auto *smartDataManager = new SmartDataManager(projectModel, &app);
    auto *playoutController = new PlayoutController(&app);
    auto *decklinkController = new DeckLinkController(&app);
    auto *secondaryDisplayManager = new SecondaryDisplayManager(&app);

    // Initialize Network Broadcast Servers
    auto *rossTalkServer = new RossTalkServer(playoutController, &app);
    rossTalkServer->start(7788);

    auto *httpServer = new BroadcastHttpServer(&app);
    httpServer->start(4989);

    // QML Engine Setup
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("animationEngine", animationEngine);
    engine.rootContext()->setContextProperty("projectModel", projectModel);
    engine.rootContext()->setContextProperty("smartDataManager", smartDataManager);
    engine.rootContext()->setContextProperty("playoutController", playoutController);
    engine.rootContext()->setContextProperty("decklinkController", decklinkController);
    engine.rootContext()->setContextProperty("secondaryDisplayManager", secondaryDisplayManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("Makasna.BroadcastCG", "Main");

    return app.exec();
}
