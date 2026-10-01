#include "EsportsMatchEngine.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

EsportsMatchEngine::EsportsMatchEngine(QObject *parent) : QObject(parent) {
    initDefaultEsportsData();

    // Auto-rotate sponsors every 15 seconds
    m_sponsorRotationTimer = new QTimer(this);
    m_sponsorRotationTimer->setInterval(15000);
    connect(m_sponsorRotationTimer, &QTimer::timeout, this, &EsportsMatchEngine::onSponsorTimerTimeout);
    m_sponsorRotationTimer->start();

    // Track on-air exposure every second (Barracks Sight analytics)
    m_exposureTimer = new QTimer(this);
    m_exposureTimer->setInterval(1000);
    connect(m_exposureTimer, &QTimer::timeout, this, &EsportsMatchEngine::onExposureTimerTimeout);
    m_exposureTimer->start();
}

void EsportsMatchEngine::initDefaultEsportsData() {
    // 1. Default Sponsors
    SponsorItem s1;
    s1.name = "ROG STRIX";
    s1.tagline = "Official Gaming Hardware Partner";
    s1.logoUrl = "qrc:/makasna-logo.svg";
    s1.impressionsCount = 1;
    m_sponsorItems.append(s1);

    SponsorItem s2;
    s2.name = "SECRET LAB";
    s2.tagline = "Official Gaming Chair";
    s2.logoUrl = "qrc:/makasna-logo.svg";
    m_sponsorItems.append(s2);

    SponsorItem s3;
    s3.name = "MONSTER ENERGY";
    s3.tagline = "Energy Drink Sponsor";
    s3.logoUrl = "qrc:/makasna-logo.svg";
    m_sponsorItems.append(s3);

    // Refresh QVariantList for QML
    m_sponsors.clear();
    for (const auto &item : m_sponsorItems) {
        QVariantMap map;
        map["name"] = item.name;
        map["tagline"] = item.tagline;
        map["logoUrl"] = item.logoUrl;
        m_sponsors.append(map);
    }

    // 2. Default Player Comparison (Head to Head)
    m_playerComparison["playerAName"] = "RENX";
    m_playerComparison["playerATeam"] = "MAKASNA";
    m_playerComparison["playerARole"] = "Duelist (Jett)";
    m_playerComparison["playerAKda"] = 1.64;
    m_playerComparison["playerAAcs"] = 284;
    m_playerComparison["playerAWinRate"] = 72.5;

    m_playerComparison["playerBName"] = "VIPERZ";
    m_playerComparison["playerBTeam"] = "APEX";
    m_playerComparison["playerBRole"] = "Initiator (Sova)";
    m_playerComparison["playerBKda"] = 1.32;
    m_playerComparison["playerBAcs"] = 238;
    m_playerComparison["playerBWinRate"] = 64.0;

    // 3. Default Pick & Ban (Map Veto)
    QVariantMap pb1{{"map", "BIND"}, {"action", "BAN"}, {"team", "MKS"}, {"status", "BANNED"}};
    QVariantMap pb2{{"map", "HAVEN"}, {"action", "BAN"}, {"team", "APX"}, {"status", "BANNED"}};
    QVariantMap pb3{{"map", "ASCENT"}, {"action", "PICK"}, {"team", "MKS"}, {"status", "CURRENT"}};
    QVariantMap pb4{{"map", "SUNSET"}, {"action", "PICK"}, {"team", "APX"}, {"status", "NEXT"}};
    QVariantMap pb5{{"map", "LOTUS"}, {"action", "DECIDER"}, {"team", "DECIDER"}, {"status", "DECIDER"}};
    m_pickBanList.append(pb1);
    m_pickBanList.append(pb2);
    m_pickBanList.append(pb3);
    m_pickBanList.append(pb4);
    m_pickBanList.append(pb5);
}

