cmake_minimum_required(VERSION 3.16)
project(Game-Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_ROOT "${CMAKE_SOURCE_DIR}/external/sfml")
set(TMXLITE_ROOT "${CMAKE_SOURCE_DIR}/external/tmxlite")
set(PROJECT_ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")

set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

find_path(TMXLITE_INCLUDE_DIR
    NAMES tmxlite/Map.hpp
    PATHS "${TMXLITE_ROOT}/include"
    NO_DEFAULT_PATH
)

find_library(TMXLITE_LIBRARY
    NAMES tmxlite tmxlite-d
    PATHS "${TMXLITE_ROOT}/lib"
    NO_DEFAULT_PATH
)

if(NOT TMXLITE_INCLUDE_DIR)
    message(FATAL_ERROR "tmxlite headers not found in ${TMXLITE_ROOT}/include")
endif()

if(NOT TMXLITE_LIBRARY)
    message(FATAL_ERROR "tmxlite library not found in ${TMXLITE_ROOT}/lib")
endif()

file(GLOB_RECURSE GAME_ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)
list(FILTER GAME_ENGINE_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_library(game_engine STATIC ${GAME_ENGINE_SOURCES})

target_include_directories(game_engine PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${SFML_ROOT}/include"
    "${TMXLITE_INCLUDE_DIR}"
)

target_link_libraries(game_engine PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    "${TMXLITE_LIBRARY}"
)

add_executable(Game-Engine src/main.cpp)
target_link_libraries(Game-Engine PRIVATE game_engine)

# Copy assets into the build tree, updating files when their contents change.
file(GLOB_RECURSE PROJECT_ASSETS CONFIGURE_DEPENDS
    "${PROJECT_ASSETS_DIR}/*"
)
foreach(ASSET_FILE IN LISTS PROJECT_ASSETS)
    file(RELATIVE_PATH ASSET_RELATIVE_PATH "${PROJECT_ASSETS_DIR}" "${ASSET_FILE}")
    set(ASSET_DESTINATION "${CMAKE_BINARY_DIR}/assets/${ASSET_RELATIVE_PATH}")
    get_filename_component(ASSET_DESTINATION_DIR "${ASSET_DESTINATION}" DIRECTORY)
    file(MAKE_DIRECTORY "${ASSET_DESTINATION_DIR}")
    configure_file("${ASSET_FILE}" "${ASSET_DESTINATION}" COPYONLY)
endforeach()

# Use the build-tree asset path so the app resolves the map correctly in CLion and IDE launchers.
target_compile_definitions(game_engine PUBLIC
    GAME_ASSET_DIR="${CMAKE_BINARY_DIR}/assets"
)

if(APPLE)
    set_target_properties(Game-Engine PROPERTIES
        BUILD_RPATH "${SFML_ROOT}/lib;${TMXLITE_ROOT}/lib"
        INSTALL_RPATH "${SFML_ROOT}/lib;${TMXLITE_ROOT}/lib"
    )
endif()
