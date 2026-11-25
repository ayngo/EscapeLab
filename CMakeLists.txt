
cmake_minimum_required(VERSION 3.16)

project(EscapeLab
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Escape the Lab - a Qt 6.5 story puzzle game"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

set(HEADERS
    include/core/GameState.h
    include/core/GameManager.h
    include/core/MapData.h

    include/ui/MainWindow.h

    include/scenes/StartScene.h
    include/scenes/MapScene.h

    include/entities/Player.h
    include/entities/GameObject.h
    include/entities/Portal.h
    include/entities/Npc.h
    include/entities/PickupItem.h


    # Dialogue Headers
    include/Dialogue/DialogueData.h
    include/Dialogue/DialogueManager.h
    include/Dialogue/DialogueWidget.h
)

set(SOURCES
    src/main.cpp

    src/core/GameState.cpp
    src/core/GameManager.cpp
    src/core/MapData.cpp

    src/ui/MainWindow.cpp

    src/scenes/StartScene.cpp
    src/scenes/MapScene.cpp

    src/entities/Player.cpp
    src/entities/GameObject.cpp
    src/entities/Portal.cpp
    src/entities/Npc.cpp
    src/entities/PickupItem.cpp


    # Dialogue
    src/dialogue/DialogueData.cpp
    src/dialogue/DialogueManager.cpp
    src/dialogue/DialogueWidget.cpp
)

set(RESOURCES resources.qrc)

add_executable(EscapeLab
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
)

target_include_directories(EscapeLab PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(EscapeLab PRIVATE
    Qt6::Widgets
)

set_target_properties(EscapeLab PROPERTIES
    WIN32_EXECUTABLE OFF
    MACOSX_BUNDLE OFF
)
