#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QQuickStyle>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#ifdef _WIN32
#include <windows.h>
#endif

#include "core/AnimationEngine.h"
#include "core/ProjectModel.h"
#include "core/PlayoutController.h"
#include "core/SmartDataManager.h"
#include "core/EsportsMatchEngine.h"
#include "hardware/DeckLinkController.h"
#include "hardware/SecondaryDisplayManager.h"
#include "network/RossTalkServer.h"
#include "network/BroadcastHttpServer.h"

// File and Console Logger
static void customLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);
    const char *typeStr = "INFO";
    switch (type) {
    case QtDebugMsg:    typeStr = "DEBUG"; break;
    case QtInfoMsg:     typeStr = "INFO"; break;
    case QtWarningMsg:  typeStr = "WARN"; break;
    case QtCriticalMsg: typeStr = "CRIT"; break;
    case QtFatalMsg:    typeStr = "FATAL"; break;
    }

    QString logLine = QString("[%1] [%2] %3\n")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"), typeStr, msg);

    fprintf(stderr, "%s", logLine.toLocal8Bit().constData());
    fflush(stderr);

    static QString logPath = QCoreApplication::applicationDirPath() + "/makasna_debug.log";
    static QFile logFile(logPath);
    if (!logFile.isOpen()) {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
    if (logFile.isOpen()) {
        QTextStream out(&logFile);
        out << logLine;
        out.flush();
    }
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    qInstallMessageHandler(customLogHandler);

    qInfo() << "==========================================";
    qInfo() << "MAKASNA Broadcast CG Studio Starting...";
    qInfo() << "==========================================";

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
    auto *esportsEngine = new EsportsMatchEngine(&app);
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

    engine.addImportPath("qrc:/qt/qml");
    engine.addImportPath("qrc:/");
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");

    engine.rootContext()->setContextProperty("animationEngine", animationEngine);
    engine.rootContext()->setContextProperty("projectModel", projectModel);
    engine.rootContext()->setContextProperty("smartDataManager", smartDataManager);
    engine.rootContext()->setContextProperty("esportsEngine", esportsEngine);
    engine.rootContext()->setContextProperty("playoutController", playoutController);
    engine.rootContext()->setContextProperty("decklinkController", decklinkController);
    engine.rootContext()->setContextProperty("secondaryDisplayManager", secondaryDisplayManager);

    QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError> &warnings) {
        for (const auto &w : warnings) {
            qWarning() << "[QML Warning]" << w.toString();
        }
    });

    // Attempt 1: loadFromModule
    qInfo() << "[Bootstrap] Loading QML module Makasna.BroadcastCG...";
    engine.loadFromModule("Makasna.BroadcastCG", "Main");

    // Attempt 2: Fallback if loadFromModule produced no root objects
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "[Bootstrap] loadFromModule yielded no root objects. Attempting fallback URLs...";
        const QStringList candidateUrls = {
            "qrc:/qt/qml/Makasna/BroadcastCG/src/qml/Main.qml",
            "qrc:/qt/qml/Makasna/BroadcastCG/Main.qml",
            "qrc:/Makasna/BroadcastCG/src/qml/Main.qml",
            "qrc:/src/qml/Main.qml"
        };
        for (const auto &url : candidateUrls) {
            qInfo() << "[Bootstrap] Trying QML URL:" << url;
            engine.load(QUrl(url));
            if (!engine.rootObjects().isEmpty()) {
                qInfo() << "[Bootstrap] Successfully initialized window from:" << url;
                break;
            }
        }
    }

    if (engine.rootObjects().isEmpty()) {
        qCritical() << "[Bootstrap] FATAL: Unable to instantiate any root QML component!";
#ifdef _WIN32
        MessageBoxW(NULL, 
            L"MAKASNA Broadcast CG tidak dapat memuat antarmuka grafis QML.\n\nDetail error telah dicatat ke file 'makasna_debug.log' di folder aplikasi.",
            L"MAKASNA Broadcast CG - Error",
            MB_ICONERROR | MB_OK);
#endif
        return -1;
    }

    qInfo() << "[Bootstrap] Window initialized successfully. Entering event loop.";
    return app.exec();
}
