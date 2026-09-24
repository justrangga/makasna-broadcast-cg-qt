import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 240
    color: "#0f131c"
    border.color: "#1e2536"
    border.width: 1

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Text {
            text: "TEMPLATES & LAYERS"
            font.bold: true
            font.pixelSize: 11
            font.letterSpacing: 1.2
            color: "#94a3b8"
        }

        // Templates List
        ListView {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            model: projectModel.templates
            delegate: Rectangle {
                width: parent.width
                height: 32
                color: projectModel.activeTemplateId === modelData.id ? "#1e293b" : "transparent"
                radius: 4

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                    text: modelData.name
                    font.pixelSize: 11
                    font.bold: projectModel.activeTemplateId === modelData.id
                    color: projectModel.activeTemplateId === modelData.id ? "#00e5ff" : "#cbd5e1"
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: projectModel.activeTemplateId = modelData.id
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#1e2536"
        }

        Text {
            text: "LAYER ORDER (Z-INDEX)"
            font.bold: true
            font.pixelSize: 11
            color: "#64748b"
        }

        // Layers List
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: projectModel.getActiveTemplate().layers || []
            delegate: Rectangle {
                width: parent.width
                height: 32
                color: projectModel.selectedLayerId === modelData.id ? "#1e293b" : "#111622"
                border.color: "#182030"
                radius: 3

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 8
                    anchors.rightMargin: 8
                    spacing: 6

                    Text {
                        Layout.fillWidth: true
                        text: modelData.name
                        elide: Text.ElideRight
                        font.pixelSize: 11
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
}
