cmake_minimum_required(VERSION 3.10)
project(elysium_launcher)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Основной исполняемый файл
add_executable(elysium_launcher
    ${CPP_SOURCES}
)

if (WIN32)
    message(STATUS "Building for Windows")
else()
    message(STATUS "Building for Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK REQUIRED gtk+-3.0 webkit2gtk-4.1)
    target_include_directories(elysium_launcher PRIVATE ${GTK_INCLUDE_DIRS})
    target_link_libraries(elysium_launcher PRIVATE ${GTK_LIBRARIES})
endif()

# Включаем сторонние библиотеки
target_include_directories(elysium_launcher PRIVATE
    libs/webview/core/include
    libs/jsoncpp/include
    libs/httplib
    include/
)

# Опция использования встроенного веб-сервера
option(USE_BUILTIN_WEBSERVER "Use built-in web server" ON)
target_compile_definitions(elysium_launcher PRIVATE USE_BUILTIN_WEBSERVER=$<BOOL:${USE_BUILTIN_WEBSERVER}>)

# Пути к Rust-коду
set(RUST_CRATE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/el-core-lib)
set(RUST_LIB ${RUST_CRATE_DIR}/target/release/libel_core_lib.a)
set(CBINDGEN_BIN "$ENV{HOME}/.cargo/bin/cbindgen")
set(BINDINGS_H ${CMAKE_CURRENT_SOURCE_DIR}/include/el-core-bindings.h)
file(GLOB_RECURSE RUST_SOURCES ${RUST_CRATE_DIR}/src/*.rs)
set(CARGO_MANIFEST ${RUST_CRATE_DIR}/Cargo.toml)

# Шаг 1: Сборка Rust-библиотеки
add_custom_command(
    OUTPUT ${RUST_LIB}
    COMMAND cargo build --release
    WORKING_DIRECTORY ${RUST_CRATE_DIR}
    DEPENDS ${RUST_SOURCES} ${CARGO_MANIFEST}
    COMMENT "Building Rust library with cargo"
)

# Шаг 2: Генерация bindings.h с помощью cbindgen
add_custom_command(
    OUTPUT ${BINDINGS_H}
    COMMAND ${CBINDGEN_BIN} --output ${BINDINGS_H}
    WORKING_DIRECTORY ${RUST_CRATE_DIR}
    DEPENDS ${RUST_LIB}
    COMMENT "Generating el-core-bindings.h with cbindgen"
)

# Шаг 3: Сборка rust
add_custom_target(build_rust ALL
    DEPENDS ${RUST_LIB} ${BINDINGS_H}
)

add_library(el_core_lib STATIC IMPORTED GLOBAL)
set_target_properties(el_core_lib PROPERTIES
    IMPORTED_LOCATION ${RUST_LIB}
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_dependencies(el_core_lib build_rust)
add_dependencies(elysium_launcher build_rust)

target_link_libraries(elysium_launcher PRIVATE el_core_lib)
