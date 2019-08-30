workspace "CTPL"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "CTPL"
	location "CTPL"
	kind "ConsoleApp"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin/Intermidiates/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/vlib/core/**.h",
		"%{prj.name}/src/vlib/core/**.c"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src"
	}

	filter "configurations:Debug"
		defines "CG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CG_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "CG_DIST"
		optimize "On"
