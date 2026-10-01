#pragma once

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QTimer>
#include <QDateTime>

class EsportsMatchEngine : public QObject {
    Q_OBJECT

    // Tournament & Match Metadata
    Q_PROPERTY(QString tournamentName READ tournamentName WRITE setTournamentName NOTIFY tournamentNameChanged)
    Q_PROPERTY(QString matchFormat READ matchFormat WRITE setMatchFormat NOTIFY matchFormatChanged)
    Q_PROPERTY(QString matchPhase READ matchPhase WRITE setMatchPhase NOTIFY matchPhaseChanged)
    Q_PROPERTY(QString currentMap READ currentMap WRITE setCurrentMap NOTIFY currentMapChanged)

    // Team A State
    Q_PROPERTY(QString teamAName READ teamAName WRITE setTeamAName NOTIFY teamAChanged)
    Q_PROPERTY(QString teamATag READ teamATag WRITE setTeamATag NOTIFY teamAChanged)
    Q_PROPERTY(int teamAMaps READ teamAMaps WRITE setTeamAMaps NOTIFY teamAChanged)
    Q_PROPERTY(int teamARounds READ teamARounds WRITE setTeamARounds NOTIFY teamAChanged)
    Q_PROPERTY(QString teamAColor READ teamAColor WRITE setTeamAColor NOTIFY teamAChanged)

    // Team B State
    Q_PROPERTY(QString teamBName READ teamBName WRITE setTeamBName NOTIFY teamBChanged)
    Q_PROPERTY(QString teamBTag READ teamBTag WRITE setTeamBTag NOTIFY teamBChanged)
    Q_PROPERTY(int teamBMaps READ teamBMaps WRITE setTeamBMaps NOTIFY teamBChanged)
    Q_PROPERTY(int teamBRounds READ teamBRounds WRITE setTeamBRounds NOTIFY teamBChanged)
    Q_PROPERTY(QString teamBColor READ teamBColor WRITE setTeamBColor NOTIFY teamBChanged)

    // Active Rotating Sponsor (Barracks Controller Feature)
    Q_PROPERTY(QString activeSponsorName READ activeSponsorName NOTIFY sponsorChanged)
    Q_PROPERTY(QString activeSponsorTagline READ activeSponsorTagline NOTIFY sponsorChanged)
    Q_PROPERTY(QString activeSponsorLogo READ activeSponsorLogo NOTIFY sponsorChanged)
    Q_PROPERTY(bool isSponsorOnAir READ isSponsorOnAir WRITE setSponsorOnAir NOTIFY sponsorOnAirChanged)
    Q_PROPERTY(QVariantList sponsors READ sponsors NOTIFY sponsorsListChanged)
    Q_PROPERTY(QVariantList sponsorReport READ sponsorReport NOTIFY sponsorReportChanged)

    // Head-to-Head & Pick-Ban
    Q_PROPERTY(QVariantMap playerComparison READ playerComparison NOTIFY playerComparisonChanged)
    Q_PROPERTY(QVariantList pickBanList READ pickBanList NOTIFY pickBanListChanged)

public:
    explicit EsportsMatchEngine(QObject *parent = nullptr);

    QString tournamentName() const { return m_tournamentName; }
    void setTournamentName(const QString &name);

    QString matchFormat() const { return m_matchFormat; }
    void setMatchFormat(const QString &format);

    QString matchPhase() const { return m_matchPhase; }
    void setMatchPhase(const QString &phase);

    QString currentMap() const { return m_currentMap; }
    void setCurrentMap(const QString &map);

    QString teamAName() const { return m_teamAName; }
    void setTeamAName(const QString &name);
    QString teamATag() const { return m_teamATag; }
    void setTeamATag(const QString &tag);
    int teamAMaps() const { return m_teamAMaps; }
    void setTeamAMaps(int maps);
    int teamARounds() const { return m_teamARounds; }
    void setTeamARounds(int rounds);
    QString teamAColor() const { return m_teamAColor; }
    void setTeamAColor(const QString &color);

