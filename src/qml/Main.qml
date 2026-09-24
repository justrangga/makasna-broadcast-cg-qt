import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Makasna.BroadcastCG

ApplicationWindow {
    id: window
    width: 1600
    height: 960
    minimumWidth: 1280
    minimumHeight: 720
    visible: true
    title: "MAKASNA Broadcast CG Studio (Native C++20 / Qt6)"
    color: "#090a0f"

    property string workspaceMode: "designer"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // 1. Top Navigation & Brand Header
        HeaderBar {
            Layout.fillWidth: true
            workspaceMode: window.workspaceMode
            onModeChanged: (mode) => window.workspaceMode = mode
            onOpenHardwareDialog: hardwareDialog.open()
            onOpenDataHubDialog: dataHubDialog.open()
        }

        // 2. Main Workspace (Designer Studio vs Playout Console)
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: window.workspaceMode === "designer" ? 0 : 1

            // VIEW 0: DESIGNER STUDIO
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Center Canvas & Panels
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        spacing: 0

                        // Left Hierarchy
                        LayersPanel {
                            Layout.fillHeight: true
                        }

                        // Center 1080p Canvas
                        CanvasStage {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        // Right Inspector
                        InspectorPanel {
                            Layout.fillHeight: true
                        }
                    }

                    // Bottom Interactive Timeline
                    TimelineView {
                        Layout.fillWidth: true
                    }
                }
            }

            // VIEW 1: PLAYOUT CONSOLE
            Item {
                PlayoutConsole {
                    anchors.fill: parent
                }
            }
        }
    }

    // Hardware Output Configuration Dialog
    HardwareOutputDialog {
        id: hardwareDialog
    }

    // Smart Data Hub Dialog
    SmartDataDialog {
        id: dataHubDialog
    }
}
