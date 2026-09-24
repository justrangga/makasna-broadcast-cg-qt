import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#07090e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // Top: Dual Monitor Bus (Preview / Program)
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            spacing: 16

            // PREVIEW MONITOR (PVW - Green Tally)
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0a0e17"
                border.color: "#00e676"
                border.width: 2
                radius: 4

                // Tally Label
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    height: 22
                    width: 100
                    color: "#00e676"
                    Text {
                        anchors.centerIn: parent
                        text: "PREVIEW (PVW)"
                        font.bold: true
                        font.pixelSize: 10
                        font.family: "Monospace"
                        color: "#000000"
                    }
                }

                // Cued Item Info
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 6
                    Text {
                        text: "CUED ITEM: " + playoutController.cuedItemId
                        font.bold: true
                        font.pixelSize: 15
                        color: "#00e676"
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Text {
                        text: "Ready to TAKE On-Air"
                        font.pixelSize: 12
                        color: "#94a3b8"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }

            // PROGRAM MONITOR (PGM - Red Tally)
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0a0e17"
                border.color: "#ff334b"
                border.width: 2
                radius: 4

                // Tally Label
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    height: 22
                    width: 110
                    color: "#ff334b"
                    Text {
                        anchors.centerIn: parent
                        text: "● PROGRAM (LIVE)"
                        font.bold: true
                        font.pixelSize: 10
                        font.family: "Monospace"
                        color: "#ffffff"
                    }
                }

                // Active Layers Status
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 6

                    Text {
                        text: "BROADCAST SDI MASTER OUTPUT"
                        font.bold: true
                        font.pixelSize: 15
                        color: "#ff334b"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    RowLayout {
                        spacing: 8
                        Layout.alignment: Qt.AlignHCenter

                        Rectangle {
                            width: 60; height: 24; radius: 3
                            color: playoutController.activeLayers.L1 ? "#ff1744" : "#1e293b"
                            Text { anchors.centerIn: parent; text: "L1"; font.bold: true; color: "#ffffff"; font.pixelSize: 11 }
                        }
                        Rectangle {
                            width: 60; height: 24; radius: 3
                            color: playoutController.activeLayers.L2 ? "#ff1744" : "#1e293b"
                            Text { anchors.centerIn: parent; text: "L2"; font.bold: true; color: "#ffffff"; font.pixelSize: 11 }
                        }
                        Rectangle {
                            width: 60; height: 24; radius: 3
                            color: playoutController.activeLayers.L3 ? "#ff1744" : "#1e293b"
                            Text { anchors.centerIn: parent; text: "L3"; font.bold: true; color: "#ffffff"; font.pixelSize: 11 }
                        }
                        Rectangle {
                            width: 60; height: 24; radius: 3
                            color: playoutController.activeLayers.L4 ? "#ff1744" : "#1e293b"
                            Text { anchors.centerIn: parent; text: "L4"; font.bold: true; color: "#ffffff"; font.pixelSize: 11 }
                        }
                    }
                }
            }
        }

        // Center: Master Playout Control Bar
        RowLayout {
            Layout.fillWidth: true
            height: 48
            spacing: 12

            Button {
                text: "▶ TAKE (SPACEBAR)"
                Layout.preferredWidth: 200
                Layout.fillHeight: true
                font.bold: true
                font.pixelSize: 14
                highlighted: true
                onClicked: playoutController.take()
            }

            Button {
                text: "⏹ CLEAR ALL (ESC)"
                Layout.preferredWidth: 160
                Layout.fillHeight: true
                font.bold: true
                font.pixelSize: 13
                onClicked: playoutController.clearAll()
            }

            Rectangle { width: 1; Layout.fillHeight: true; color: "#1e2536" }

            // Layer Clear Buttons
            Button { text: "CLR L1"; onClicked: playoutController.clearLayer("L1") }
            Button { text: "CLR L2"; onClicked: playoutController.clearLayer("L2") }
            Button { text: "CLR L3"; onClicked: playoutController.clearLayer("L3") }
            Button { text: "CLR L4"; onClicked: playoutController.clearLayer("L4") }

            Item { Layout.fillWidth: true } // Spacer

            // Match Clock Box
            Rectangle {
                width: 180
                height: 40
                color: "#111622"
                radius: 4
                border.color: "#1e2536"

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 8
                    Text {
                        text: "⏱ " + playoutController.clockString
                        font.family: "Monospace"
                        font.bold: true
                        font.pixelSize: 16
                        color: playoutController.isClockRunning ? "#00e676" : "#f59e0b"
                    }
                    Button {
                        text: playoutController.isClockRunning ? "⏸" : "▶"
                        implicitWidth: 32; implicitHeight: 28
                        onClicked: {
                            if (playoutController.isClockRunning) playoutController.pauseClock()
                            else playoutController.startClock()
                        }
                    }
                }
            }

            // Score Box
            Rectangle {
                width: 140
                height: 40
                color: "#111622"
                radius: 4
                border.color: "#1e2536"

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 6
                    Button { text: "+"; implicitWidth: 24; implicitHeight: 24; onClicked: playoutController.adjustScore(true, 1) }
                    Text { text: playoutController.homeScore + " - " + playoutController.awayScore; font.bold: true; font.pixelSize: 15; color: "#ffffff" }
                    Button { text: "+"; implicitWidth: 24; implicitHeight: 24; onClicked: playoutController.adjustScore(false, 1) }
                }
            }
        }

        // Bottom: Rundown Playlist Table
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f131c"
            border.color: "#1e2536"
            border.width: 1

            ListView {
                anchors.fill: parent
                anchors.margins: 8
                model: projectModel.rundown
                delegate: Rectangle {
                    width: parent.width
                    height: 44
                    color: playoutController.cuedItemId === modelData.id ? "#13231f" : "#111622"
                    border.color: playoutController.cuedItemId === modelData.id ? "#00e676" : "#182030"
                    radius: 4

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12
                        spacing: 12

                        Rectangle {
                            width: 36
                            height: 22
                            radius: 3
                            color: "#1e293b"
                            Text { anchors.centerIn: parent; text: modelData.targetLayer || "L2"; font.bold: true; color: "#00e5ff"; font.pixelSize: 11 }
                        }

                        Text {
                            Layout.fillWidth: true
                            text: modelData.title || ""
                            font.bold: true
                            font.pixelSize: 13
                            color: "#ffffff"
                        }

                        Button {
                            text: "CUE"
                            implicitHeight: 28
                            onClicked: playoutController.cueItem(modelData.id)
                        }

                        Button {
                            text: "TAKE"
                            implicitHeight: 28
                            highlighted: true
                            onClicked: {
                                playoutController.takeItem(modelData.id, modelData.templateId, modelData.targetLayer)
                            }
                        }
                    }
                }
            }
        }
    }
}
