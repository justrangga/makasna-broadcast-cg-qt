import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    // Properties bound to esportsEngine
    property string tournament: esportsEngine.tournamentName
    property string matchFormat: esportsEngine.matchFormat
    property string currentMap: esportsEngine.currentMap
    property string matchPhase: esportsEngine.matchPhase

    property string teamAName: esportsEngine.teamAName
    property string teamATag: esportsEngine.teamATag
    property int teamAMaps: esportsEngine.teamAMaps
    property int teamARounds: esportsEngine.teamARounds
    property string teamAColor: esportsEngine.teamAColor

    property string teamBName: esportsEngine.teamBName
    property string teamBTag: esportsEngine.teamBTag
    property int teamBMaps: esportsEngine.teamBMaps
    property int teamBRounds: esportsEngine.teamBRounds
    property string teamBColor: esportsEngine.teamBColor

    property string sponsorName: esportsEngine.activeSponsorName

    // Main Top Center Scorebug Container
    Item {
        anchors.top: parent.top
        anchors.topMargin: 40
        anchors.horizontalCenter: parent.horizontalCenter
        width: 760
        height: 64

        // Top Metadata Strip (Tournament, Map, Format)
        Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 440
            height: 20
            color: "#080c14"
            radius: 3

            Row {
                anchors.centerIn: parent
                spacing: 12

                Text {
                    text: root.tournament
                    font.family: "Ubuntu"
                    font.pixelSize: 10
                    font.bold: true
                    font.letterSpacing: 1.2
                    color: "#94a3b8"
                }

                Rectangle { width: 1; height: 10; anchors.verticalCenter: parent.verticalCenter; color: "#334155" }

                Text {
                    text: root.currentMap + " (" + root.matchFormat + ")"
                    font.family: "Ubuntu"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#00e5ff"
                }

                Rectangle { width: 1; height: 10; anchors.verticalCenter: parent.verticalCenter; color: "#334155" }

                Text {
                    text: root.matchPhase
                    font.family: "Ubuntu"
                    font.pixelSize: 10
                    font.bold: true
                    color: root.matchPhase.indexOf("MATCH POINT") !== -1 ? "#f59e0b" : "#cbd5e1"
                }
            }
        }

        // Main Scoreboard Bar
        Rectangle {
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            width: 760
            height: 44
            color: "#0c101c"
            border.color: "#1e293b"
            border.width: 1
            radius: 4

            Row {
                anchors.fill: parent

                // TEAM A SECTION
                Rectangle {
                    width: 320
                    height: parent.height
                    color: "transparent"

                    // Team A Color Stripe
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 5
                        color: root.teamAColor
                    }

                    // Team A Maps Won Indicator
                    Row {
                        anchors.left: parent.left
                        anchors.leftMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 4

                        Repeater {
                            model: 2
                            Rectangle {
                                width: 8
                                height: 8
                                radius: 2
                                color: index < root.teamAMaps ? root.teamAColor : "#1e293b"
                            }
                        }
                    }

                    // Team A Tag & Name
                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 46
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.teamATag
                        font.family: "Oswald"
                        font.pixelSize: 22
                        font.bold: true
                        color: "#ffffff"
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 100
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.teamAName
                        font.family: "Ubuntu"
                        font.pixelSize: 12
                        font.bold: true
                        color: "#94a3b8"
                        elide: Text.ElideRight
                        width: 130
                    }

                    // Team A Round Score Box
                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 54
                        color: "#111827"

                        Text {
                            anchors.centerIn: parent
                            text: root.teamARounds
                            font.family: "Oswald"
                            font.pixelSize: 26
                            font.bold: true
                            color: "#ffffff"
                        }
                    }
                }

                // VS DIVIDER
                Rectangle {
                    width: 120
                    height: parent.height
                    color: "#080b12"

                    Column {
                        anchors.centerIn: parent
                        spacing: 2
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "VS"
                            font.family: "Oswald"
                            font.pixelSize: 14
                            font.bold: true
                            color: "#64748b"
                        }
                    }
                }

                // TEAM B SECTION
                Rectangle {
                    width: 320
                    height: parent.height
                    color: "transparent"

                    // Team B Round Score Box
                    Rectangle {
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 54
                        color: "#111827"

                        Text {
                            anchors.centerIn: parent
                            text: root.teamBRounds
                            font.family: "Oswald"
                            font.pixelSize: 26
                            font.bold: true
                            color: "#ffffff"
                        }
                    }

                    // Team B Name & Tag
                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: 100
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.teamBName
                        font.family: "Ubuntu"
                        font.pixelSize: 12
                        font.bold: true
                        color: "#94a3b8"
                        horizontalAlignment: Text.AlignRight
                        elide: Text.ElideLeft
                        width: 130
                    }

                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: 46
                        anchors.verticalCenter: parent.verticalCenter
                        text: root.teamBTag
                        font.family: "Oswald"
                        font.pixelSize: 22
                        font.bold: true
                        color: "#ffffff"
                    }

                    // Team B Maps Won Indicator
                    Row {
                        anchors.right: parent.right
                        anchors.rightMargin: 16
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 4

                        Repeater {
                            model: 2
                            Rectangle {
                                width: 8
                                height: 8
                                radius: 2
                                color: index < root.teamBMaps ? root.teamBColor : "#1e293b"
                            }
                        }
                    }

                    // Team B Color Stripe
                    Rectangle {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: 5
                        color: root.teamBColor
                    }
                }
            }
        }

        // Bottom Rotating Sponsor Tag (Barracks Controller Sponsor Rotation)
        Rectangle {
            anchors.top: parent.top
            anchors.topMargin: 66
            anchors.horizontalCenter: parent.horizontalCenter
            width: 260
            height: 18
            color: "#080c14"
            radius: 3
            border.color: "#1e2536"
            border.width: 1
            visible: root.sponsorName.length > 0

            Row {
                anchors.centerIn: parent
                spacing: 6

                Text {
                    text: "PRESENTED BY"
                    font.family: "Ubuntu"
                    font.pixelSize: 9
                    font.bold: true
                    color: "#64748b"
                }

                Text {
                    text: root.sponsorName
                    font.family: "Ubuntu"
                    font.pixelSize: 9
                    font.bold: true
                    color: "#00e5ff"
                }
            }
        }
    }
}
