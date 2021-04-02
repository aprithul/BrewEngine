workspace "PrEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
enginedir = "Game_Engine/"
gamedir = "Game/"

project "PrEngine"
	location "Game_Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	targetdir (enginedir .. "Build/" .. outputdir .. "/%{prj.name}")
	objdir (enginedir .. "Build/Obj/" .. outputdir .. "/%{prj.name}")
	staticruntime "on"

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

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"
	targetdir (gamedir .. "Build/" .. outputdir .. "/%{prj.name}")
	objdir (enginedir .. "Build/Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		gamedir .. "/src/**.cpp",
		gamedir .. "/include/**.hpp"
	}

	includedirs { 
		enginedir .. "/include",
		enginedir .. "/Vendor",
		enginedir .. "/Vendor/include",
		enginedir .. "/Vendor/imgui-master"
	}

	links{
		"PrEngine"
	}


