import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "MAKASNA Smart Data Hub & Spreadsheet Repeater"
    modal: true
    anchors.centerIn: parent
    width: 720
    height: 500
    standardButtons: Dialog.Close

    background: Rectangle {
        color: "#0f131c"
        border.color: "#1e2536"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 12

        Text {
            text: "DATASETS PENYIARAN & LEADERBOARD"
            font.bold: true
            font.pixelSize: 13
            color: "#00e5ff"
        }

        // Datasets list
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: projectModel.datasets
            delegate: Rectangle {
                width: parent.width
                height: 120
                color: "#111622"
                border.color: "#1e2536"
                radius: 4

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 6

                    Text {
                        text: modelData.name + " (" + (modelData.rows ? modelData.rows.length : 0) + " baris)"
                        font.bold: true
                        font.pixelSize: 13
                        color: "#ffffff"
                    }

                    Text {
                        text: "Kolom: " + (modelData.headers ? modelData.headers.join(", ") : "")
                        font.pixelSize: 11
                        color: "#00e5ff"
                        font.family: "Monospace"
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
