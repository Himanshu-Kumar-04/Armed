IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Armed/vendor/stb_image"
IncludeDir["GLFW"] = "%{wks.location}/Armed/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Armed/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Armed/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/Armed/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/Armed/vendor/entt"
IncludeDir["VulkanSDK"] = os.getenv("VULKAN_SDK") .. "/Include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Armed/vendor/yaml-cpp/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = os.getenv("VULKAN_SDK") .. "/Lib/"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

