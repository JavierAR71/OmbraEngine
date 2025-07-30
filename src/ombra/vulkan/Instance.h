#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

namespace ombra::vulkan {

    class Instance {
    public:
        Instance(const std::string& applicationName, uint32_t applicationVersion);
        ~Instance();

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;

        VkInstance get() const { return m_instance; }

    private:
        VkInstance m_instance = VK_NULL_HANDLE;

        const std::vector<const char*> m_validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };
        const bool m_enableValidationLayers =
#ifdef NDEBUG
        false;
#else
        true;
#endif


        bool checkValidationLayerSupport() const;
        std::vector<const char*> getRequiredExtensions() const;
    };

}

