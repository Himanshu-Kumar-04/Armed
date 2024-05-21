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

	include "Armed/vendor/GLFW"
	include "Armed/vendor/Glad"
	include "Armed/vendor/ImGui"

	include "Armed"
	include "Editor"
	include "Sandbox"