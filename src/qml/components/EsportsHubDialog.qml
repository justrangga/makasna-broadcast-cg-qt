import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "MAKASNA Esports Engine & Sponsor Telemetry Hub"
    modal: true
    anchors.centerIn: parent
    width: 880
    height: 640
    standardButtons: Dialog.Close

    background: Rectangle {
        color: "#0a0d15"
        border.color: "#1e2536"
        border.width: 1
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // Top Status Header
        Rectangle {
            Layout.fillWidth: true
            height: 44
            color: "#101624"
            border.color: "#1e293b"
            radius: 6

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 12

                Text {
                    text: "ESPORTS CONTROLLER & SIGHT ENGINE"
                    font.bold: true
                    font.pixelSize: 12
                    font.letterSpacing: 1.5
                    color: "#00e5ff"
                }

                Rectangle { width: 1; height: 16; color: "#334155" }

                Text {
                    text: esportsEngine.tournamentName + " • " + esportsEngine.matchFormat
                    font.pixelSize: 11
                    color: "#cbd5e1"
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: "SPONSOR ON AIR: " + (esportsEngine.isSponsorOnAir ? "ACTIVE" : "STANDBY")
                    font.bold: true
                    font.pixelSize: 11
                    color: esportsEngine.isSponsorOnAir ? "#00e676" : "#64748b"
                }

                Button {
                    text: esportsEngine.isSponsorOnAir ? "Set Standby" : "Set On-Air"
                    implicitHeight: 26
                    onClicked: esportsEngine.isSponsorOnAir = !esportsEngine.isSponsorOnAir
                }
            }
        }

        // Navigation Tabs
        TabBar {
            id: tabs
            Layout.fillWidth: true
            background: Rectangle { color: "#0f131c" }

            TabButton { text: "Match & Teams"; width: implicitWidth + 20 }
            TabButton { text: "Head-to-Head Stats"; width: implicitWidth + 20 }
            TabButton { text: "Map Veto (Pick/Ban)"; width: implicitWidth + 20 }
            TabButton { text: "Sponsor Engine (Sight)"; width: implicitWidth + 20 }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabs.currentIndex

            // TAB 1: MATCH & TEAMS
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 10

                    // Tournament & Map inputs
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        TextField {
                            id: tourneyInput
                            Layout.fillWidth: true
                            text: esportsEngine.tournamentName
                            placeholderText: "Tournament Name"
                            onEditingFinished: esportsEngine.tournamentName = text
                        }

                        ComboBox {
                            model: ["BO1", "BO3", "BO5", "BO7"]
                            currentIndex: 1
                            onActivated: esportsEngine.matchFormat = currentText
                        }

                        TextField {
                            id: mapInput
                            Layout.preferredWidth: 160
                            text: esportsEngine.currentMap
                            placeholderText: "Current Map"
                            onEditingFinished: esportsEngine.currentMap = text
                        }

                        ComboBox {
                            model: ["MAP 1 - LIVE", "MAP 2 - LIVE", "MAP 3 - LIVE", "MATCH POINT", "TIMEOUT", "HALFTIME", "POST MATCH"]
                            onActivated: esportsEngine.matchPhase = currentText
                        }
                    }

                    // Scoreboard Cards (Team A vs Team B)
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        // Team A Box
                        Rectangle {
                            Layout.fillWidth: true
                            height: 170
                            color: "#111726"
                            border.color: "#00e5ff"
                            radius: 6

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                RowLayout {
                                    Text { text: "TEAM A (CYAN)"; color: "#00e5ff"; font.bold: true; font.pixelSize: 12 }
                                    Item { Layout.fillWidth: true }
                                    Text { text: "Maps: " + esportsEngine.teamAMaps; color: "#ffffff"; font.bold: true }
                                }

                                RowLayout {
                                    TextField {
                                        id: teamANameInput
                                        Layout.fillWidth: true
                                        text: esportsEngine.teamAName
                                        onEditingFinished: esportsEngine.teamAName = text
                                    }
                                    TextField {
                                        id: teamATagInput
                                        Layout.preferredWidth: 80
                                        text: esportsEngine.teamATag
                                        onEditingFinished: esportsEngine.teamATag = text
                                    }
                                }

                                RowLayout {
                                    Text { text: "Rounds Won: "; color: "#94a3b8" }
                                    Text { text: esportsEngine.teamARounds; color: "#ffffff"; font.bold: true; font.pixelSize: 22 }
                                    Item { Layout.fillWidth: true }
                                    Button { text: "-1"; implicitWidth: 36; onClicked: esportsEngine.adjustRoundScore(true, -1) }
                                    Button { text: "+1"; implicitWidth: 36; highlighted: true; onClicked: esportsEngine.adjustRoundScore(true, 1) }
                                }

                                RowLayout {
                                    Text { text: "Maps Won: "; color: "#94a3b8" }
                                    Item { Layout.fillWidth: true }
                                    Button { text: "-1 Map"; implicitWidth: 60; onClicked: esportsEngine.adjustMapScore(true, -1) }
                                    Button { text: "+1 Map"; implicitWidth: 60; onClicked: esportsEngine.adjustMapScore(true, 1) }
                                }
                            }
                        }

                        // Team B Box
                        Rectangle {
                            Layout.fillWidth: true
                            height: 170
                            color: "#111726"
                            border.color: "#ff334b"
                            radius: 6

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                RowLayout {
                                    Text { text: "TEAM B (RED)"; color: "#ff334b"; font.bold: true; font.pixelSize: 12 }
                                    Item { Layout.fillWidth: true }
                                    Text { text: "Maps: " + esportsEngine.teamBMaps; color: "#ffffff"; font.bold: true }
                                }

                                RowLayout {
                                    TextField {
                                        id: teamBNameInput
                                        Layout.fillWidth: true
                                        text: esportsEngine.teamBName
                                        onEditingFinished: esportsEngine.teamBName = text
                                    }
                                    TextField {
                                        id: teamBTagInput
                                        Layout.preferredWidth: 80
                                        text: esportsEngine.teamBTag
                                        onEditingFinished: esportsEngine.teamBTag = text
                                    }
                                }

                                RowLayout {
                                    Text { text: "Rounds Won: "; color: "#94a3b8" }
                                    Text { text: esportsEngine.teamBRounds; color: "#ffffff"; font.bold: true; font.pixelSize: 22 }
                                    Item { Layout.fillWidth: true }
                                    Button { text: "-1"; implicitWidth: 36; onClicked: esportsEngine.adjustRoundScore(false, -1) }
                                    Button { text: "+1"; implicitWidth: 36; highlighted: true; onClicked: esportsEngine.adjustRoundScore(false, 1) }
                                }

                                RowLayout {
                                    Text { text: "Maps Won: "; color: "#94a3b8" }
                                    Item { Layout.fillWidth: true }
                                    Button { text: "-1 Map"; implicitWidth: 60; onClicked: esportsEngine.adjustMapScore(false, -1) }
                                    Button { text: "+1 Map"; implicitWidth: 60; onClicked: esportsEngine.adjustMapScore(false, 1) }
                                }
                            }
                        }
                    }

                    // Actions Row
                    RowLayout {
                        spacing: 10
                        Button {
                            text: "Swap Sides (Switch A/B)"
                            onClicked: esportsEngine.swapSides()
                        }
                        Button {
                            text: "Reset Match Scores"
                            onClicked: esportsEngine.resetMatchScores()
                        }
                        Item { Layout.fillWidth: true }
                    }

                    // Telemetry JSON Ingest
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#0c1018"
                        border.color: "#1e293b"
                        radius: 4

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 6

                            Text {
                                text: "Ingest Game State / Tournament Telemetry JSON"
                                font.bold: true
                                font.pixelSize: 11
                                color: "#00e5ff"
                            }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 8

                                TextField {
                                    id: telemetryJsonInput
                                    Layout.fillWidth: true
                                    placeholderText: "{\"tournament\":\"MKS 2026\",\"teamA\":{\"name\":\"MAKASNA\",\"rounds\":12},\"teamB\":{\"name\":\"APEX\",\"rounds\":10}}"
                                }

                                Button {
                                    text: "Apply Telemetry"
                                    highlighted: true
                                    onClicked: {
                                        if (telemetryJsonInput.text.length > 0) {
                                            esportsEngine.ingestTelemetryJson(telemetryJsonInput.text);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // TAB 2: HEAD TO HEAD STATS
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Text {
                        text: "Player vs Player Head-to-Head Comparison Editor"
                        font.bold: true
                        font.pixelSize: 13
                        color: "#00e5ff"
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 16

                        // Player A Card
                        Rectangle {
                            Layout.fillWidth: true
                            height: 220
                            color: "#111726"
                            radius: 6

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Text { text: "PLAYER A (MKS)"; color: "#00e5ff"; font.bold: true }

                                TextField { id: pAName; Layout.fillWidth: true; text: esportsEngine.playerComparison.playerAName || "RENX"; placeholderText: "Player Name" }
                                TextField { id: pATeam; Layout.fillWidth: true; text: esportsEngine.playerComparison.playerATeam || "MAKASNA"; placeholderText: "Team & Role" }

                                RowLayout {
                                    Text { text: "K/D:"; color: "#94a3b8" }
                                    TextField { id: pAKda; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerAKda || "1.64" }
                                    Text { text: "ACS:"; color: "#94a3b8" }
                                    TextField { id: pAAcs; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerAAcs || "284" }
                                    Text { text: "Win %:"; color: "#94a3b8" }
                                    TextField { id: pAWin; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerAWinRate || "72.5" }
                                }
                            }
                        }

                        // Player B Card
                        Rectangle {
                            Layout.fillWidth: true
                            height: 220
                            color: "#111726"
                            radius: 6

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 12
                                spacing: 8

                                Text { text: "PLAYER B (APX)"; color: "#ff334b"; font.bold: true }

                                TextField { id: pBName; Layout.fillWidth: true; text: esportsEngine.playerComparison.playerBName || "VIPERZ"; placeholderText: "Player Name" }
                                TextField { id: pBTeam; Layout.fillWidth: true; text: esportsEngine.playerComparison.playerBTeam || "APEX"; placeholderText: "Team & Role" }

                                RowLayout {
                                    Text { text: "K/D:"; color: "#94a3b8" }
                                    TextField { id: pBKda; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerBKda || "1.32" }
                                    Text { text: "ACS:"; color: "#94a3b8" }
                                    TextField { id: pBAcs; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerBAcs || "238" }
                                    Text { text: "Win %:"; color: "#94a3b8" }
                                    TextField { id: pBWin; Layout.preferredWidth: 60; text: esportsEngine.playerComparison.playerBWinRate || "64.0" }
                                }
                            }
                        }
                    }

                    Button {
                        text: "Update Head-to-Head Comparison"
                        highlighted: true
                        onClicked: {
                            esportsEngine.updatePlayerComparison(
                                pAName.text, pATeam.text, parseFloat(pAKda.text), parseInt(pAAcs.text), parseFloat(pAWin.text),
                                pBName.text, pBTeam.text, parseFloat(pBKda.text), parseInt(pBAcs.text), parseFloat(pBWin.text)
                            )
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // TAB 3: MAP VETO
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Text {
                        text: "Tournament Map Veto / Pick & Ban Sequence"
                        font.bold: true
                        font.pixelSize: 13
                        color: "#00e5ff"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: esportsEngine.pickBanList
                        spacing: 6
                        delegate: Rectangle {
                            width: parent.width
                            height: 48
                            color: "#111726"
                            radius: 4

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 12

                                Text { text: "Slot " + (index + 1); font.bold: true; color: "#64748b" }
                                Text { text: modelData.map; font.bold: true; color: "#ffffff"; Layout.preferredWidth: 100 }
                                Text { text: modelData.action; font.bold: true; color: modelData.action === "BAN" ? "#ef4444" : "#00e5ff"; Layout.preferredWidth: 80 }
                                Text { text: modelData.team; color: "#94a3b8"; Layout.preferredWidth: 80 }
                                Text { text: "Status: " + modelData.status; color: "#cbd5e1"; Layout.fillWidth: true }
                            }
                        }
                    }
                }
            }

            // TAB 4: SPONSOR ENGINE (BARRACKS SIGHT)
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    // Top: Sponsor Controls
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Text {
                            text: "Active Sponsor: " + esportsEngine.activeSponsorName
                            font.bold: true
                            font.pixelSize: 13
                            color: "#00e5ff"
                        }

                        Button {
                            text: "Next Sponsor (Rotate)"
                            onClicked: esportsEngine.rotateNextSponsor()
                        }

                        Item { Layout.fillWidth: true }

                        Button {
                            text: "Export Proof-of-Play (CSV)"
                            highlighted: true
                            onClicked: {
                                var csv = esportsEngine.exportSponsorReportCsv();
                                console.log("Exported Sponsor Report:\n" + csv);
                            }
                        }
                    }

                    // Add Sponsor Row
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        TextField {
                            id: newSponsorName
                            Layout.preferredWidth: 160
                            placeholderText: "Sponsor Brand"
                        }
                        TextField {
                            id: newSponsorTagline
                            Layout.fillWidth: true
                            placeholderText: "Tagline (e.g. Official Energy Drink)"
                        }
                        Button {
                            text: "Add Sponsor"
                            onClicked: {
                                if (newSponsorName.text.length > 0) {
                                    esportsEngine.addSponsor(newSponsorName.text, newSponsorTagline.text, "")
                                    newSponsorName.text = ""
                                    newSponsorTagline.text = ""
                                }
                            }
                        }
                    }

                    // Proof of Play Report Table (Barracks Sight)
                    Text {
                        text: "LIVE PROOF-OF-PLAY TELEMETRY (ON-AIR EXPOSURE TRACKER)"
                        font.bold: true
                        font.pixelSize: 11
                        font.letterSpacing: 1.2
                        color: "#94a3b8"
                    }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        model: esportsEngine.sponsorReport
                        spacing: 6
                        clip: true
                        delegate: Rectangle {
                            width: parent.width
                            height: 52
                            color: "#111726"
                            border.color: esportsEngine.activeSponsorName === modelData.name ? "#00e5ff" : "#1e293b"
                            border.width: esportsEngine.activeSponsorName === modelData.name ? 2 : 1
                            radius: 4

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 14

                                Rectangle {
                                    width: 10; height: 10; radius: 5
                                    color: esportsEngine.activeSponsorName === modelData.name && esportsEngine.isSponsorOnAir ? "#00e676" : "#64748b"
                                }

                                Text {
                                    text: modelData.name
                                    font.bold: true
                                    font.pixelSize: 14
                                    color: "#ffffff"
                                    Layout.preferredWidth: 160
                                }

                                Text {
                                    text: modelData.tagline
                                    font.pixelSize: 11
                                    color: "#94a3b8"
                                    Layout.fillWidth: true
                                }

                                Text {
                                    text: "Impressions: " + modelData.impressions
                                    font.pixelSize: 12
                                    color: "#cbd5e1"
                                    Layout.preferredWidth: 120
                                }

                                Text {
                                    text: "Total On-Air: " + modelData.formattedTime
                                    font.family: "Monospace"
                                    font.bold: true
                                    font.pixelSize: 14
                                    color: "#00e5ff"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
