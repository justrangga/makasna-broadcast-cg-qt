cmake_minimum_required(VERSION 3.24)
project(MakasnaBroadcastCG VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml Network Svg QuickControls2)
qt_standard_project_setup()

# Application Executable
qt_add_executable(makasna_broadcast_cg
    src/main.cpp
    src/core/AnimationEngine.h
    src/core/AnimationEngine.cpp
    src/core/ProjectModel.h
    src/core/ProjectModel.cpp
    src/core/SmartDataManager.h
    src/core/SmartDataManager.cpp
    src/core/PlayoutController.h
    src/core/PlayoutController.cpp
    src/hardware/DeckLinkController.h
    src/hardware/DeckLinkController.cpp
    src/hardware/SecondaryDisplayManager.h
    src/hardware/SecondaryDisplayManager.cpp
    src/network/RossTalkServer.h
    src/network/RossTalkServer.cpp
    src/network/BroadcastHttpServer.h
    src/network/BroadcastHttpServer.cpp
    resources/app.qrc
)

# QML Modules
qt_add_qml_module(makasna_broadcast_cg
    URI Makasna.BroadcastCG
    VERSION 1.0
    QML_FILES
        src/qml/Main.qml
        src/qml/components/HeaderBar.qml
        src/qml/components/SafeZoneOverlay.qml
        src/qml/components/TimelineView.qml
        src/qml/components/CanvasStage.qml
        src/qml/components/InspectorPanel.qml
        src/qml/components/LayersPanel.qml
        src/qml/components/PlayoutConsole.qml
        src/qml/components/HardwareOutputDialog.qml
        src/qml/components/SmartDataDialog.qml
        src/qml/templates/LowerThirdNews.qml
        src/qml/templates/SmartLeaderboard.qml
        src/qml/templates/ScorebugMatch.qml
        src/qml/templates/RunningTicker.qml
        src/qml/templates/ChannelBugWatermark.qml
)

target_link_libraries(makasna_broadcast_cg PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Network
    Qt6::Svg
    Qt6::QuickControls2
)

if (WIN32)
    set_target_properties(makasna_broadcast_cg PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    target_link_libraries(makasna_broadcast_cg PRIVATE ole32 oleaut32)
endif()
