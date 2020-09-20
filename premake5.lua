workspace "PrEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
enginedir = "Game Engine/"
project "PrEngine"
	location "Game Engine"
	kind "WindowedApp"
	language "C++"
	targetdir ("Build/" .. outputdir .. "/%{prj.name}")
	objdir ("Build/Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		enginedir .. "/src/**.cpp",
		enginedir .. "/include/**.hpp",
		enginedir .. "/src/**.c",
		enginedir .. "/include/**.h",	
		enginedir .. "/Vendor/include/**.h",
		enginedir .. "/Vendor/src/**.cpp",
		enginedir .. "/Vendor/imgui-master/**.cpp",
		enginedir .. "/Vendor/imgui-master/**.h"
	}

	excludes{
		enginedir .. "/Vendor/imgui-master/misc/freetype/imgui_freetype.cpp",
		enginedir .. "/Vendor/imgui-master/misc/freetype/imgui_freetype.h",
		enginedir .. "/Vendor/imgui-master/misc/fonts/binary_to_compressed_c.cpp",
	}

	includedirs { 
		enginedir .. "/include",
		enginedir .. "/Vendor",
		enginedir .. "/Vendor/include",
		enginedir .. "/Vendor/imgui-master"

	}
	libdirs {"vendor/libs" }

	filter "system:windows"
	links
	{
		"SDL2",
		"SDL2main",
		"SDL2_mixer",
		"glew32",
		"OpenGL32"
	}

