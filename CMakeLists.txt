cmake_minimum_required(VERSION 3.10)
project(elysium_launcher)

set(CMAKE_CXX_STANDARD 23)

# Найти GTK и WebKit2
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0 webkit2gtk-4.1)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})

add_executable(elysium_launcher main.cpp ${WEBVIEW_SOURCES})

target_include_directories(elysium_launcher PRIVATE libs/webview/core/include)
target_include_directories(elysium_launcher PRIVATE libs/jsoncpp/include)

target_link_libraries(elysium_launcher ${GTK_LIBRARIES})

