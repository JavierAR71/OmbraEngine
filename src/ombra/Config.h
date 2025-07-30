//
// Created by Usuario on 30/07/2025.
//

#ifndef CONFIG_H
#define CONFIG_H
#pragma once

namespace ombra {
#ifdef NDEBUG
    constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
    constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif // NDEBUG

    constexpr int ENGINE_VERSION_MAJOR = 0;
    constexpr int ENGINE_VERSION_MINOR = 1;
    constexpr int ENGINE_VERSION_PATCH = 0;

    constexpr bool ENABLE_EDITOR_UI = false;
    constexpr bool ENABLE_HOT_RELOAD =ENABLE_VALIDATION_LAYERS;
}

#endif //CONFIG_H