    QString teamBName() const { return m_teamBName; }
    void setTeamBName(const QString &name);
    QString teamBTag() const { return m_teamBTag; }
    void setTeamBTag(const QString &tag);
    int teamBMaps() const { return m_teamBMaps; }
    void setTeamBMaps(int maps);
    int teamBRounds() const { return m_teamBRounds; }
    void setTeamBRounds(int rounds);
    QString teamBColor() const { return m_teamBColor; }
    void setTeamBColor(const QString &color);

    QString activeSponsorName() const;
    QString activeSponsorTagline() const;
    QString activeSponsorLogo() const;
    bool isSponsorOnAir() const { return m_isSponsorOnAir; }
    void setSponsorOnAir(bool onAir);

    QVariantList sponsors() const { return m_sponsors; }
    QVariantList sponsorReport() const;
    QVariantMap playerComparison() const { return m_playerComparison; }
    QVariantList pickBanList() const { return m_pickBanList; }

    // Invokable Methods for UI & Automation
    Q_INVOKABLE void adjustRoundScore(bool isTeamA, int delta);
    Q_INVOKABLE void adjustMapScore(bool isTeamA, int delta);
    Q_INVOKABLE void swapSides();
    Q_INVOKABLE void resetMatchScores();

    // Sponsor Rotation & Analytics
    Q_INVOKABLE void rotateNextSponsor();
    Q_INVOKABLE void addSponsor(const QString &name, const QString &tagline, const QString &logoUrl);
    Q_INVOKABLE void removeSponsor(int index);
    Q_INVOKABLE void setSponsorRotationInterval(int seconds);
    Q_INVOKABLE QString exportSponsorReportCsv() const;

    // Head-to-Head & Pick-Ban
    Q_INVOKABLE void updatePlayerComparison(const QString &pAName, const QString &pATeam, double pAKda, int pAAcs, double pAWinRate,
                                            const QString &pBName, const QString &pBTeam, double pBKda, int pBAcs, double pBWinRate);
    Q_INVOKABLE void updatePickBanItem(int index, const QString &mapName, const QString &action, const QString &teamTag);

    // Ingest Game Telemetry / Webhook JSON
    Q_INVOKABLE bool ingestTelemetryJson(const QString &jsonString);

signals:
    void tournamentNameChanged();
    void matchFormatChanged();
    void matchPhaseChanged();
    void currentMapChanged();
    void teamAChanged();
    void teamBChanged();
    void sponsorChanged();
    void sponsorOnAirChanged();
    void sponsorsListChanged();
    void sponsorReportChanged();
    void playerComparisonChanged();
    void pickBanListChanged();
    void matchEventFired(const QString &eventName, const QVariantMap &eventDetails);

private slots:
    void onSponsorTimerTimeout();
    void onExposureTimerTimeout();

private:
    void initDefaultEsportsData();

    QString m_tournamentName = "MAKASNA CHAMPIONSHIP SERIES 2026";
    QString m_matchFormat = "BO3";
    QString m_matchPhase = "MAP 1 - LIVE";
    QString m_currentMap = "ASCENT";

    QString m_teamAName = "MAKASNA ESPORTS";
    QString m_teamATag = "MKS";
    int m_teamAMaps = 1;
    int m_teamARounds = 11;
    QString m_teamAColor = "#00e5ff";

    QString m_teamBName = "PACIFIC APEX";
    QString m_teamBTag = "APX";
    int m_teamBMaps = 0;
    int m_teamBRounds = 9;
    QString m_teamBColor = "#ff334b";

    // Sponsors & Exposure Tracking (Barracks Sight)
    struct SponsorItem {
        QString name;
        QString tagline;
        QString logoUrl;
        qint64 totalExposureMs = 0;
        int impressionsCount = 0;
    };
    QList<SponsorItem> m_sponsorItems;
    int m_currentSponsorIndex = 0;
    bool m_isSponsorOnAir = false;
    QTimer *m_sponsorRotationTimer = nullptr;
    QTimer *m_exposureTimer = nullptr;
    QVariantList m_sponsors;

    // Comparison & Veto
    QVariantMap m_playerComparison;
    QVariantList m_pickBanList;
};
