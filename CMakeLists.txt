cmake_minimum_required(VERSION 3.25)

# Auto detect Emscripten toolchain when building for Web
if(WEB OR PLATFORM STREQUAL "Web")
    if(NOT CMAKE_TOOLCHAIN_FILE)
        file(GLOB EMSCRIPTEN_CMAKE_LIST
            "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
            "$ENV{HOME}/.xmake/packages/e/emscripten/*/*/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
            "$ENV{HOME}/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
            "/usr/share/emscripten/cmake/Modules/Platform/Emscripten.cmake"
            "/usr/lib/emscripten/cmake/Modules/Platform/Emscripten.cmake"
        )
        list(LENGTH EMSCRIPTEN_CMAKE_LIST NUM_FOUND)
        if(NUM_FOUND GREATER 0)
            list(GET EMSCRIPTEN_CMAKE_LIST 0 FOUND_TOOLCHAIN)
            set(CMAKE_TOOLCHAIN_FILE "${FOUND_TOOLCHAIN}" CACHE FILEPATH "Emscripten toolchain")
        endif()
    endif()
endif()

project(pure_game LANGUAGES C)

# Export compilation database for IDE integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable C23 standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable GNU extensions for Emscripten compatibility
set(CMAKE_C_EXTENSIONS ON)

# Configure raylib web platform when compiling with Emscripten
if(EMSCRIPTEN)
    set(PLATFORM "Web" CACHE STRING "" FORCE)
endif()

# Download raylib release archive via FetchContent
include(FetchContent)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.2.tar.gz
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(game src/main.c)
target_link_libraries(game PRIVATE raylib)

# Compiler warning flags
if(MSVC)
    target_compile_options(game PRIVATE /W4)
else()
    target_compile_options(game PRIVATE -Wall -Wextra)
endif()

# WebAssembly build settings for Emscripten
if(EMSCRIPTEN)
    set_target_properties(game PROPERTIES
        OUTPUT_NAME "index"
        SUFFIX ".html"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/dist"
    )
    target_link_options(game PRIVATE
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/src/main.html"
    )
else()
    # Target to run the compiled game binary
    add_custom_target(run
        COMMAND $<TARGET_FILE:game>
        DEPENDS game
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running game..."
    )

    # Target to build web version from desktop build directory
    add_custom_target(web
        COMMAND ${CMAKE_COMMAND} -B "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-web" -DWEB=ON
        COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-web" -j
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Building WebAssembly target..."
    )

    # Find HTTP server tool for local preview
    find_program(PYTHON_BIN NAMES python3 python)
    find_program(NPX_BIN NAMES npx npx.cmd)
    find_program(BUSYBOX_BIN NAMES busybox)
    find_program(PHP_BIN NAMES php)

    if(PYTHON_BIN)
        set(WEB_SERVER_COMMAND ${PYTHON_BIN} -m http.server 8000 --directory "${CMAKE_CURRENT_SOURCE_DIR}/dist")
    elseif(NPX_BIN)
        set(WEB_SERVER_COMMAND ${NPX_BIN} http-server "${CMAKE_CURRENT_SOURCE_DIR}/dist" -p 8000)
    elseif(BUSYBOX_BIN)
        set(WEB_SERVER_COMMAND ${BUSYBOX_BIN} httpd -f -p 8000 -h "${CMAKE_CURRENT_SOURCE_DIR}/dist")
    elseif(PHP_BIN)
        set(WEB_SERVER_COMMAND ${PHP_BIN} -S localhost:8000 -t "${CMAKE_CURRENT_SOURCE_DIR}/dist")
    else()
        set(WEB_SERVER_COMMAND ${CMAKE_COMMAND} -E echo "Error: No HTTP server found (install Python or Node.js)")
    endif()

    # Target to build and run web version locally
    add_custom_target(web-run
        COMMAND ${CMAKE_COMMAND} -B "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-web" -DWEB=ON
        COMMAND ${CMAKE_COMMAND} --build "${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-web" -j
        COMMAND ${CMAKE_COMMAND} -E echo "Serving web game at http://localhost:8000"
        COMMAND ${WEB_SERVER_COMMAND}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running WebAssembly local server..."
    )
endif()
