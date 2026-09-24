import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "MAKASNA Broadcast Output & Hardware Routing"
    modal: true
    anchors.centerIn: parent
    width: 680
    height: 520
    standardButtons: Dialog.Close

    background: Rectangle {
        color: "#0f131c"
        border.color: "#1e2536"
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 16

        // Tab Navigation
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            TabButton { text: "🔌 Blackmagic DeckLink (SDI)" }
            TabButton { text: "📺 Secondary Display (HDMI/DP)" }
            TabButton { text: "🌐 RossTalk & Web Server" }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // TAB 1: DECKLINK
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Text {
                        text: "Blackmagic Design SDI Hardware Playout"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#00e5ff"
                    }

                    Text {
                        text: "Memisahkan video menjadi SDI 1 (Color Fill) dan SDI 2 (Alpha Key) untuk switcher ATEM / Ross Carbonite."
                        font.pixelSize: 11
                        color: "#94a3b8"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    // Device selection
                    RowLayout {
                        Text { text: "Device:"; color: "#cbd5e1"; font.pixelSize: 12 }
                        ComboBox {
                            id: decklinkDeviceCombo
                            Layout.fillWidth: true
                            model: [
                                "DeckLink Duo 2 (Channel 1 - SDI Fill & Key)",
                                "DeckLink Duo 2 (Channel 2 - SDI)",
                                "DeckLink Quad 2 (SDI 1-8)",
                                "DeckLink 8K Pro (SDI 1-4 12G-SDI)"
                            ]
                        }
                    }

                    // Video Standard
                    RowLayout {
                        Text { text: "Standard:"; color: "#cbd5e1"; font.pixelSize: 12 }
                        ComboBox {
                            id: videoStandardCombo
                            Layout.fillWidth: true
                            model: ["1080p60", "1080p59.94", "1080p50", "1080i59.94", "1080i50", "720p60", "2160p60 (4K)"]
                        }
                    }

                    // Keyer Mode
                    RowLayout {
                        Text { text: "Keyer Mode:"; color: "#cbd5e1"; font.pixelSize: 12 }
                        ComboBox {
                            id: keyerModeCombo
                            Layout.fillWidth: true
                            model: [
                                "External Key & Fill (SDI 1 = Fill, SDI 2 = Key)",
                                "Internal Keyer (SDI Pass-Through Loop)",
                                "Single Clean Output"
                            ]
                        }
                    }

                    // Playout Action Buttons
                    RowLayout {
                        spacing: 12
                        Button {
                            text: decklinkController.isStreaming ? "⏹ STOP DECKLINK" : "▶ START DECKLINK PLAYOUT"
                            font.bold: true
                            highlighted: !decklinkController.isStreaming
                            onClicked: {
                                if (decklinkController.isStreaming) {
                                    decklinkController.stopPlayout()
                                } else {
                                    decklinkController.startPlayout(decklinkDeviceCombo.currentIndex, videoStandardCombo.currentText, "external")
                                }
                            }
                        }

                        Text {
                            text: decklinkController.isStreaming
                                ? "● STREAMING: " + decklinkController.outputFrames + " frames"
                                : "STATUS: Idle"
                            font.family: "Monospace"
                            font.bold: true
                            color: decklinkController.isStreaming ? "#00e676" : "#64748b"
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // TAB 2: SECONDARY DISPLAY
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Text {
                        text: "Layar Sekunder Fullscreen (HDMI / DisplayPort)"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#00e5ff"
                    }

                    Text {
                        text: "Mengirimkan tampilan grafis transparan murni secara fullscreen ke monitor atau kartu capture HDMI."
                        font.pixelSize: 11
                        color: "#94a3b8"
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Button {
                        text: secondaryDisplayManager.isOpen ? "⏹ Tutup Output Display" : "▶ Buka Fullscreen Secondary Output"
                        highlighted: !secondaryDisplayManager.isOpen
                        onClicked: {
                            if (secondaryDisplayManager.isOpen) {
                                secondaryDisplayManager.closeFullscreenOutput()
                            } else {
                                secondaryDisplayManager.openFullscreenOutput(1, "none")
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // TAB 3: NETWORK & AUTOMATION
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

                    Text {
                        text: "RossTalk TCP & Web Browser Output"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#00e5ff"
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 60
                        color: "#111622"
                        radius: 4
                        border.color: "#1e2536"
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            Text { text: "RossTalk Port: 7788 (TCP Raw Listener)"; font.bold: true; color: "#00e676" }
                            Text { text: "Menerima perintah TAKE, CUE, CLEAR, GOAL dari switcher & Bitfocus Companion"; font.pixelSize: 11; color: "#94a3b8" }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 60
                        color: "#111622"
                        radius: 4
                        border.color: "#1e2536"
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            Text { text: "OBS / vMix Web Server: http://localhost:4989/"; font.bold: true; color: "#00e5ff" }
                            Text { text: "Canvas transparan murni untuk ingest Browser Source 1080p60"; font.pixelSize: 11; color: "#94a3b8" }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
