project "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs{
		"%{wks.location}/Armed/src",
		"%{wks.location}/Armed/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"src"
	}

	links{
		"Armed"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"ARMED_PLATFORM_WINDOWS"
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