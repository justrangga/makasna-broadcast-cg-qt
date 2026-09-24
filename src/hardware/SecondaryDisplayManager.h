#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QQuickWindow>
#include <QScreen>

class SecondaryDisplayManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged)
    Q_PROPERTY(int targetScreenIndex READ targetScreenIndex NOTIFY targetScreenIndexChanged)
    Q_PROPERTY(QString testPattern READ testPattern WRITE setTestPattern NOTIFY testPatternChanged)

public:
    explicit SecondaryDisplayManager(QObject *parent = nullptr);
    ~SecondaryDisplayManager();

    bool isOpen() const { return m_isOpen; }
    int targetScreenIndex() const { return m_targetScreenIndex; }
    QString testPattern() const { return m_testPattern; }
    void setTestPattern(const QString &pattern);

    Q_INVOKABLE QVariantList getDisplays() const;
    Q_INVOKABLE bool openFullscreenOutput(int screenIndex, const QString &pattern = "none");
    Q_INVOKABLE void closeFullscreenOutput();

signals:
    void isOpenChanged();
    void targetScreenIndexChanged();
    void testPatternChanged();

private:
    bool m_isOpen = false;
    int m_targetScreenIndex = 0;
    QString m_testPattern = "none";
    QQuickWindow *m_secondaryWindow = nullptr;
};
