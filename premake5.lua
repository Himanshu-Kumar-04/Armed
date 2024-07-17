include "Dependencies.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


workspace "Armed"
	startproject "Editor"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

group "Dependencies"
	include "Armed/vendor/GLFW"
	include "Armed/vendor/Glad"
	include "Armed/vendor/gem"
	include "Armed/vendor/ImGui"
	include "Armed/vendor/yaml-cpp"
group ""

	include "Armed"
	include "Editor"