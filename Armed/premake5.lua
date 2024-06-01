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
	}

	includedirs{
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"ARMED_PLATFORM_WINDOWS",
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