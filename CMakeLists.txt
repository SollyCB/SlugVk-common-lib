cmake_minimum_required(VERSION 3.2)
project(SlugVk-lib-common)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

option(BUILD_TESTS ON)
option(DEBUG ON)

if(DEBUG)
    message("Building DEBUG...")
    set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    set(CMAKE_BUILD_TYPE DEBUG)
    add_compile_definitions(DEBUG=true)
else()
    message("Building RELEASE...")
    set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
    set(CMAKE_BUILD_TYPE RELEASE)
    add_compile_definitions(DEBUG=false)
endif()

add_library(SlugVk-lib-common
    Allocator.cpp
    Camera.cpp
    Clock.cpp
    CMakeLists.txt
    File.cpp
    glTF.cpp
    Image.cpp
    QuartCamera.cpp
    Spv.cpp
    String.cpp
    VulkanErrors.cpp

    external/tlsf.cpp
)
target_include_directories(SlugVk-lib-common PUBLIC . ./external ./external/nlohmann)
target_compile_options(SlugVk-lib-common PRIVATE "${GCC_COVERAGE_COMPILE_FLAGS}")
target_link_libraries(SlugVk-lib-common PRIVATE "${GCC_COVERAGE_LINK_FLAGS}")

if (BUILD_TESTS)
    message("Building TESTS...")

    add_executable(SlugVk-lib-common-TestAll
        test/test_entrypoint.cpp
        test/test.cpp
        test/spv_test.cpp
    )
    target_link_libraries(SlugVk-lib-common-TestAll PRIVATE
        SlugVk-lib-common
        "${GCC_COVERAGE_LINK_FLAGS}"
    )
    target_include_directories(SlugVk-lib-common-TestAll PRIVATE .)
    target_compile_options(SlugVk-lib-common-TestAll PRIVATE "${GCC_COVERAGE_COMPILE_FLAGS}")
endif()
