cmake_minimum_required(VERSION 3.10)

# Register general cmake commands
include(cmake/Custom.cmake)

# Set policies
cmake_policy(SET CMP0054 NEW) # Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0042 NEW) # MACOSX_RPATH is enabled by default.
cmake_policy(SET CMP0063 NEW) # Honor visibility properties for all target types.
cmake_policy(SET CMP0077 NEW) # option() honors normal variables.
cmake_policy(SET CMP0120 OLD) # The WriteCompilerDetectionHeader module is removed.

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)  # You can choose 11, 14, or 17 depending on your needs
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add message color headers

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldBlue    "${Esc}[1;90m")
  set(Reset "${Esc}[0m")  # ANSI code for bright and bold green

endif()



# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateExportHeader)

set(WriterCompilerDetectionHeaderFound NOTFOUND)

# This module is only available with CMake >=3.1, so check whether it could be found
# BUT in CMake 3.1 this module doesn't recognize AppleClang as compiler, so just use it as of CMake 3.2
if (${CMAKE_VERSION} VERSION_GREATER "3.2")
    include(WriteCompilerDetectionHeader OPTIONAL RESULT_VARIABLE WriterCompilerDetectionHeaderFound)
endif()

# Include custom cmake modules
include(cmake/Coverage.cmake)
include(cmake/GenerateTemplateExportHeader.cmake)
include(cmake/GetGitRevisionDescription.cmake)
include(cmake/HealthCheck.cmake)

# 
# Project description and (meta) information
# 

# Get git revision
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 12 GIT_REV)
if(NOT GIT_SHA1)
    set(GIT_REV "0")
endif()

# Check if the platform is Windows or Unix-based and set the BUILD_DATE accordingly
if(WIN32)
  # Windows command to get time with milliseconds
  execute_process(
      COMMAND powershell -Command "[DateTime]::Now.ToString('yyyy-MM-dd HH:mm:ss.fff')"
      OUTPUT_VARIABLE BUILD_DATE
  )
else()
  # Unix-based command to get time with milliseconds
  execute_process(
      COMMAND bash -c "echo -n $(date '+%Y-%m-%d %H:%M:%S.%3N')"
      OUTPUT_VARIABLE BUILD_DATE
  )
endif()

# Add the BUILD_DATE definition
add_definitions(-DBUILD_DATE="${BUILD_DATE}")

# Meta information about the project
set(META_PROJECT_NAME        "ndaimm")
set(META_PROJECT_DESCRIPTION "NDAIMM Main Control Systems")
set(META_AUTHOR_ORGANIZATION "University of Notre Dame")
set(META_AUTHOR_DOMAIN       "https://github.com/cginternals/")
set(META_AUTHOR_MAINTAINER   "ndaimm@nd.edu")
set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")
set(META_CMAKE_INIT_SHA      "${GIT_REV}")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)

set_target_properties(${PROJECT_NAME} PROPERTIES
                       RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Configure the version file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

#
# Project configuration options
# 

# Project options
# option(BUILD_SHARED_LIBS      "Build shared instead of static libraries."              ON)
# option(OPTION_SELF_CONTAINED  "Create a self-contained install with all dependencies." OFF)
# option(OPTION_BUILD_TESTS     "Build tests."                                           ON)
# option(OPTION_BUILD_DOCS      "Build documentation."                                   OFF)
# option(OPTION_BUILD_EXAMPLES  "Build examples."                                        OFF)
option(OPTION_ENABLE_COVERAGE "Add coverage information."                              ON)
option(SERVER_CONNECT "Connect Without GUI" OFF)


#
# Project Health Check Setup
#

# Add cmake-init template check cmake targets
add_check_template_target(${META_CMAKE_INIT_SHA})

# Configure health check tools
enable_cppcheck(ON)
enable_clang_tidy(ON)
enable_coverage(${OPTION_ENABLE_COVERAGE})

# Conditional package checks for Unix-like systems
if (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client>=0.2.7)
    pkg_check_modules(WAYLAND_CURSOR REQUIRED wayland-cursor>=0.2.7)
    pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl>=0.2.7)
    pkg_check_modules(XKBCOMMON REQUIRED xkbcommon>=0.5.0)
endif()

# Including Wayland support
if (WAYLAND_CLIENT_FOUND AND WAYLAND_CURSOR_FOUND AND WAYLAND_EGL_FOUND AND XKBCOMMON_FOUND)
    message(STATUS "Wayland support enabled")
endif()



# Find packages required by the project. 
# Add any new find_package() calls heres for additional external libraries.
# This is only for system calls which should be as limited.

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
if(Python3_FOUND)
    message(STATUS "Found Python3: ${Python3_EXECUTABLE} (found version \"${Python3_VERSION}\")")
    message(STATUS "Python3 Headers: ${Python3_INCLUDE_DIRS}")
    message(STATUS "Python3 Libraries: ${Python3_LIBRARIES}")
else()
    message(FATAL_ERROR "Python3 is not found.")
endif()

# Include Python headers
include_directories(${Python3_INCLUDE_DIRS})

# Link Python libraries
link_libraries(${Python3_LIBRARIES})

# Add pybind11
add_subdirectory(lib/pybind11)

set(imgui_SOURCE_DIR "lib/imgui/")

