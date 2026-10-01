import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

Dialog {
    id: root
    title: "MAKASNA Media & After Effects Asset Library"
    modal: true
    anchors.centerIn: parent
    width: 860
    height: 600
    standardButtons: Dialog.Close

    background: Rectangle {
        color: "#0b0e14"
        border.color: "#1e2536"
        radius: 8
    }

    FileDialog {
        id: fileDialog
        title: "Pilih File Media atau Animasi After Effects"
        currentFolder: StandardPaths.standardLocations(StandardPaths.DocumentsLocation)[0]
        nameFilters: [
            "Semua Format Penyiaran (*.json *.mp4 *.mov *.webm *.png *.svg *.jpg *.webp *.wav *.mp3)",
            "After Effects Lottie JSON (*.json)",
            "Video Alpha & Stingers (*.mov *.webm *.mp4)",
            "Vektor & Grafis Raster (*.svg *.png *.webp *.jpg)",
            "Audio Broadcast (*.wav *.mp3 *.aac *.flac)"
        ]
        onAccepted: {
            if (selectedFiles && selectedFiles.length > 0) {
                for (var i = 0; i < selectedFiles.length; i++) {
                    assetManager.importAsset(selectedFiles[i]);
                }
            } else if (selectedFile) {
                assetManager.importAsset(selectedFile);
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12

        // Top Header and Description
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: "MAKASNA Broadcast Asset Library"
                    font.bold: true
                    font.pixelSize: 15
                    color: "#00e5ff"
                }

                Text {
                    text: "Import video transparan (MOV/WebM), animasi After Effects Lottie (JSON), vektor SVG, dan grafis raster langsung ke timeline CG."
                    font.pixelSize: 11
                    color: "#94a3b8"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }

            Button {
                text: "Import Media / AE File..."
                onClicked: fileDialog.open()
                contentItem: Text {
                    text: parent.text
                    color: "#050811"
                    font.bold: true
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: "#00e5ff"
                    radius: 4
                }
            }
        }

        // Filter Bar & Drag-and-drop indicator
        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            TabBar {
                id: filterBar
                Layout.fillWidth: true
                currentIndex: 0
                onCurrentIndexChanged: {
                    if (currentIndex === 0) assetManager.filterType = "all";
                    else if (currentIndex === 1) assetManager.filterType = "lottie";
                    else if (currentIndex === 2) assetManager.filterType = "video";
                    else if (currentIndex === 3) assetManager.filterType = "svg";
                    else if (currentIndex === 4) assetManager.filterType = "image";
                    else if (currentIndex === 5) assetManager.filterType = "audio";
                }

                TabButton { text: "Semua (" + assetManager.totalAssets + ")" }
                TabButton { text: "After Effects (JSON)" }
                TabButton { text: "Video Alpha / Stinger" }
                TabButton { text: "Vektor SVG" }
                TabButton { text: "Gambar Raster" }
                TabButton { text: "Audio" }
            }

            Button {
                text: "Reset Sampel"
                onClicked: assetManager.createSampleBroadcastAssets()
                contentItem: Text {
                    text: parent.text
                    color: "#94a3b8"
                    font.pixelSize: 11
                }
                background: Rectangle {
                    color: "#161b26"
                    border.color: "#2d3748"
                    radius: 4
                }
            }
        }

        // Drop Area for direct drag and drop from Windows Explorer
        DropArea {
            id: dropArea
            Layout.fillWidth: true
            Layout.fillHeight: true

            onDropped: function(drop) {
                if (drop.hasUrls) {
                    for (var i = 0; i < drop.urls.length; i++) {
                        assetManager.importAsset(drop.urls[i]);
                    }
                }
            }

            Rectangle {
                anchors.fill: parent
                color: dropArea.containsDrag ? "#1a2536" : "#0d111a"
                border.color: dropArea.containsDrag ? "#00e5ff" : "#1e2536"
                border.width: dropArea.containsDrag ? 2 : 1
                radius: 6

                // Drag indicator overlay if dragging
                Text {
                    anchors.centerIn: parent
                    visible: assetManager.filteredAssets.length === 0
                    text: "Tarik dan lepas file media dari Windows Explorer ke sini, atau klik tombol 'Import Media / AE File...'"
                    color: "#64748b"
                    font.pixelSize: 12
                }

                // Grid View of Assets
                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 8
                    clip: true
                    visible: assetManager.filteredAssets.length > 0

                    GridView {
                        id: assetGrid
                        anchors.fill: parent
                        cellWidth: 260
                        cellHeight: 115
                        model: assetManager.filteredAssets

                        delegate: Item {
                            width: 250
                            height: 105

                            Rectangle {
                                anchors.fill: parent
                                color: "#131824"
                                border.color: "#232b3e"
                                radius: 6

                                ColumnLayout {
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 4

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 6

                                        Rectangle {
                                            width: 80
                                            height: 18
                                            radius: 3
                                            color: {
                                                var t = modelData.mediaType;
                                                if (t === "lottie") return "#8b5cf6";
                                                if (t === "video") return "#3b82f6";
                                                if (t === "svg") return "#10b981";
                                                if (t === "image") return "#f59e0b";
                                                return "#64748b";
                                            }
                                            Text {
                                                anchors.centerIn: parent
                                                text: modelData.typeLabel || "[MEDIA]"
                                                font.pixelSize: 9
                                                font.bold: true
                                                color: "#ffffff"
                                            }
                                        }

                                        Text {
                                            text: modelData.fileSizeFormatted || ""
                                            font.pixelSize: 10
                                            color: "#64748b"
                                            Layout.fillWidth: true
                                            horizontalAlignment: Text.AlignRight
                                        }
                                    }

                                    Text {
                                        text: modelData.name || "Aset Tanpa Nama"
                                        font.bold: true
                                        font.pixelSize: 12
                                        color: "#f8fafc"
                                        elide: Text.ElideRight
                                        Layout.fillWidth: true
                                    }

                                    Text {
                                        text: modelData.filePath || ""
                                        font.pixelSize: 10
                                        color: "#64748b"
                                        elide: Text.ElideMiddle
                                        Layout.fillWidth: true
                                    }

                                    // Action Buttons: Sisipkan ke Bus L1 / L2 / Hapus
                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 4

                                        Button {
                                            text: "Sisipkan (Bus L1)"
                                            Layout.fillWidth: true
                                            onClicked: {
                                                projectModel.addMediaLayer(modelData.name, modelData.filePath, modelData.mediaType, 1);
                                                root.close();
                                            }
                                            contentItem: Text {
                                                text: parent.text
                                                color: "#00e5ff"
                                                font.pixelSize: 10
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                            background: Rectangle {
                                                color: "#1e293b"
                                                border.color: "#00e5ff"
                                                radius: 3
                                            }
                                        }

                                        Button {
                                            text: "Bus L2"
                                            Layout.preferredWidth: 50
                                            onClicked: {
                                                projectModel.addMediaLayer(modelData.name, modelData.filePath, modelData.mediaType, 2);
                                                root.close();
                                            }
                                            contentItem: Text {
                                                text: parent.text
                                                color: "#cbd5e1"
                                                font.pixelSize: 10
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                            background: Rectangle {
                                                color: "#161b26"
                                                border.color: "#2d3748"
                                                radius: 3
                                            }
                                        }

                                        Button {
                                            text: "Hapus"
                                            Layout.preferredWidth: 46
                                            onClicked: assetManager.removeAsset(index)
                                            contentItem: Text {
                                                text: parent.text
                                                color: "#ef4444"
                                                font.pixelSize: 10
                                                horizontalAlignment: Text.AlignHCenter
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                            background: Rectangle {
                                                color: "#1a1215"
                                                border.color: "#4a1d24"
                                                radius: 3
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // Bottom Info Bar
        Rectangle {
            Layout.fillWidth: true
            height: 32
            color: "#10141d"
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12

                Text {
                    text: "Format didukung: After Effects Bodymovin (Lottie JSON), Apple ProRes 4444 MOV, WebM VP9 Alpha, MP4 H.264/H.265, SVG, PNG 32-bit."
                    font.pixelSize: 10
                    color: "#64748b"
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: "Total: " + assetManager.totalAssets + " Aset Siap Pakai"
                    font.pixelSize: 10
                    font.bold: true
                    color: "#00e5ff"
                }
            }
        }
    }
}
