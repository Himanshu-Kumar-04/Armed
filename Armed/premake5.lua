project "Armed"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	pchheader "ArmPCH.h"
	pchsource "src/ArmPCH.cpp"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",

		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.VulkanSDK}"
	}
	
	libdirs{
		"%{LibraryDir.VulkanSDK}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32",
		"yaml-cpp",
		"%{Library.Vulkan}"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }
	
	filter "system:windows"
		systemversion "latest"

		defines{
			"ARMED_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines "ARM_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "ARM_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "ARM_DIST"
		optimize "On"
		runtime "Release"