﻿cmake_minimum_required(VERSION 3.15)
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
        src/ombra/Application.cpp
        src/ombra/Application.h
        src/ombra/vulkan/Instance.cpp
        src/ombra/vulkan/Instance.h
        src/ombra/vulkan/DebugMessenger.cpp
        src/ombra/vulkan/DebugMessenger.h
        src/ombra/Config.h
        src/ombra/vulkan/Surface.cpp
        src/ombra/vulkan/Surface.h
        src/ombra/vulkan/PhysicalDevice.cpp
        src/ombra/vulkan/PhysicalDevice.h
        src/ombra/core/Macros.h
        src/ombra/core/OmbraCore.h
        src/ombra/vulkan/Device.cpp
        src/ombra/vulkan/Device.h
        src/ombra/vulkan/Swapchain.cpp
        src/ombra/vulkan/Swapchain.h
        src/ombra/vulkan/renderer/Renderer.cpp
        src/ombra/vulkan/renderer/Renderer.h
)
target_include_directories(OmbraEngine PRIVATE src/ombra)
target_include_directories(OmbraEngine PRIVATE src/ombra src/ombra/vulkan)
target_link_libraries(OmbraEngine PRIVATE Vulkan::Vulkan glfw)