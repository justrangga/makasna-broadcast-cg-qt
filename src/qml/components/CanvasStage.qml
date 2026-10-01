import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#07090e"
    clip: true

    property double zoom: 0.65
    property var activeTemplate: projectModel.getActiveTemplate()
    property string selectedLayerId: projectModel.selectedLayerId

    // Canvas Container (1920x1080 scaled by zoom)
    DropArea {
        id: canvasDropArea
        width: 1920 * root.zoom
        height: 1080 * root.zoom
        anchors.centerIn: parent

        onDropped: function(drop) {
            if (drop.hasUrls) {
                for (var i = 0; i < drop.urls.length; i++) {
                    var url = drop.urls[i];
                    var type = assetManager.detectAssetType(url);
                    assetManager.importAsset(url);
                    projectModel.addMediaLayer("Media Asset", url, type, 1);
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            color: canvasDropArea.containsDrag ? "#141c2b" : "#0a0e17"
            border.color: canvasDropArea.containsDrag ? "#00e5ff" : "#1e293b"
            border.width: canvasDropArea.containsDrag ? 2 : 1

            // 1920x1080 Unscaled Coordinate Root
            Item {
                id: stage1080p
                width: 1920
                height: 1080
                scale: root.zoom
                transformOrigin: Item.TopLeft

                // Render Layers
                Repeater {
                    model: root.activeTemplate ? (root.activeTemplate.layers || []) : []
                    delegate: Item {
                        id: layerItem
                        property var layerData: modelData
                        property var tr: layerData.transform || ({})
                        property bool isSelected: root.selectedLayerId === layerData.id

                        x: tr.x || 0
                        y: tr.y || 0
                        width: tr.width || 100
                        height: tr.height || 40
                        visible: layerData.visible !== false

                        // Visual Content (Shape, Image, Video, Lottie, or Text)
                        Rectangle {
                            anchors.fill: parent
                            visible: layerData.type === "shape"
                            color: (layerData.style && layerData.style.fill) ? layerData.style.fill : "transparent"
                            radius: (layerData.style && layerData.style.borderRadius) ? layerData.style.borderRadius : 0
                        }

                        Image {
                            anchors.fill: parent
                            visible: layerData.type === "image" || layerData.type === "svg" || (layerData.mediaType === "image" || layerData.mediaType === "svg")
                            source: layerData.source ? (layerData.source.indexOf(":") !== -1 ? layerData.source : "file:///" + layerData.source) : ((layerData.content && layerData.content.src) ? layerData.content.src : "")
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            mipmap: true
                        }

                        Rectangle {
                            anchors.fill: parent
                            visible: layerData.type === "video" || layerData.type === "lottie" || layerData.mediaType === "video" || layerData.mediaType === "lottie"
                            color: "#1e1e2f"
                            border.color: layerData.mediaType === "lottie" ? "#8b5cf6" : "#3b82f6"
                            border.width: 1
                            radius: 4

                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 4

                                Text {
                                    text: layerData.mediaType === "lottie" ? "[AE LOTTIE MOTION]" : "[BROADCAST VIDEO]"
                                    font.bold: true
                                    font.pixelSize: 14
                                    color: layerData.mediaType === "lottie" ? "#c084fc" : "#60a5fa"
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                Text {
                                    text: layerData.name || layerData.source || ""
                                    font.pixelSize: 11
                                    color: "#94a3b8"
                                    horizontalAlignment: Text.AlignHCenter
                                    elide: Text.ElideMiddle
                                }
                            }
                        }

                        Text {
                            anchors.fill: parent
                            visible: layerData.type === "text"
                            text: (layerData.content && layerData.content.text) ? layerData.content.text : ""
                            color: (layerData.style && layerData.style.fill) ? layerData.style.fill : "#ffffff"
                            font.pixelSize: (layerData.style && layerData.style.fontSize) ? layerData.style.fontSize : 20
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                        }

                        // Selected Bounding Box & Handles
                        Rectangle {
                            anchors.fill: parent
                            visible: layerItem.isSelected
                            color: "transparent"
                            border.color: "#00e5ff"
                            border.width: 2

                            // 4 Corner Dots
                            Rectangle { width: 8; height: 8; color: "#ffffff"; border.color: "#00e5ff"; anchors.left: parent.left; anchors.top: parent.top; anchors.margins: -4 }
                            Rectangle { width: 8; height: 8; color: "#ffffff"; border.color: "#00e5ff"; anchors.right: parent.right; anchors.top: parent.top; anchors.margins: -4 }
                            Rectangle { width: 8; height: 8; color: "#ffffff"; border.color: "#00e5ff"; anchors.left: parent.left; anchors.bottom: parent.bottom; anchors.margins: -4 }
                            Rectangle { width: 8; height: 8; color: "#ffffff"; border.color: "#00e5ff"; anchors.right: parent.right; anchors.bottom: parent.bottom; anchors.margins: -4 }

                            // Layer Tag
                            Rectangle {
                                anchors.bottom: parent.top
                                anchors.left: parent.left
                                anchors.bottomMargin: 4
                                height: 18
                                width: tagText.contentWidth + 12
                                color: "#00e5ff"
                                radius: 2

                                Text {
                                    id: tagText
                                    anchors.centerIn: parent
                                    text: layerData.name + " (" + Math.round(layerItem.x) + ", " + Math.round(layerItem.y) + ")"
                                    font.pixelSize: 10
                                    font.bold: true
                                    font.family: "Monospace"
                                    color: "#000000"
                                }
                            }
                        }

                        // Direct Mouse Drag & Drop
                        MouseArea {
                            id: dragArea
                            anchors.fill: parent
                            cursorShape: layerItem.isSelected ? Qt.SizeAllCursor : Qt.PointingHandCursor
                            property real startMouseX: 0
                            property real startMouseY: 0
                            property real initialX: 0
                            property real initialY: 0

                            onPressed: (mouse) => {
                                projectModel.selectedLayerId = layerData.id
                                startMouseX = mouse.x
                                startMouseY = mouse.y
                                initialX = layerItem.x
                                initialY = layerItem.y
                            }

                            onPositionChanged: (mouse) => {
                                if (pressed && !layerData.locked) {
                                    var dx = (mouse.x - startMouseX)
                                    var dy = (mouse.y - startMouseY)
                                    projectModel.moveLayer(projectModel.activeTemplateId, layerData.id, dx, dy)
                                }
                            }
                        }
                    }
                }

                // Safe Zones (SMPTE Action 90% and Title 80%)
                SafeZoneOverlay {
                    showActionSafe: true
                    showTitleSafe: true
                    showCrosshair: true
                }
            }
        }
    }
}
