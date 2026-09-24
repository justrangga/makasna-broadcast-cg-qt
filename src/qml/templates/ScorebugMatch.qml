import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    Rectangle {
        x: 120
        y: 65
        width: 480
        height: 40
        radius: 4
        color: "#0f172a"
        border.color: "#334155"

        Text {
            anchors.centerIn: parent
            text: "IDN  2 - 1  MAS  |  ⏱ 45:00"
            font.bold: true
            font.pixelSize: 18
            color: "#ffffff"
        }
    }
}
