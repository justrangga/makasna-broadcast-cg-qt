import QtQuick

Item {
    id: root
    width: 1920
    height: 1080
    property string headline: "Dr. Ir. Rengga Pratama, M.Kom"
    property string subtitle: "Pakar Penyiaran Digital & AI Developer"

    // Tag Bar
    Rectangle {
        x: 120
        y: 830
        width: 180
        height: 32
        radius: 6
        color: "#ff1744"

        Image {
            source: "qrc:/makasna-logo.svg"
            x: 10
            y: 5
            width: 22
            height: 22
            fillMode: Image.PreserveAspectFit
        }

        Text {
            x: 38
            y: 6
            text: "LIVE EXCLUSIVE"
            color: "#ffffff"
            font.bold: true
            font.pixelSize: 13
        }
    }

    // Headline
    Text {
        x: 140
        y: 874
        text: root.headline
        font.pixelSize: 32
        font.bold: true
        color: "#ffffff"
    }

    // Subtitle
    Text {
        x: 140
        y: 926
        text: root.subtitle
        font.pixelSize: 17
        font.bold: true
        color: "#00e5ff"
    }
}
