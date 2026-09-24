import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 280
    color: "#0f131c"
    border.color: "#1e2536"
    border.width: 1

    property var selectedLayer: projectModel.getSelectedLayer()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        Text {
            text: "PROPERTIES INSPECTOR"
            font.bold: true
            font.pixelSize: 11
            font.letterSpacing: 1.2
            color: "#94a3b8"
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#1e2536"
        }

        // Layer Name & Type
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: "Layer: " + (root.selectedLayer.name || "None")
                font.bold: true
                font.pixelSize: 13
                color: "#00e5ff"
            }

            Text {
                text: "Type: " + (root.selectedLayer.type || "-")
                font.pixelSize: 11
                color: "#64748b"
            }
        }

        // Transform Coordinates (X, Y, W, H)
        GridLayout {
            columns: 2
            Layout.fillWidth: true
            rowSpacing: 8
            columnSpacing: 8

            Text { text: "X (px):"; color: "#94a3b8"; font.pixelSize: 11 }
            TextField {
                text: root.selectedLayer.transform ? String(root.selectedLayer.transform.x) : "0"
                font.pixelSize: 11
                Layout.fillWidth: true
                onEditingFinished: {
                    if (root.selectedLayer.id) {
                        var tr = root.selectedLayer.transform || ({})
                        projectModel.updateLayerTransform(projectModel.activeTemplateId, root.selectedLayer.id, Number(text), tr.y, tr.width, tr.height)
                    }
                }
            }

            Text { text: "Y (px):"; color: "#94a3b8"; font.pixelSize: 11 }
            TextField {
                text: root.selectedLayer.transform ? String(root.selectedLayer.transform.y) : "0"
                font.pixelSize: 11
                Layout.fillWidth: true
                onEditingFinished: {
                    if (root.selectedLayer.id) {
                        var tr = root.selectedLayer.transform || ({})
                        projectModel.updateLayerTransform(projectModel.activeTemplateId, root.selectedLayer.id, tr.x, Number(text), tr.width, tr.height)
                    }
                }
            }

            Text { text: "Width:"; color: "#94a3b8"; font.pixelSize: 11 }
            TextField {
                text: root.selectedLayer.transform ? String(root.selectedLayer.transform.width) : "0"
                font.pixelSize: 11
                Layout.fillWidth: true
                onEditingFinished: {
                    if (root.selectedLayer.id) {
                        var tr = root.selectedLayer.transform || ({})
                        projectModel.updateLayerTransform(projectModel.activeTemplateId, root.selectedLayer.id, tr.x, tr.y, Number(text), tr.height)
                    }
                }
            }

            Text { text: "Height:"; color: "#94a3b8"; font.pixelSize: 11 }
            TextField {
                text: root.selectedLayer.transform ? String(root.selectedLayer.transform.height) : "0"
                font.pixelSize: 11
                Layout.fillWidth: true
                onEditingFinished: {
                    if (root.selectedLayer.id) {
                        var tr = root.selectedLayer.transform || ({})
                        projectModel.updateLayerTransform(projectModel.activeTemplateId, root.selectedLayer.id, tr.x, tr.y, tr.width, Number(text))
                    }
                }
            }
        }

        // Text Content Editor
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4
            visible: root.selectedLayer.type === "text"

            Text { text: "Text Content:"; color: "#94a3b8"; font.pixelSize: 11 }
            TextField {
                Layout.fillWidth: true
                text: (root.selectedLayer.content && root.selectedLayer.content.text) ? root.selectedLayer.content.text : ""
                font.pixelSize: 11
                onEditingFinished: {
                    if (root.selectedLayer.id) {
                        projectModel.updateLayerContent(projectModel.activeTemplateId, root.selectedLayer.id, text)
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Spacer
    }
}
