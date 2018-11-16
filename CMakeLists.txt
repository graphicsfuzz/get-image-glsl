cmake_minimum_required(VERSION 2.8)

project(cpp)

# Add "deps" to search prefix.
# We can manually copy libEGL{.dll,.so} and libGLESv2{.dll,.so} to deps
# so we can link get_image_egl; the linker needs to see *a* version of these
# libraries, even though different versions will be used a run-time.
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/deps" "${CMAKE_PREFIX_PATH}")

# The EGL and GLESv2 libraries have the "lib" prefix even on Windows.
set(CMAKE_FIND_LIBRARY_PREFIXES "lib" "${CMAKE_FIND_LIBRARY_PREFIXES}")

# Use C++11, this is compiler-agnostic (not GCC specific).
set (CMAKE_CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
endif()

# Set rpath for Mac and Linux.
# This ensures the libEGL and libGLESv2 libraries can be found at run-time
# in the same directory as the executable.
# On Windows, this is already the default behaviour.
if(APPLE)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:@executable_path/../lib:@executable_path/" )
else(UNIX)
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:$ORIGIN/../lib:$ORIGIN/" )
endif()

# Make sure CMake can find built GLFW dependency.
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/deps/install ${CMAKE_PREFIX_PATH})

option(BUILD_GLFW_VERSION "Build get_image_glfw" ON)
option(BUILD_EGL_VERSION "Build get_image_egl" ON)


if(BUILD_GLFW_VERSION)
    # -- Build dependencies -- #
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/deps)
    execute_process(
            COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} ${CMAKE_SOURCE_DIR}/deps -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps
    )
    execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps
    )
    execute_process(
            COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=../install -DBUILD_TYPE=${CMAKE_BUILD_TYPE} -P cmake_install.cmake
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps
    )
    # -- End build dependencies -- #

    find_package(glfw3 REQUIRED)


    add_executable(get_image_glfw
            include/glad/glad.h
            common.h
            context_glfw.cpp
            context_glfw.h
            glad.c
            json.hpp
            lodepng.cpp
            lodepng.h
            main.cpp
            openglcontext.h
            )

    target_link_libraries(get_image_glfw PUBLIC glfw)
    target_compile_definitions(get_image_glfw PUBLIC -DGETIMAGE_CONTEXT=CONTEXT_GLFW)
    target_include_directories(get_image_glfw PUBLIC include)

    install(TARGETS get_image_glfw
            DESTINATION bin
    )

endif()


if(BUILD_EGL_VERSION)
    # Find libraries
    find_library(LIB_EGL    NAMES EGL)
    find_library(LIB_GLES   NAMES GLESv2)

    if(NOT LIB_EGL)
        message(FATAL_ERROR "EGL library not found")
    endif()

    if(NOT LIB_GLES)
        message(FATAL_ERROR "GLES library not found")
    endif()

    add_executable(get_image_egl
            include/EGL/egl.h
            include/GLES/gl.h
            include/GLES3/gl3.h
            common.h
            context_egl.cpp
            context_egl.h
            json.hpp
            lodepng.cpp
            lodepng.h
            main.cpp
            openglcontext.h
            )

    target_compile_definitions(get_image_egl PUBLIC -DGETIMAGE_CONTEXT=CONTEXT_EGL)
    target_include_directories(get_image_egl PUBLIC include)
    target_link_libraries(get_image_egl PUBLIC ${LIB_EGL} ${LIB_GLES})

    install(TARGETS get_image_egl
            DESTINATION bin
    )

endif()
