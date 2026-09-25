import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    height: 48
    color: "#0f131c"
    border.color: "#1e2536"
    border.width: 1

    property string workspaceMode: "designer"
    signal modeChanged(string newMode)
    signal openHardwareDialog()
    signal openDataHubDialog()

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 16

        // Brand Logo
        RowLayout {
            spacing: 10
            Image {
                source: "qrc:/makasna-logo.svg"
                sourceSize.width: 28
                sourceSize.height: 28
                fillMode: Image.PreserveAspectFit
            }

            Text {
                text: "MAKASNA <font color='#00e5ff'>NATIVE CG</font>"
                textFormat: Text.RichText
                font.bold: true
                font.pixelSize: 14
                font.letterSpacing: 1.5
                color: "#ffffff"
            }
        }

        // Workspace Mode Switcher
        Rectangle {
            height: 32
            width: 240
            color: "#090c12"
            radius: 6
            border.color: "#1e2536"

            RowLayout {
                anchors.fill: parent
                spacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 5
                    color: root.workspaceMode === "designer" ? "#1e293b" : "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "Designer"
                        font.pixelSize: 12
                        font.bold: true
                        color: root.workspaceMode === "designer" ? "#00e5ff" : "#94a3b8"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.modeChanged("designer")
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 5
                    color: root.workspaceMode === "playout" ? "#1e293b" : "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "Playout"
                        font.pixelSize: 12
                        font.bold: true
                        color: root.workspaceMode === "playout" ? "#ff334b" : "#94a3b8"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.modeChanged("playout")
                    }
                }
            }
        }

        Item { Layout.fillWidth: true } // Spacer

        // Smart Data Hub Button
        Button {
            text: "Smart Data Hub"
            font.pixelSize: 12
            onClicked: root.openDataHubDialog()
        }

        // Hardware Output Dialog Button
        Button {
            text: "Hardware Output (DeckLink)"
            font.pixelSize: 12
            onClicked: root.openHardwareDialog()
        }

        // Telemetry Badges
        Rectangle {
            height: 26
            width: 140
            color: "#090c12"
            radius: 4
            border.color: "#1e2536"

            RowLayout {
                anchors.centerIn: parent
                spacing: 6

                Rectangle {
                    width: 7
                    height: 7
                    radius: 3.5
                    color: decklinkController.isStreaming ? "#00e676" : "#00e5ff"
                }

                Text {
                    text: decklinkController.isStreaming ? "SDI ON-AIR" : "NATIVE 60FPS"
                    font.pixelSize: 10
                    font.bold: true
                    font.family: "Monospace"
                    color: decklinkController.isStreaming ? "#00e676" : "#00e5ff"
                }
            }
        }
    }
}
