import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    property var comp: esportsEngine.playerComparison
    property string sponsorName: esportsEngine.activeSponsorName

    // Center Comparison Card
    Rectangle {
        anchors.centerIn: parent
        width: 1000
        height: 480
        color: "#0a0e18"
        border.color: "#1e293b"
        border.width: 1
        radius: 8

        // Top Header Bar
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 52
            color: "#0f172a"
            radius: 8

            Row {
                anchors.left: parent.left
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                spacing: 12

                Text {
                    text: "HEAD TO HEAD COMPARISON"
                    font.family: "Oswald"
                    font.pixelSize: 20
                    font.bold: true
                    font.letterSpacing: 2
                    color: "#ffffff"
                }

                Rectangle { width: 1; height: 16; anchors.verticalCenter: parent.verticalCenter; color: "#334155" }

                Text {
                    text: "MATCH TELEMETRY"
                    font.family: "Ubuntu"
                    font.pixelSize: 12
                    font.bold: true
                    color: "#00e5ff"
                }
            }

            // Sponsor Presenter
            Row {
                anchors.right: parent.right
                anchors.rightMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                spacing: 6
                visible: root.sponsorName.length > 0

                Text {
                    text: "POWERED BY"
                    font.family: "Ubuntu"
                    font.pixelSize: 11
                    font.bold: true
                    color: "#64748b"
                }
                Text {
                    text: root.sponsorName
                    font.family: "Ubuntu"
                    font.pixelSize: 11
                    font.bold: true
                    color: "#00e5ff"
                }
            }
        }

        // Main Comparison Content
        Item {
            anchors.top: parent.top
            anchors.topMargin: 52
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            // Left Player A
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 280
                color: "transparent"

                Column {
                    anchors.centerIn: parent
                    spacing: 8

                    Rectangle {
                        width: 100
                        height: 100
                        radius: 50
                        color: "#1e293b"
                        border.color: "#00e5ff"
                        border.width: 3
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            anchors.centerIn: parent
                            text: (root.comp.playerAName || "P1").substring(0, 2)
                            font.family: "Oswald"
                            font.pixelSize: 32
                            font.bold: true
                            color: "#ffffff"
                        }
                    }

                    Text {
                        text: root.comp.playerAName || "PLAYER 1"
                        font.family: "Oswald"
                        font.pixelSize: 26
                        font.bold: true
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: (root.comp.playerATeam || "TEAM A") + " • " + (root.comp.playerARole || "DUELIST")
                        font.family: "Ubuntu"
                        font.pixelSize: 12
                        font.bold: true
                        color: "#00e5ff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }

            // Center Stats Comparison Bars
            Column {
                anchors.centerIn: parent
                width: 420
                spacing: 24

                // Stat 1: K/D Ratio
                Column {
                    width: parent.width
                    spacing: 6

                    Row {
                        width: parent.width
                        Text { text: (root.comp.playerAKda || 1.64).toFixed(2); font.bold: true; font.pixelSize: 16; color: "#00e5ff" }
                        Item { width: 340 }
                        Text { text: (root.comp.playerBKda || 1.32).toFixed(2); font.bold: true; font.pixelSize: 16; color: "#ff334b"; anchors.right: parent.right }
                    }

                    Text {
                        text: "K / D RATIO"
                        font.family: "Ubuntu"
                        font.pixelSize: 11
                        font.bold: true
                        color: "#94a3b8"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        width: parent.width
                        spacing: 8
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.85; height: parent.height; radius: 4; color: "#00e5ff"; anchors.right: parent.right }
                        }
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.65; height: parent.height; radius: 4; color: "#ff334b" }
                        }
                    }
                }

                // Stat 2: ACS (Average Combat Score)
                Column {
                    width: parent.width
                    spacing: 6

                    Row {
                        width: parent.width
                        Text { text: root.comp.playerAAcs || 284; font.bold: true; font.pixelSize: 16; color: "#00e5ff" }
                        Item { width: 340 }
                        Text { text: root.comp.playerBAcs || 238; font.bold: true; font.pixelSize: 16; color: "#ff334b"; anchors.right: parent.right }
                    }

                    Text {
                        text: "AVERAGE COMBAT SCORE (ACS)"
                        font.family: "Ubuntu"
                        font.pixelSize: 11
                        font.bold: true
                        color: "#94a3b8"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        width: parent.width
                        spacing: 8
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.78; height: parent.height; radius: 4; color: "#00e5ff"; anchors.right: parent.right }
                        }
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.62; height: parent.height; radius: 4; color: "#ff334b" }
                        }
                    }
                }

                // Stat 3: Win Rate
                Column {
                    width: parent.width
                    spacing: 6

                    Row {
                        width: parent.width
                        Text { text: (root.comp.playerAWinRate || 72.5).toFixed(1) + "%"; font.bold: true; font.pixelSize: 16; color: "#00e5ff" }
                        Item { width: 340 }
                        Text { text: (root.comp.playerBWinRate || 64.0).toFixed(1) + "%"; font.bold: true; font.pixelSize: 16; color: "#ff334b"; anchors.right: parent.right }
                    }

                    Text {
                        text: "MAP WIN RATE"
                        font.family: "Ubuntu"
                        font.pixelSize: 11
                        font.bold: true
                        color: "#94a3b8"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Row {
                        width: parent.width
                        spacing: 8
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.72; height: parent.height; radius: 4; color: "#00e5ff"; anchors.right: parent.right }
                        }
                        Rectangle { width: 206; height: 8; radius: 4; color: "#1e293b";
                            Rectangle { width: parent.width * 0.64; height: parent.height; radius: 4; color: "#ff334b" }
                        }
                    }
                }
            }

            // Right Player B
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 280
                color: "transparent"

                Column {
                    anchors.centerIn: parent
                    spacing: 8

                    Rectangle {
                        width: 100
                        height: 100
                        radius: 50
                        color: "#1e293b"
                        border.color: "#ff334b"
                        border.width: 3
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            anchors.centerIn: parent
                            text: (root.comp.playerBName || "P2").substring(0, 2)
                            font.family: "Oswald"
                            font.pixelSize: 32
                            font.bold: true
                            color: "#ffffff"
                        }
                    }

                    Text {
                        text: root.comp.playerBName || "PLAYER 2"
                        font.family: "Oswald"
                        font.pixelSize: 26
                        font.bold: true
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    Text {
                        text: (root.comp.playerBTeam || "TEAM B") + " • " + (root.comp.playerBRole || "INITIATOR")
                        font.family: "Ubuntu"
                        font.pixelSize: 12
                        font.bold: true
                        color: "#ff334b"
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
}