void EsportsMatchEngine::setTournamentName(const QString &name) {
    if (m_tournamentName != name) {
        m_tournamentName = name;
        emit tournamentNameChanged();
    }
}

void EsportsMatchEngine::setMatchFormat(const QString &format) {
    if (m_matchFormat != format) {
        m_matchFormat = format;
        emit matchFormatChanged();
    }
}

void EsportsMatchEngine::setMatchPhase(const QString &phase) {
    if (m_matchPhase != phase) {
        m_matchPhase = phase;
        emit matchPhaseChanged();
    }
}

void EsportsMatchEngine::setCurrentMap(const QString &map) {
    if (m_currentMap != map) {
        m_currentMap = map;
        emit currentMapChanged();
    }
}

void EsportsMatchEngine::setTeamAName(const QString &name) {
    if (m_teamAName != name) {
        m_teamAName = name;
        emit teamAChanged();
    }
}

void EsportsMatchEngine::setTeamATag(const QString &tag) {
    if (m_teamATag != tag) {
        m_teamATag = tag;
        emit teamAChanged();
    }
}

void EsportsMatchEngine::setTeamAMaps(int maps) {
    if (m_teamAMaps != maps) {
        m_teamAMaps = maps;
        emit teamAChanged();
    }
}

void EsportsMatchEngine::setTeamARounds(int rounds) {
    if (m_teamARounds != rounds) {
        m_teamARounds = rounds;
        emit teamAChanged();
    }
}

void EsportsMatchEngine::setTeamAColor(const QString &color) {
    if (m_teamAColor != color) {
        m_teamAColor = color;
        emit teamAChanged();
    }
}

void EsportsMatchEngine::setTeamBName(const QString &name) {
    if (m_teamBName != name) {
        m_teamBName = name;
        emit teamBChanged();
    }
}

void EsportsMatchEngine::setTeamBTag(const QString &tag) {
    if (m_teamBTag != tag) {
        m_teamBTag = tag;
        emit teamBChanged();
    }
}

void EsportsMatchEngine::setTeamBMaps(int maps) {
    if (m_teamBMaps != maps) {
        m_teamBMaps = maps;
        emit teamBChanged();
    }
}

void EsportsMatchEngine::setTeamBRounds(int rounds) {
    if (m_teamBRounds != rounds) {
        m_teamBRounds = rounds;
        emit teamBChanged();
    }
}

void EsportsMatchEngine::setTeamBColor(const QString &color) {
    if (m_teamBColor != color) {
        m_teamBColor = color;
        emit teamBChanged();
    }
}

QString EsportsMatchEngine::activeSponsorName() const {
    if (m_sponsorItems.isEmpty()) return "";
    return m_sponsorItems[m_currentSponsorIndex % m_sponsorItems.size()].name;
}

QString EsportsMatchEngine::activeSponsorTagline() const {
    if (m_sponsorItems.isEmpty()) return "";
    return m_sponsorItems[m_currentSponsorIndex % m_sponsorItems.size()].tagline;
}

QString EsportsMatchEngine::activeSponsorLogo() const {
    if (m_sponsorItems.isEmpty()) return "";
    return m_sponsorItems[m_currentSponsorIndex % m_sponsorItems.size()].logoUrl;
}

void EsportsMatchEngine::setSponsorOnAir(bool onAir) {
    if (m_isSponsorOnAir != onAir) {
        m_isSponsorOnAir = onAir;
        emit sponsorOnAirChanged();
    }
}

QVariantList EsportsMatchEngine::sponsorReport() const {
    QVariantList list;
    for (const auto &item : m_sponsorItems) {
        QVariantMap map;
        map["name"] = item.name;
        map["tagline"] = item.tagline;
        map["impressions"] = item.impressionsCount;
        int totalSeconds = item.totalExposureMs / 1000;
        int mins = totalSeconds / 60;
        int secs = totalSeconds % 60;
        map["formattedTime"] = QString("%1:%2")
                                   .arg(mins, 2, 10, QChar('0'))
                                   .arg(secs, 2, 10, QChar('0'));
        map["secondsOnAir"] = totalSeconds;
        list.append(map);
    }
    return list;
}

