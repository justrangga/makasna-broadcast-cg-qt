import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    Rectangle {
        y: 1032
        width: 1920
        height: 48
        color: "#0f172a"
        border.color: "#334155"

        Rectangle {
            width: 200
            height: parent.height
            color: "#ff1744"
            Text {
                anchors.centerIn: parent
                text: "BREAKING NEWS"
                font.bold: true
                color: "#ffffff"
            }
        }

        Text {
            x: 220
            anchors.verticalCenter: parent.verticalCenter
            text: "+++ MAKASNA BROADCAST CG C++20 / QT6 NATIVE PLAYOUT ENGINE AKTIF +++"
            font.bold: true
            font.pixelSize: 16
            color: "#f8fafc"
        }
    }
}
