cmake_minimum_required(VERSION 3.5)

project(RayTracingGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        custom.h
        double.h
        utils.h
        ray.h
        camera.h
        camera.cpp
        renderer.h
        renderer.cpp
        hittable.h
        hittable_list.h
        hittable_list.cpp
        scene.h
        scene.cpp
        sphere.h
        sphere.cpp
        arect.h
        arect.cpp
        cube.h
        cube.cpp
        triangle.h
        triangle.cpp
        mesh.h
        mesh.cpp
        material.h
        lambertian.h
        metal.h
        glass.h
        aabb.h
        bvh_node.h
        bvh_node.cpp
        texture.h
        color_texture.h
        image_texture.h
        light.h
        transform.h
        transform.cpp
        OBJ_Loader.h
        gui_handler.h
        gui_handler.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RayTracingGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RayTracingGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RayTracingGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RayTracingGUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RayTracingGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(RayTracingGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS RayTracingGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RayTracingGUI)
endif()