void EsportsMatchEngine::adjustRoundScore(bool isTeamA, int delta) {
    if (isTeamA) {
        m_teamARounds = std::max(0, m_teamARounds + delta);
    } else {
        m_teamBRounds = std::max(0, m_teamBRounds + delta);
    }
    emit teamAChanged();
    emit teamBChanged();

    // Trigger match point notification if at 12 rounds
    if (m_teamARounds == 12 || m_teamBRounds == 12) {
        m_matchPhase = "MATCH POINT";
        emit matchPhaseChanged();
    }
}

void EsportsMatchEngine::adjustMapScore(bool isTeamA, int delta) {
    if (isTeamA) {
        m_teamAMaps = std::max(0, m_teamAMaps + delta);
    } else {
        m_teamBMaps = std::max(0, m_teamBMaps + delta);
    }
    emit teamAChanged();
    emit teamBChanged();
}

void EsportsMatchEngine::swapSides() {
    std::swap(m_teamAName, m_teamBName);
    std::swap(m_teamATag, m_teamBTag);
    std::swap(m_teamAMaps, m_teamBMaps);
    std::swap(m_teamARounds, m_teamBRounds);
    std::swap(m_teamAColor, m_teamBColor);
    emit teamAChanged();
    emit teamBChanged();
}

void EsportsMatchEngine::resetMatchScores() {
    m_teamAMaps = 0;
    m_teamARounds = 0;
    m_teamBMaps = 0;
    m_teamBRounds = 0;
    m_matchPhase = "LIVE";
    emit teamAChanged();
    emit teamBChanged();
    emit matchPhaseChanged();
}

void EsportsMatchEngine::rotateNextSponsor() {
    if (m_sponsorItems.isEmpty()) return;
    m_currentSponsorIndex = (m_currentSponsorIndex + 1) % m_sponsorItems.size();
    m_sponsorItems[m_currentSponsorIndex].impressionsCount++;
    emit sponsorChanged();
    emit sponsorReportChanged();
}

void EsportsMatchEngine::addSponsor(const QString &name, const QString &tagline, const QString &logoUrl) {
    SponsorItem item;
    item.name = name.trimmed();
    item.tagline = tagline.trimmed();
    item.logoUrl = logoUrl.isEmpty() ? "qrc:/makasna-logo.svg" : logoUrl.trimmed();
    m_sponsorItems.append(item);

    m_sponsors.clear();
    for (const auto &s : m_sponsorItems) {
        QVariantMap map;
        map["name"] = s.name;
        map["tagline"] = s.tagline;
        map["logoUrl"] = s.logoUrl;
        m_sponsors.append(map);
    }
    emit sponsorsListChanged();
    emit sponsorReportChanged();
}

void EsportsMatchEngine::removeSponsor(int index) {
    if (index >= 0 && index < m_sponsorItems.size()) {
        m_sponsorItems.removeAt(index);
        if (m_currentSponsorIndex >= m_sponsorItems.size()) {
            m_currentSponsorIndex = 0;
        }
        m_sponsors.clear();
        for (const auto &s : m_sponsorItems) {
            QVariantMap map;
            map["name"] = s.name;
            map["tagline"] = s.tagline;
            map["logoUrl"] = s.logoUrl;
            m_sponsors.append(map);
        }
        emit sponsorsListChanged();
        emit sponsorChanged();
        emit sponsorReportChanged();
    }
}

void EsportsMatchEngine::setSponsorRotationInterval(int seconds) {
    if (seconds < 5) seconds = 5;
    m_sponsorRotationTimer->setInterval(seconds * 1000);
}

