import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    Rectangle {
        x: 460
        y: 200
        width: 1000
        height: 72
        radius: 6
        color: "#0284c7"

        Text {
            anchors.centerIn: parent
            text: "MAKASNA GRAND PRIX - KLASEMEN SEMENTARA"
            font.bold: true
            font.pixelSize: 22
            color: "#ffffff"
        }
    }
}
