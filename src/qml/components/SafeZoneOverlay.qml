import QtQuick

Item {
    id: root
    anchors.fill: parent
    property bool showActionSafe: true
    property bool showTitleSafe: true
    property bool showCrosshair: true

    // Action Safe (90%)
    Rectangle {
        visible: root.showActionSafe
        anchors.fill: parent
        anchors.margins: parent.width * 0.05
        color: "transparent"
        border.color: Qt.rgba(0, 0.9, 0.4, 0.4)
        border.width: 1

        Text {
            text: "ACTION SAFE (90%)"
            font.pixelSize: 10
            font.bold: true
            font.family: "Monospace"
            color: Qt.rgba(0, 0.9, 0.4, 0.8)
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 4
        }
    }

    // Title Safe (80%)
    Rectangle {
        visible: root.showTitleSafe
        anchors.fill: parent
        anchors.margins: parent.width * 0.10
        color: "transparent"
        border.color: Qt.rgba(1.0, 0.7, 0.0, 0.4)
        border.width: 1

        Text {
            text: "TITLE SAFE (80%)"
            font.pixelSize: 10
            font.bold: true
            font.family: "Monospace"
            color: Qt.rgba(1.0, 0.7, 0.0, 0.8)
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 4
        }
    }

    // Center Crosshair
    Rectangle {
        visible: root.showCrosshair
        width: 1
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: Qt.rgba(0, 0.89, 1.0, 0.25)
    }

    Rectangle {
        visible: root.showCrosshair
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        color: Qt.rgba(0, 0.89, 1.0, 0.25)
    }
}
