//
// Created by Usuario on 30/07/2025.
//

#ifndef CONFIG_H
#define CONFIG_H
#pragma once

namespace ombra {
#ifdef NDEBUG
    constexpr bool ENABLE_VALIDATION_LAYERS = false;
    constexpr bool ENABLE_LOGGING = false;
#else
    constexpr bool ENABLE_VALIDATION_LAYERS = true;
    constexpr bool ENABLE_LOGGING = true;
#endif // NDEBUG

    constexpr int ENGINE_VERSION_MAJOR = 0;
    constexpr int ENGINE_VERSION_MINOR = 1;
    constexpr int ENGINE_VERSION_PATCH = 0;

#define OMBRA_ENGINE_VERSION \
VK_MAKE_VERSION(ombra::ENGINE_VERSION_MAJOR, ombra::ENGINE_VERSION_MINOR, ombra::ENGINE_VERSION_PATCH)

    constexpr bool ENABLE_EDITOR_UI = false;
    constexpr bool ENABLE_HOT_RELOAD =ENABLE_VALIDATION_LAYERS;
}

#endif //CONFIG_H
