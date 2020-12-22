cmake_minimum_required(VERSION 3.17)
project(2Cars2Racing)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        AirHockey.cpp
        charFunctions.cpp
        config.cpp
        binaryIO.cpp

        gameObject.cpp
        puck.cpp
        paddle.cpp
        table.cpp
        shapes.cpp

        audio.cpp
        goal.cpp
        ran.cpp

        shapeDrawing.cpp
        camera.cpp

        physicsHelper.cpp

        menu.cpp
        UI.cpp
        button.cpp
        UI_text.cpp)

set(HEADER_FILES
        charFunctions.h
        config.h
        binaryIO.h
        vector.h

        gameObject.h
        puck.h
        paddle.h
        table.h
        shapes.h
        customMath.h

        audio.h
        goal.h

        ran.h

        shapeDrawing.h
        camera.h

        physicalObject.h
        physicsHelper.h

        menu.h
        UI.h
        button.h
        UI_text.h gameInfo.h)

SET(GRAPHICS_FILES
        2D_graphics/timer.h
        2D_graphics/timer.cpp
        2D_graphics/2D_graphics.h
        2D_graphics/game_pad.h)

SET(NETWORKING_FILES
        2D_graphics/UDP_com.cpp
        2D_graphics/UDP_com.h
        2D_graphics/UDP_com6.cpp
        2D_graphics/UDP_com6.h
        gameNetworking.cpp
        gameNetworking.h)

#Make executable
add_executable(AirHockey ${SOURCE_FILES} ${HEADER_FILES} ${GRAPHICS_FILES} ${NETWORKING_FILES})

set_property(TARGET AirHockey PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")


# Add 2D_graphics_ver1.lib game_pad.lib and ws2_32
target_link_libraries(AirHockey "${CMAKE_CURRENT_SOURCE_DIR}/2D_graphics/2D_graphics_ver1.lib" "${CMAKE_CURRENT_SOURCE_DIR}/2D_graphics/game_pad.lib" "ws2_32")


# Build host version too. 
add_executable(AirHockeyHost ${SOURCE_FILES} ${HEADER_FILES} ${GRAPHICS_FILES} ${NETWORKING_FILES})
set_property(TARGET AirHockeyHost PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
target_link_libraries(AirHockeyHost "${CMAKE_CURRENT_SOURCE_DIR}/2D_graphics/2D_graphics_ver1.lib" "${CMAKE_CURRENT_SOURCE_DIR}/2D_graphics/game_pad.lib" "ws2_32")
target_compile_definitions(AirHockeyHost PUBLIC __CMAKE_PROJECT__="HOST")