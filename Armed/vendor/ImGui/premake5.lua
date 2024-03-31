project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_demo.cpp",
		"imconfig.h",
		"imstb_rectpack.h",
		"imstb_truetype.h",
		"imstb_textedit.h",
		"imgui_widgets.cpp",
		"imgui_tables.cpp",
		"imgui_internal.h"
	}

	filter "system:windows"
		systemversion "latest"

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