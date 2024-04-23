cmake_minimum_required(VERSION 3.5)

project(SmartCalc_v2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(Qt6_DIR /home/allie/Qt/6.6.1/gcc_64/lib/cmake/Qt6)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)
set(PROJECT_DIRS
        model/
        controller/
        view/
        .
)

set(PROJECT_SOURCES "")
foreach(dir ${PROJECT_DIRS})
    file(GLOB CPP_SOURCES "${dir}/*.cpp" "${dir}**/*.cpp" "${dir}**/**/*.cpp")
    file(GLOB HEADER_SOURCES "${dir}/*.h" "${dir}/*.tpp" "${dir}**/*.h" "${dir}**/*.tpp" "${dir}**/**/*.h" "${dir}**/**/*.tpp")
    file(GLOB UI_SOURCES "${dir}/*.ui")
    list(APPEND PROJECT_SOURCES ${CPP_SOURCES} ${HEADER_SOURCES} ${UI_SOURCES})
endforeach()


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SmartCalc_v2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SmartCalc_v2 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SmartCalc_v2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SmartCalc_v2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SmartCalc_v2)
endif()

target_link_libraries(SmartCalc_v2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(SmartCalc_v2 PUBLIC Qt${QT_VERSION_MAJOR}::PrintSupport)
target_link_libraries(SmartCalc_v2 PRIVATE Qt${QT_VERSION_MAJOR}::Gui)

set_target_properties(SmartCalc_v2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SmartCalc_v2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SmartCalc_v2)
endif()
