cmake_minimum_required(VERSION 3.10)
project(LunarLander VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)

if(CMAKE_HOST_WIN32 )
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Static linking
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif(CMAKE_HOST_WIN32 )

# Manually set SDL2 include and library paths
if(CMAKE_HOST_WIN32 )
    set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\include")
    set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\lib\\x64\\SDL2.lib")
    set(SDL2_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2-2.32.0\\lib\\x64\\SDL2.dll")

    set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\include")
    set(SDL2_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\lib\\x64\\SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2_image-2.8.8\\lib\\x64\\SDL2_image.dll")

    set(SDL2_TTF_INCLUDE_DIR "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\include")
    set(SDL2_TTF_LIBRARY "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\lib\\x64\\SDL2_ttf.lib")
    set(SDL2_TTF_LIBRARY_DLL "${CMAKE_SOURCE_DIR}\\..\\SDL2_ttf-2.24.0\\lib\\x64\\SDL2_ttf.dll")
else()
    set(SDL2_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2.dylib")

    set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")

    set(SDL2_TTF_INCLUDE_DIR "/opt/homebrew/include/SDL2")
    set(SDL2_TTF_LIBRARY "/opt/homebrew/lib/libSDL2_ttf.dylib")
endif(CMAKE_HOST_WIN32 )

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# the intention is that the engine lib encapsulates the rendering functionality (SDL2)
add_library(engine_lib STATIC 
    src/engine/BaseEngine.cpp
    src/engine/Texture.cpp
)
set_project_warnings(engine_lib)
target_link_libraries(engine_lib
    PRIVATE
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_TTF_LIBRARY}
)

# lunar lander game
add_library(lunar_lander_lib STATIC 
    src/lunar_lander/LunarLanderEngine.cpp
    src/lunar_lander/TerrainGenerator.cpp
)
set_project_warnings(lunar_lander_lib)
target_link_libraries(lunar_lander_lib
    PUBLIC engine_lib
)
add_executable(lunar_lander_game 
    apps/lunar_lander_main.cpp
)
set_project_warnings(lunar_lander_game)
target_link_libraries(lunar_lander_game
    PRIVATE lunar_lander_lib
)

# set assets directory relative to the source
target_compile_definitions(engine_lib PRIVATE 
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets"
)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1  # or a more recent version if available
)
FetchContent_MakeAvailable(googletest)
enable_testing()
add_subdirectory(tests)

set(CMAKE_BUILD_TYPE Debug)

if (WIN32)
    add_custom_command(
        TARGET lunar_lander_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:lunar_lander_game>"
        VERBATIM
    )
    add_custom_command(
        TARGET lunar_lander_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:lunar_lander_game>"
        VERBATIM
    )
    add_custom_command(
        TARGET lunar_lander_game POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:lunar_lander_game>"
        VERBATIM
    )
endif()
