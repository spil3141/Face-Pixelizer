-- premake5.lua

workspace "Face-Pixelizer"
    architecture "x64"
    startproject "Face-Pixelizer"

   configurations 
   { 
       "Debug",
       "Release",
   }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "Face-Pixelizer"
    location "Face-Pixelizer"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{wks.location}/%{prj.name}/src/**.h",
        "%{wks.location}/%{prj.name}/src/**.cpp" 
    }

    removefiles 
    {
        "%{wks.location}/%{prj.name}/src/Junk/**.cpp",
        "%{wks.location}/%{prj.name}/src/Junk/**.h"
    }

    includedirs
    {
        "%{wks.location}/%{prj.name}/src",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/include",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/vendor/spdlog/include",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/vendor/ImGui",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/vendor/glm/include",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/vendor/asio/include",
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/vendor/EnTT/include"
    }

    libdirs 
    { 
        "%{wks.location}/%{prj.name}/vendor/Tiny_0.2/libs",
    }

    defines 
    {
    }

    links {
        "Tiny_debug",
        "Glad",
        "GLFW",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "T_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "T_RELEASE"
        optimize "On"
