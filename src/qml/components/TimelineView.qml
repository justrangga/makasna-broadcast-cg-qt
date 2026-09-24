import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    height: 240
    color: "#0f131c"
    border.color: "#1e2536"
    border.width: 1

    property double duration: projectModel.duration
    property double currentTime: projectModel.currentTime
    property bool isPlaying: projectModel.isPlaying

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 1. Controls Header
        Rectangle {
            Layout.fillWidth: true
            height: 38
            color: "#141a26"
            border.color: "#1e2536"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                spacing: 8

                // Jump Start
                Button {
                    text: "⏮"
                    implicitWidth: 32
                    implicitHeight: 28
                    onClicked: projectModel.currentTime = 0
                }

                // Step -1 Frame
                Button {
                    text: "◀"
                    implicitWidth: 32
                    implicitHeight: 28
                    onClicked: projectModel.currentTime = Math.max(0, projectModel.currentTime - 0.05)
                }

                // Play / Pause
                Button {
                    text: projectModel.isPlaying ? "⏸" : "▶"
                    implicitWidth: 36
                    implicitHeight: 28
                    highlighted: projectModel.isPlaying
                    onClicked: projectModel.isPlaying = !projectModel.isPlaying
                }

                // Step +1 Frame
                Button {
                    text: "▶"
                    implicitWidth: 32
                    implicitHeight: 28
                    onClicked: projectModel.currentTime = Math.min(root.duration, projectModel.currentTime + 0.05)
                }

                // Timecode Box
                Rectangle {
                    width: 120
                    height: 26
                    color: "#090c12"
                    radius: 4
                    border.color: "#1e2536"

                    Text {
                        anchors.centerIn: parent
                        text: projectModel.currentTime.toFixed(2) + "s / " + root.duration.toFixed(2) + "s"
                        font.family: "Monospace"
                        font.bold: true
                        font.pixelSize: 11
                        color: "#00e5ff"
                    }
                }

                Item { Layout.fillWidth: true } // Spacer

                // Marker Badges
                Rectangle {
                    height: 22
                    width: 90
                    radius: 3
                    color: Qt.rgba(0, 0.9, 0.4, 0.15)
                    border.color: Qt.rgba(0, 0.9, 0.4, 0.4)
                    Text {
                        anchors.centerIn: parent
                        text: "[INTRO] 0.8s"
                        font.family: "Monospace"
                        font.pixelSize: 10
                        font.bold: true
                        color: "#00e676"
                    }
                }

                Rectangle {
                    height: 22
                    width: 90
                    radius: 3
                    color: Qt.rgba(1.0, 0.7, 0.0, 0.15)
                    border.color: Qt.rgba(1.0, 0.7, 0.0, 0.4)
                    Text {
                        anchors.centerIn: parent
                        text: "[HOLD] 4.2s"
                        font.family: "Monospace"
                        font.pixelSize: 10
                        font.bold: true
                        color: "#ffb300"
                    }
                }

                Rectangle {
                    height: 22
                    width: 90
                    radius: 3
                    color: Qt.rgba(1.0, 0.2, 0.3, 0.15)
                    border.color: Qt.rgba(1.0, 0.2, 0.3, 0.4)
                    Text {
                        anchors.centerIn: parent
                        text: "[OUTRO] 5.0s"
                        font.family: "Monospace"
                        font.pixelSize: 10
                        font.bold: true
                        color: "#ff1744"
                    }
                }
            }
        }

        // 2. Tracks & Timeline Scrubber Area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Left: Layers Track Column
            Rectangle {
                Layout.preferredWidth: 200
                Layout.fillHeight: true
                color: "#111622"
                border.color: "#1e2536"

                ListView {
                    anchors.fill: parent
                    anchors.topMargin: 26
                    model: projectModel.getActiveTemplate().layers || []
                    delegate: Rectangle {
                        width: parent.width
                        height: 36
                        color: projectModel.selectedLayerId === modelData.id ? "#1e293b" : "transparent"
                        border.color: "#182030"

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            anchors.rightMargin: 8
                            spacing: 6

                            Rectangle {
                                width: 8
                                height: 8
                                radius: 4
                                color: modelData.type === "text" ? "#38bdf8" : (modelData.type === "shape" ? "#c084fc" : "#34d399")
                            }

                            Text {
                                Layout.fillWidth: true
                                text: modelData.name || ""
                                elide: Text.ElideRight
                                font.pixelSize: 11
                                font.bold: projectModel.selectedLayerId === modelData.id
                                color: projectModel.selectedLayerId === modelData.id ? "#00e5ff" : "#cbd5e1"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: projectModel.selectedLayerId = modelData.id
                        }
                    }
                }
            }

            // Right: Ruler and Interactive Scrubbing Track
            Rectangle {
                id: trackArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0a0d14"
                clip: true

                // Top Ruler (0 to duration)
                Rectangle {
                    id: ruler
                    width: parent.width
                    height: 26
                    color: "#131926"
                    border.color: "#1e2536"

                    Repeater {
                        model: 11
                        delegate: Item {
                            property double t: (index / 10.0) * root.duration
                            x: (index / 10.0) * trackArea.width
                            width: 1
                            height: parent.height

                            Rectangle {
                                width: 1
                                height: 8
                                anchors.bottom: parent.bottom
                                color: "#475569"
                            }

                            Text {
                                text: t.toFixed(1) + "s"
                                font.pixelSize: 9
                                font.family: "Monospace"
                                color: "#94a3b8"
                                anchors.bottom: parent.bottom
                                anchors.bottomMargin: 10
                                anchors.left: parent.left
                                anchors.leftMargin: 2
                            }
                        }
                    }

                    // Mouse Drag on Ruler
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.SizeHorCursor
                        onPositionChanged: (mouse) => {
                            if (pressed) {
                                var ratio = Math.max(0, Math.min(1, mouse.x / width))
                                projectModel.currentTime = ratio * root.duration
                            }
                        }
                        onPressed: (mouse) => {
                            var ratio = Math.max(0, Math.min(1, mouse.x / width))
                            projectModel.currentTime = ratio * root.duration
                        }
                    }
                }

                // Mouse Wheel Scrubbing on trackArea
                WheelHandler {
                    onWheel: (event) => {
                        var delta = event.angleDelta.y > 0 ? -0.05 : 0.05
                        projectModel.currentTime = Math.max(0, Math.min(root.duration, projectModel.currentTime + delta))
                    }
                }

                // Playhead Line & Needle (Continuous Smooth Drag)
                Item {
                    id: playhead
                    x: (projectModel.currentTime / Math.max(0.1, root.duration)) * trackArea.width - width / 2
                    width: 24
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    z: 50

                    Rectangle {
                        id: needleHead
                        width: 14
                        height: 14
                        rotation: 45
                        anchors.top: parent.top
                        anchors.topMargin: 6
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#ff1744"
                    }

                    Rectangle {
                        width: 2
                        anchors.top: needleHead.bottom
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "#ff1744"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.SizeHorCursor
                        drag.target: playhead
                        drag.axis: Drag.XAxis
                        drag.minimumX: -width / 2
                        drag.maximumX: trackArea.width - width / 2

                        onPositionChanged: {
                            if (pressed) {
                                var currentCenterX = playhead.x + width / 2
                                var ratio = Math.max(0, Math.min(1, currentCenterX / trackArea.width))
                                projectModel.currentTime = ratio * root.duration
                            }
                        }
                    }
                }
            }
        }
    }

    // Playhead Animation Timer
    Timer {
        interval: 16 // 60fps
        running: projectModel.isPlaying
        repeat: true
        onTriggered: {
            var nextTime = projectModel.currentTime + 0.016
            if (nextTime >= root.duration) {
                projectModel.currentTime = 0
            } else {
                projectModel.currentTime = nextTime
            }
        }
    }
}
