workspace "Trail"
	architecture "x64"
	startproject  "Sandbox"
	buildoptions "/utf-8"
	
	configurations
	{
		"Debug","Release","Dist"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Trail/vendor/GLFW/include"
IncludeDir["Glad"] = "Trail/vendor/Glad/include"
IncludeDir["ImGui"] = "Trail/vendor/imgui"
IncludeDir["glm"] = "Trail/vendor/glm"
IncludeDir["stb_image"] = "Trail/vendor/stb_image"

include "Trail/vendor/GLFW"
include "Trail/vendor/Glad"
include "Trail/vendor/imgui"


project "Trail"
	location "Trail"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++17"
	
	targetdir("bin/" ..outputDir.. "/%{prj.name}")
	objdir("bin-int/" ..outputDir.. "/%{prj.name}")


	pchheader "trlpch.h"
	pchsource "Trail/src/trlpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/src/Trail/Renderer/RendererAssets/**.png",
		"%{prj.name}/src/Trail/Renderer/RendererAssets/**.glsl",
		"%{prj.name}/vendor/glm/glm/**.h"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}


	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}



	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TRL_PLATFORM_WINDOWS",
			"TRL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "TRL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TRL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TRL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"
	buildoptions "/utf-8"

	targetdir("bin/" ..outputDir.. "/%{prj.name}")
	objdir("bin-int/" ..outputDir.. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/Shaders/**.glsl",
		"%{prj.name}/assets/Textures/**.png",
		"%{prj.name}/assets/Fonts/**.ttf",
		"%{prj.name}/assets/Sounds/**.mp3"
	}
	includedirs
	{
		"Trail/vendor/spdlog/include",
		"Trail/src",
		"Trail/vendor/glm",
		"Trail/vendor/Glad/include",
		"Trail/vendor"
		
	}
	links
	{
		"Trail"
	}

	postbuildcommands
	{
		"{COPYDIR} \"%{prj.location}assets\" \"%{cfg.targetdir}/Assets\""
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TRL_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "TRL_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "TRL_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "TRL_DIST"
		optimize "on"