QString EsportsMatchEngine::exportSponsorReportCsv() const {
    QString csv = "Sponsor Name,Tagline,Impressions,Total On-Air Time (Seconds),Formatted Time\n";
    for (const auto &item : m_sponsorItems) {
        int secs = item.totalExposureMs / 1000;
        int m = secs / 60;
        int s = secs % 60;
        QString fmt = QString("%1:%2").arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0'));
        csv += QString("\"%1\",\"%2\",%3,%4,\"%5\"\n")
                   .arg(item.name, item.tagline)
                   .arg(item.impressionsCount)
                   .arg(secs)
                   .arg(fmt);
    }
    return csv;
}

void EsportsMatchEngine::updatePlayerComparison(const QString &pAName, const QString &pATeam, double pAKda, int pAAcs, double pAWinRate,
                                                const QString &pBName, const QString &pBTeam, double pBKda, int pBAcs, double pBWinRate) {
    m_playerComparison["playerAName"] = pAName;
    m_playerComparison["playerATeam"] = pATeam;
    m_playerComparison["playerAKda"] = pAKda;
    m_playerComparison["playerAAcs"] = pAAcs;
    m_playerComparison["playerAWinRate"] = pAWinRate;

    m_playerComparison["playerBName"] = pBName;
    m_playerComparison["playerBTeam"] = pBTeam;
    m_playerComparison["playerBKda"] = pBKda;
    m_playerComparison["playerBAcs"] = pBAcs;
    m_playerComparison["playerBWinRate"] = pBWinRate;

    emit playerComparisonChanged();
}

void EsportsMatchEngine::updatePickBanItem(int index, const QString &mapName, const QString &action, const QString &teamTag) {
    if (index >= 0 && index < m_pickBanList.size()) {
        QVariantMap item = m_pickBanList[index].toMap();
        item["map"] = mapName;
        item["action"] = action;
        item["team"] = teamTag;
        m_pickBanList[index] = item;
        emit pickBanListChanged();
    }
}

bool EsportsMatchEngine::ingestTelemetryJson(const QString &jsonString) {
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (doc.isNull() || !doc.isObject()) return false;

    QJsonObject root = doc.object();
    if (root.contains("tournament")) {
        setTournamentName(root["tournament"].toString());
    }
    if (root.contains("format")) {
        setMatchFormat(root["format"].toString());
    }
    if (root.contains("phase")) {
        setMatchPhase(root["phase"].toString());
    }
    if (root.contains("map")) {
        setCurrentMap(root["map"].toString());
    }

    if (root.contains("teamA") && root["teamA"].isObject()) {
        QJsonObject tA = root["teamA"].toObject();
        if (tA.contains("name")) m_teamAName = tA["name"].toString();
        if (tA.contains("tag")) m_teamATag = tA["tag"].toString();
        if (tA.contains("maps")) m_teamAMaps = tA["maps"].toInt();
        if (tA.contains("rounds")) m_teamARounds = tA["rounds"].toInt();
        emit teamAChanged();
    }

    if (root.contains("teamB") && root["teamB"].isObject()) {
        QJsonObject tB = root["teamB"].toObject();
        if (tB.contains("name")) m_teamBName = tB["name"].toString();
        if (tB.contains("tag")) m_teamBTag = tB["tag"].toString();
        if (tB.contains("maps")) m_teamBMaps = tB["maps"].toInt();
        if (tB.contains("rounds")) m_teamBRounds = tB["rounds"].toInt();
        emit teamBChanged();
    }

    return true;
}

void EsportsMatchEngine::onSponsorTimerTimeout() {
    rotateNextSponsor();
}

void EsportsMatchEngine::onExposureTimerTimeout() {
    if (m_isSponsorOnAir && !m_sponsorItems.isEmpty()) {
        int idx = m_currentSponsorIndex % m_sponsorItems.size();
        m_sponsorItems[idx].totalExposureMs += 1000;
        emit sponsorReportChanged();
    }
}
