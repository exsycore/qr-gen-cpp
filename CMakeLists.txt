cmake_minimum_required(VERSION 3.20)
project(QR-GEN)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/header)

add_subdirectory(libs/cpp-httplib)

set(ZLIB_FOUND TRUE)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/header)
set(ZLIB_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/external/zlib.lib)

add_subdirectory(libs/libpng)

set(PNG_SHARED ON)
set(PNG_STATIC OFF)
set(PNG_TESTS OFF)

file(GLOB_RECURSE SOURCES
    source/*.cpp
)

set(ICON_RC ${CMAKE_CURRENT_SOURCE_DIR}/qr-icon.rc)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${ICON_RC}
)

#add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:CONSOLE"
)
target_link_libraries(${PROJECT_NAME} PRIVATE png_shared ${ZLIB_LIBRARY})