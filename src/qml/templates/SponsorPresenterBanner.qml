import QtQuick

Item {
    id: root
    width: 1920
    height: 1080

    property string sponsorName: esportsEngine.activeSponsorName
    property string sponsorTagline: esportsEngine.activeSponsorTagline
    property string sponsorLogo: esportsEngine.activeSponsorLogo

    // Bottom Left Sponsor Strap (L2/L3 Bus)
    Rectangle {
        x: 120
        y: 920
        width: 480
        height: 54
        color: "#0a0e18"
        border.color: "#1e293b"
        border.width: 1
        radius: 6

        // Left Glow Accent
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 4
            color: "#00e5ff"
            radius: 2
        }

        Row {
            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16
            spacing: 14

            // Logo
            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: root.sponsorLogo
                sourceSize.width: 32
                sourceSize.height: 32
                fillMode: Image.PreserveAspectFit
            }

            Rectangle { width: 1; height: 26; anchors.verticalCenter: parent.verticalCenter; color: "#1e293b" }

            Column {
                anchors.verticalCenter: parent.verticalCenter
                spacing: 2

                Text {
                    text: root.sponsorName
                    font.family: "Oswald"
                    font.pixelSize: 16
                    font.bold: true
                    font.letterSpacing: 1.2
                    color: "#ffffff"
                }

                Text {
                    text: root.sponsorTagline
                    font.family: "Ubuntu"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#00e5ff"
                }
            }
        }
    }
}