# Project-wide include directories. Add any new general include directories here.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/tests)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/gui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/threading)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/analysis)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/analysis/movement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/analysis/r_tree)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/networking)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/serialization)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/gui/networking_helper)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/gui/native)
include_directories(${CMAKE_CURRENT_BINARY_DIR}) # Include the binary directory for version.h
include_directories(${CMAKE_BINARY_DIR}/models)
include_directories(${FLATBUFFERS_INCLUDE_DIRS})
include_directories(${imgui_SOURCE_DIR})


# Adding external dependencies located in the 'lib' directory. 
# To add new libraries, create a subdirectory in 'lib' and use add_subdirectory() here.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/depthai-core EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib/quill)

# Project declaration must be after the Hunter Cmake build system.
project(AIMM LANGUAGES CXX)

find_package(OpenCV 4 REQUIRED)
find_package(OpenGL REQUIRED)


include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set( GLAD_GL "" )

add_subdirectory(lib/glfw)
option( GLFW-CMAKE-STARTER-USE-GLFW-GLAD "Use GLAD from GLFW" ON )

if( GLFW-CMAKE-STARTER-USE-GLFW-GLAD )
    include_directories("${GLFW_SOURCE_DIR}/deps")
    set( GLAD_GL "${GLFW_SOURCE_DIR}/deps/glad/gl.h" )
endif()

if(HEADLESS)
  add_definitions(-DHEADLESS)
endif()



set(FLATBUFFERS_BUILD_TESTS OFF CACHE BOOL "Build flatbuffers tests")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/flatbuffers
                 ${CMAKE_BINARY_DIR}/flatbuffers-build
                 EXCLUDE_FROM_ALL)


file(GLOB SOCKET_SOURCES "src/gui/socket/*.cpp")

# Define the executable target. 
# Add new source files to this list to compile them as part of the project.
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/serialization/serialib.cpp
    src/logger/logger.cpp
    src/pipeline/oak_lr_pipeline.cpp
    src/networking/udp.cpp
    src/peripherals.cpp
    src/pipeline/inference.cpp
    src/serialization/serialFactory.cpp
    src/serialization/serializerExample.cpp
    src/threading.cpp
    src/tests/fb_check_t.cpp
    src/tests/peripherals_t.cpp
    src/gui/gui.cpp
    src/gui/runner.cpp
    src/gui/aimm_drawer.cpp
    src/gui/aimm_status.cpp
    src/gui/aimm_settings.cpp
    src/gui/resources/fonts/roboto_medium.cpp
    src/networking/rns_sender.cpp
    src/logger/binding.cpp
    src/networking/rns_manager.cpp
    src/gui/aimm_status_helper.cpp
    src/gui/aimm_status_logger.cpp
    src/gui/aimm_console.cpp
    src/gui/aimm_test_helper.cpp
    src/initialization.cpp
    src/gui/aimm_headless.cpp
    src/analysis/analysis.cpp
    src/analysis/finder.cpp
    src/analysis/normalizer.cpp
    src/analysis/path_planner.cpp
    src/analysis/r_tree_querier.cpp
    src/analysis/r_tree_synthesizer.cpp
    src/tests/a_star_check_t.cpp
    ${GLAD_GL}
    ${SOCKET_SOURCES}
)

# Set include directories for the target. 
# Add specific include directories related to this target.
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/depthai-core/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Python3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

pybind11_add_module(quill_logger src/logger/binding.cpp)
target_link_libraries(quill_logger PRIVATE pybind11::module quill::quill ${Python3_LIBRARIES})

# Link libraries with the target. 
# Add new libraries to link against by appending them to this list.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        depthai::opencv  # DepthAI's specialized OpenCV library.
        ${OpenCV_LIBS}   # Linking with OpenCV libraries.
        quill::quill     # Logging library.
        flatbuffers
        glfw
        pybind11::embed 
        ${Python3_LIBRARIES}
)

target_sources(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imgui_internal.h
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}/
    ${imgui_SOURCE_DIR}/backends)

# Specify C++17 as the target C++ standard for this project.
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Compiler options for non-MSVC compilers. 
# Add or modify options as necessary.
if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror=return-type)
endif()

# Set default build type to Release if not specified. 
# Change this to Debug or another type if needed.
set(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Define the source path of the models
set(MODELS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/models")

# Define the destination path in the build directory
set(MODELS_DEST_DIR "${CMAKE_BINARY_DIR}/models")

# Copy models directory to build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODELS_SOURCE_DIR} ${MODELS_DEST_DIR}
)


add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env bash ${CMAKE_SOURCE_DIR}/scripts/post_build_script.sh
)




# Windows-specific configuration to handle runtime DLLs, crucial for deployment.
if(WIN32)
    # Ensure the correct handling of runtime DLLs.
    if(CMAKE_VERSION VERSION_LESS "3.21")
        message(WARNING "CMake version less than 3.21 detected. Automatic DLL handling is not available. Manually ensure DLLs are copied to the executable directory.")
    else()
        # Automatically copy required DLLs next to the executable.
        #  Useful for deployment and distribution.
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> 
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endif()

# Documentation Generation
# Find Doxygen package
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Configure the Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add a custom target to generate documentation
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()

