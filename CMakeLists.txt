cmake_minimum_required(VERSION 3.22)
project(AbyssHollow VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ABYSS_FETCH_SFML "Download SFML when it is not installed locally" ON)

find_package(SFML 2.6 COMPONENTS graphics window system audio QUIET)
if(NOT SFML_FOUND)
    if(ABYSS_FETCH_SFML)
        include(FetchContent)
        FetchContent_Declare(
            SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.1
        )
        FetchContent_MakeAvailable(SFML)
    else()
        find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)
    endif()
endif()

add_executable(AbyssHollow
    src/main.cpp
    src/Core/Game.cpp
    src/Core/Settings.cpp
    src/Core/StateStack.cpp
    src/States/GameplayState.cpp
    src/States/MainMenuState.cpp
    src/States/PauseMenuState.cpp
    src/States/SettingsState.cpp
    src/Systems/AudioSystem.cpp
    src/Systems/ResourceManager.cpp
    src/Systems/SaveSystem.cpp
    src/UI/Button.cpp
    src/UI/MenuList.cpp
    src/UI/Slider.cpp
    src/World/WorldSystem.cpp
    src/World/LevelData.cpp
    src/Player/PlayerSystem.cpp
)

target_include_directories(AbyssHollow PRIVATE src)
target_link_libraries(AbyssHollow PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

if(WIN32)
    add_custom_command(TARGET AbyssHollow POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy -t "$<TARGET_FILE_DIR:AbyssHollow>" "$<TARGET_RUNTIME_DLLS:AbyssHollow>"
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime DLLs beside AbyssHollow"
        VERBATIM
    )
endif()

if(MSVC)
    target_compile_options(AbyssHollow PRIVATE /W4 /permissive-)
else()
    target_compile_options(AbyssHollow PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET AbyssHollow POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:AbyssHollow>/assets"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/configs" "$<TARGET_FILE_DIR:AbyssHollow>/configs"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/levels" "$<TARGET_FILE_DIR:AbyssHollow>/levels"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_FILE_DIR:AbyssHollow>/saves"
    COMMENT "Copying runtime assets, configs, levels, and save folder beside AbyssHollow"
    VERBATIM
)
