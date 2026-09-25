import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "MAKASNA Smart Data Hub - Excel, Google Sheets, JSON & XML"
    modal: true
    anchors.centerIn: parent
    width: 820
    height: 580
    standardButtons: Dialog.Close

    background: Rectangle {
        color: "#0b0e14"
        border.color: "#1e2536"
        border.width: 1
        radius: 8
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        // Top Status Header
        Rectangle {
            Layout.fillWidth: true
            height: 48
            color: "#111724"
            border.color: smartDataManager.isSyncing ? "#00e5ff" : "#1e2536"
            border.width: 1
            radius: 6

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 12

                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: smartDataManager.isSyncing ? "#00e5ff" : "#64748b"
                }

                Text {
                    text: smartDataManager.isSyncing ? "STATUS: LIVE AUTO-SYNC AKTIF" : "STATUS: IDLE (TIDAK ADA SYNC)"
                    color: smartDataManager.isSyncing ? "#00e5ff" : "#94a3b8"
                    font.bold: true
                    font.pixelSize: 11
                }

                Text {
                    text: "• Sumber: " + smartDataManager.activeSourceType
                    color: "#cbd5e1"
                    font.pixelSize: 11
                }

                Text {
                    text: "• Update Terakhir: " + smartDataManager.lastSyncTime
                    color: "#94a3b8"
                    font.pixelSize: 11
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "🔄 Sync Sekarang"
                    enabled: smartDataManager.isSyncing
                    onClicked: smartDataManager.manualRefresh()
                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#00e5ff" : "#475569"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: "#1e293b"
                        border.color: "#00e5ff"
                        radius: 4
                    }
                }

                Button {
                    text: "⏹ Berhenti"
                    enabled: smartDataManager.isSyncing
                    onClicked: smartDataManager.stopAutoSync()
                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#f43f5e" : "#475569"
                        font.bold: true
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: "#1e293b"
                        border.color: "#f43f5e"
                        radius: 4
                    }
                }
            }
        }

        // Configuration Tabs
        TabBar {
            id: navTabs
            Layout.fillWidth: true
            background: Rectangle { color: "#0f131c" }

            TabButton {
                text: "📑 Local Excel / CSV / JSON"
                width: implicitWidth + 24
            }
            TabButton {
                text: "📊 Google Spreadsheet (Live)"
                width: implicitWidth + 24
            }
            TabButton {
                text: "🌐 REST API JSON / XML Feed"
                width: implicitWidth + 24
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 140
            currentIndex: navTabs.currentIndex

            // 1. LOCAL FILE TAB
            Rectangle {
                color: "#111622"
                border.color: "#1e2536"
                radius: 6

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Hubungkan file lokal di komputer (Excel .xlsx / .csv / .json). Perubahan file di disk akan ter-update otomatis secara real-time ke CG!"
                        color: "#94a3b8"
                        font.pixelSize: 11
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        TextField {
                            id: localPathInput
                            Layout.fillWidth: true
                            placeholderText: "Contoh: C:/BroadcastData/Klasemen.csv atau D:/Data/news.json"
                            color: "#ffffff"
                            background: Rectangle {
                                color: "#0b0e14"
                                border.color: "#334155"
                                radius: 4
                            }
                        }

                        Button {
                            text: "Hubungkan File"
                            onClicked: {
                                if (localPathInput.text.length > 0) {
                                    var fmt = localPathInput.text.endsWith(".json") ? "json" : "csv";
                                    smartDataManager.connectLocalFile(localPathInput.text, fmt);
                                }
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 11
                            }
                            background: Rectangle {
                                color: "#0284c7"
                                radius: 4
                            }
                        }
                    }
                }
            }

            // 2. GOOGLE SHEETS TAB
            Rectangle {
                color: "#111622"
                border.color: "#1e2536"
                radius: 6

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Masukkan URL Google Spreadsheet (pastikan Sheet diatur 'Anyone with the link can view'). CG akan otomatis mem-polling perubahan data."
                        color: "#94a3b8"
                        font.pixelSize: 11
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        TextField {
                            id: gsheetUrlInput
                            Layout.fillWidth: true
                            placeholderText: "https://docs.google.com/spreadsheets/d/1BxiMVs0XRA5nFMdKvBdBZjgmUUqptlbs74OgvE2upms/edit"
                            color: "#ffffff"
                            background: Rectangle {
                                color: "#0b0e14"
                                border.color: "#334155"
                                radius: 4
                            }
                        }

                        Text {
                            text: "Interval (dtk):"
                            color: "#cbd5e1"
                            font.pixelSize: 11
                        }

                        SpinBox {
                            id: gsheetInterval
                            from: 1
                            to: 60
                            value: 3
                            editable: true
                        }

                        Button {
                            text: "Connect Google Sheet"
                            onClicked: {
                                if (gsheetUrlInput.text.length > 0) {
                                    smartDataManager.connectGoogleSheet(gsheetUrlInput.text, gsheetInterval.value);
                                }
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 11
                            }
                            background: Rectangle {
                                color: "#059669"
                                radius: 4
                            }
                        }
                    }
                }
            }

            // 3. REMOTE JSON / XML TAB
            Rectangle {
                color: "#111622"
                border.color: "#1e2536"
                radius: 6

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Text {
                        text: "Hubungkan ke REST API endpoint online (JSON) atau XML feed / RSS ticker berita langsung dari server."
                        color: "#94a3b8"
                        font.pixelSize: 11
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8

                        TextField {
                            id: apiUrlInput
                            Layout.fillWidth: true
                            placeholderText: "https://api.scoreboard.live/v1/match/4982 atau https://news.tv/feed.xml"
                            color: "#ffffff"
                            background: Rectangle {
                                color: "#0b0e14"
                                border.color: "#334155"
                                radius: 4
                            }
                        }

                        ComboBox {
                            id: apiFormatCombo
                            model: ["JSON API", "XML / RSS"]
                            width: 120
                        }

                        SpinBox {
                            id: apiInterval
                            from: 1
                            to: 60
                            value: 2
                            editable: true
                        }

                        Button {
                            text: "Connect API"
                            onClicked: {
                                if (apiUrlInput.text.length > 0) {
                                    if (apiFormatCombo.currentIndex === 0) {
                                        smartDataManager.connectRemoteJson(apiUrlInput.text, apiInterval.value);
                                    } else {
                                        smartDataManager.connectRemoteXml(apiUrlInput.text, apiInterval.value);
                                    }
                                }
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "#ffffff"
                                font.bold: true
                                font.pixelSize: 11
                            }
                            background: Rectangle {
                                color: "#7c3aed"
                                radius: 4
                            }
                        }
                    }
                }
            }
        }

        // Active Datasets Preview
        Text {
            text: "DATASETS TERSIMPAN & STRUKTUR TABEL (SIAP DI-BIND KE LAYER CG)"
            font.bold: true
            font.pixelSize: 12
            color: "#00e5ff"
            Layout.topMargin: 4
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: projectModel.datasets
            spacing: 6
            clip: true
            delegate: Rectangle {
                width: parent.width
                height: 76
                color: "#111622"
                border.color: "#1e2536"
                radius: 4

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 12

                    Rectangle {
                        width: 38
                        height: 38
                        radius: 4
                        color: "#1e2536"
                        Text {
                            anchors.centerIn: parent
                            text: "DB"
                            font.bold: true
                            color: "#00e5ff"
                            font.pixelSize: 12
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        RowLayout {
                            Text {
                                text: modelData.name
                                font.bold: true
                                font.pixelSize: 13
                                color: "#ffffff"
                            }
                            Text {
                                text: "(" + (modelData.rows ? modelData.rows.length : 0) + " baris data)"
                                font.pixelSize: 11
                                color: "#10b981"
                                font.bold: true
                            }
                        }

                        Text {
                            text: "Kolom Tersedia: " + (modelData.headers ? modelData.headers.join(" | ") : "-")
                            font.pixelSize: 11
                            color: "#94a3b8"
                            font.family: "Monospace"
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
