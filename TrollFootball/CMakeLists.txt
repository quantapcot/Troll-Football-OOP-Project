cmake_minimum_required(VERSION 3.28)

project(TrollFootball)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Đường dẫn tới SFMLConfig.cmake
set(CMAKE_PREFIX_PATH $ENV{SFML_DIR})

find_package(SFML 3 REQUIRED COMPONENTS
    Graphics
    Window
    System
    Audio
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "src/*.cpp"
)

add_executable(TrollFootball ${SOURCES})

target_include_directories(TrollFootball PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(TrollFootball PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# ============================
# Tự động copy DLL của SFML
# ============================

set(SFML_BIN_DIR "$ENV{SFML_DIR}/bin")

file(GLOB SFML_DLLS
    "${SFML_BIN_DIR}/*.dll"
)

foreach(DLL ${SFML_DLLS})
    add_custom_command(TARGET TrollFootball POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}"
            $<TARGET_FILE_DIR:TrollFootball>
    )
endforeach()