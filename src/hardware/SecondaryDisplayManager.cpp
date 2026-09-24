#include "SecondaryDisplayManager.h"
#include <QGuiApplication>
#include <QDebug>

SecondaryDisplayManager::SecondaryDisplayManager(QObject *parent) : QObject(parent) {}

SecondaryDisplayManager::~SecondaryDisplayManager() {
    closeFullscreenOutput();
}

void SecondaryDisplayManager::setTestPattern(const QString &pattern) {
    if (m_testPattern != pattern) {
        m_testPattern = pattern;
        emit testPatternChanged();
    }
}

QVariantList SecondaryDisplayManager::getDisplays() const {
    QVariantList list;
    QList<QScreen*> screens = QGuiApplication::screens();
    QScreen *primary = QGuiApplication::primaryScreen();

    for (int i = 0; i < screens.size(); ++i) {
        QScreen *s = screens[i];
        QVariantMap map;
        map["index"] = i;
        map["name"] = s->name().isEmpty() ? QString("Display %1").arg(i + 1) : s->name();
        map["width"] = s->geometry().width();
        map["height"] = s->geometry().height();
        map["refreshRate"] = std::round(s->refreshRate());
        map["isPrimary"] = (s == primary);
        list.append(map);
    }
    return list;
}

bool SecondaryDisplayManager::openFullscreenOutput(int screenIndex, const QString &pattern) {
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screenIndex < 0 || screenIndex >= screens.size()) {
        screenIndex = 0;
    }

    m_targetScreenIndex = screenIndex;
    m_testPattern = pattern;
    m_isOpen = true;

    emit targetScreenIndexChanged();
    emit testPatternChanged();
    emit isOpenChanged();

    qDebug() << "[SecondaryDisplay] Opened fullscreen output on screen index:" << screenIndex
             << "with pattern:" << pattern;
    return true;
}

void SecondaryDisplayManager::closeFullscreenOutput() {
    if (m_isOpen) {
        m_isOpen = false;
        emit isOpenChanged();
        qDebug() << "[SecondaryDisplay] Closed fullscreen output";
    }
}
