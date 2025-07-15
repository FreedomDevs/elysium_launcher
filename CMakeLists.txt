cmake_minimum_required(VERSION 3.10)
project(elysium_launcher)

set(CMAKE_CXX_STANDARD 23)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0 webkit2gtk-4.1)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_definitions(${GTK_CFLAGS_OTHER})

file(GLOB_RECURSE WEBUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/webui/*.c")

add_executable(elysium_launcher src/main.cpp ${WEBUI_SOURCES} ${WEBVIEW_SOURCES})

option(USE_BUILTIN_WEBSERVER "Use built-in web server" ON)
if(USE_BUILTIN_WEBSERVER)
    add_compile_definitions(USE_BUILTIN_WEBSERVER=1)
else()
    add_compile_definitions(USE_BUILTIN_WEBSERVER=0)
endif()

target_include_directories(elysium_launcher PRIVATE libs/webview/core/include)
target_include_directories(elysium_launcher PRIVATE libs/jsoncpp/include)
target_include_directories(elysium_launcher PRIVATE libs/httplib)

target_link_libraries(elysium_launcher ${GTK_LIBRARIES})
