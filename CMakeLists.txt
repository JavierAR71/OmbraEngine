cmake_minimum_required(VERSION 3.15)
project(OmbraEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Vulkan
find_package(Vulkan REQUIRED)

# GLFW usando FetchContent
include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        latest
)
FetchContent_MakeAvailable(glfw)

# Archivos fuente del motor
file(GLOB_RECURSE OMBRA_ENGINE_SRC CONFIGURE_DEPENDS src/ombra/*.cpp)

# Ejecutable principal
add_executable(OmbraEngine ${OMBRA_ENGINE_SRC}
        src/ombra/main.cpp
)
target_include_directories(OmbraEngine PRIVATE src/ombra)
target_link_libraries(OmbraEngine PRIVATE Vulkan::Vulkan glfw)