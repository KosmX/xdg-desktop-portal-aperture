cmake_minimum_required(VERSION 3.14)

project(xdg-desktop-portal-aperture LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core DBus Concurrent)

include_directories("src")

file(GLOB_RECURSE SOURCES "src/*.*")

add_executable(xdg-desktop-portal-aperture ${SOURCES})
target_link_libraries(xdg-desktop-portal-aperture Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::DBus Qt${QT_VERSION_MAJOR}::Concurrent)

include(GNUInstallDirs)
install(TARGETS xdg-desktop-portal-aperture
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
