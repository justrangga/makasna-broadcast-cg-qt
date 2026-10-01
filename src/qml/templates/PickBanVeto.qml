import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    property var pickBans: esportsEngine.pickBanList
    property string tournament: esportsEngine.tournamentName
    property string sponsorName: esportsEngine.activeSponsorName

    // Center Map Veto Card
    Rectangle {
        anchors.centerIn: parent
        width: 1100
        height: 380
        color: "#0a0e18"
        border.color: "#1e293b"
        border.width: 1
        radius: 8

        // Header
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
                    text: "MAP VETO & PICKS"
                    font.family: "Oswald"
                    font.pixelSize: 20
                    font.bold: true
                    font.letterSpacing: 2
                    color: "#ffffff"
                }

                Rectangle { width: 1; height: 16; anchors.verticalCenter: parent.verticalCenter; color: "#334155" }

                Text {
                    text: root.tournament
                    font.family: "Ubuntu"
                    font.pixelSize: 12
                    font.bold: true
                    color: "#94a3b8"
                }
            }

            Row {
                anchors.right: parent.right
                anchors.rightMargin: 24
                anchors.verticalCenter: parent.verticalCenter
                spacing: 6
                visible: root.sponsorName.length > 0

                Text {
                    text: "OFFICIAL PARTNER"
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

        // Map Cards List
        Row {
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 24
            spacing: 16

            Repeater {
                model: root.pickBans
                delegate: Rectangle {
                    width: 190
                    height: 240
                    color: modelData.status === "BANNED" ? "#0f1118" : (modelData.status === "CURRENT" ? "#101e2b" : "#131926")
                    border.color: modelData.status === "CURRENT" ? "#00e5ff" : (modelData.status === "BANNED" ? "#334155" : "#1e293b")
                    border.width: modelData.status === "CURRENT" ? 2 : 1
                    radius: 6

                    Column {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 10

                        // Action Badge (PICK / BAN / DECIDER)
                        Rectangle {
                            height: 22
                            width: parent.width
                            radius: 3
                            color: modelData.action === "BAN" ? "#991b1b" : (modelData.action === "PICK" ? "#0284c7" : "#d97706")

                            Text {
                                anchors.centerIn: parent
                                text: modelData.action + (modelData.team ? " - " + modelData.team : "")
                                font.family: "Ubuntu"
                                font.pixelSize: 11
                                font.bold: true
                                color: "#ffffff"
                            }
                        }

                        // Map Graphic Box
                        Rectangle {
                            width: parent.width
                            height: 120
                            color: "#1e293b"
                            radius: 4

                            Text {
                                anchors.centerIn: parent
                                text: modelData.map
                                font.family: "Oswald"
                                font.pixelSize: 24
                                font.bold: true
                                color: modelData.status === "BANNED" ? "#64748b" : "#ffffff"
                            }

                            // Ban Red Strike
                            Rectangle {
                                anchors.centerIn: parent
                                width: parent.width
                                height: 3
                                color: "#ef4444"
                                rotation: -30
                                visible: modelData.status === "BANNED"
                            }
                        }

                        // Status Note
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: modelData.status
                            font.family: "Ubuntu"
                            font.pixelSize: 11
                            font.bold: true
                            color: modelData.status === "CURRENT" ? "#00e5ff" : (modelData.status === "DECIDER" ? "#fbbf24" : "#64748b")
                        }
                    }
                }
            }
        }
    }
}
