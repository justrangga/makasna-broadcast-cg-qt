import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    Image {
        x: 1680
        y: 65
        width: 52
        height: 52
        source: "qrc:/makasna-logo.svg"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        x: 1740
        y: 80
        text: "MAKASNA"
        font.pixelSize: 16
        font.bold: true
        font.letterSpacing: 2
        color: "#ffffff"
    }
}
