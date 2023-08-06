workspace "SkyEngine"
   architecture "x64"
   configurations 
   { 
      "Debug", 
      "Release",
      "Dist"
   }
   
outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"
-- outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directors relative to root folder (solution directory)
IncludeDir = {}

project "SkyEngine"
   location "SkyEngine"
   kind "SharedLib"
   language "C++"

   targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
   objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

   pchheader "SEPCH.h"
   pchsource "SkyEngine/SEPCH.cpp"

   files 
   { 
      "%{prj.name}/**.h", 
      "%{prj.name}/**.cpp",
      "%{prj.name}/**.hpp",
      "README.md" 
   }
   
   removefiles 
   { 
      "%{prj.name}/Dependencies/**.*",
   }

   includedirs
   {
      "%{prj.name}",
      "%{prj.name}/Dependencies/include",
      "%{prj.name}/Dependencies/include/freetype",
   }

   libdirs
   {
      "%{prj.name}/Dependencies/lib/Win64/",
      "%{prj.name}/Dependencies/lib/Win64/freetype/",
      "%{prj.name}/Dependencies/lib/Win64/freeglut/",
      "%{prj.name}/Dependencies/lib/Win64/glew/",
      "%{prj.name}/Dependencies/lib/Win64/soil/",
      "%{prj.name}/Dependencies/lib/Win64/assimp/",
      "%{prj.name}/Dependencies/lib/Win64/FMOD/",
      "%{prj.name}/Dependencies/lib/Win64/vld/",
      "%{prj.name}/Dependencies/lib/Win64/Box2D/",
      "%{prj.name}/Dependencies/lib/Win64/GLFW/",
   }

   links
   {
      "opengl32.lib",
      "assimp-vc142-mtd.lib",
      "zlibstaticd.lib",
      "Box2D.lib",
      "fmod64_vc.lib",
      "freetype.lib",
      "glew64s.lib",
      "glfw3.lib",
      "glfw3dll.lib",
      "soil2.lib"
   }

   filter "system:windows"
      cppdialect "C++20"
      staticruntime "On"
      systemversion "latest"

      defines
      {
         "GLEW_STATIC",
         "SE_PLATFORM_WINDOWS",
         "SE_BUILD_DLL"
         -- _DEBUG;
         -- CPPDYNAMICLIBRARYTEMPLATE_EXPORTS;
         -- _WINDOWS;
         -- _USRDLL;
      }

      postbuildcommands
      {
         ("{COPY} %{cfg.buildtarget.relpath} ../Binaries/" .. outputdir .. "/SkyEditor")
      }

   filter "configurations:Debug"
      defines "SKYENGINE_DEBUG"
      symbols "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Debug"
      staticruntime "off"

   filter "configurations:Release"
      defines "SKYENGINE_RELEASE"
      optimize "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Release"
      staticruntime "off"

   filter "configurations:Dist"
   defines "SKYENGINE_DIST"
      optimize "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Release"
      staticruntime "off"

      
project "SkyEditor"
   location "SkyEditor"
   kind "ConsoleApp"
   language "C++"

   targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
   objdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

   files 
   { 
      "%{prj.name}/**.h", 
      "%{prj.name}/**.cpp",
      "%{prj.name}/Resources/**.*"
   }

   includedirs
   {
      "SkyEngine",
      "SkyEditor",
      "SkyEngine/Dependencies/include",
   }

   links
   {
      "SkyEngine"
   }

   filter "system:windows"
      cppdialect "C++20"
      staticruntime "On"
      systemversion "latest"

      defines
      {
         "SE_PLATFORM_WINDOWS"
         -- _DEBUG;
         -- CPPDYNAMICLIBRARYTEMPLATE_EXPORTS;
         -- _WINDOWS;
         -- _USRDLL;
      }

   filter "configurations:Debug"
      defines "SKYENGINE_DEBUG"
      symbols "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Debug"
      staticruntime "off"

   filter "configurations:Release"
      defines "SKYENGINE_RELEASE"
      optimize "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Release"
      staticruntime "off"

   filter "configurations:Dist"
   defines "SKYENGINE_DIST"
      optimize "On"
      -- Used to make multie threaded do dll mode - otherwise lib linking fails - Can be replaced once relevant vendors switched to projects within solution
      runtime "Release"
      staticruntime "off"


-- TODO SANDBOX
-- project "Sandbox"
--    location "Sandbox"
--    kind "ConsoleApp"
--    language "C++"

--    targetdir ("Binaries/" .. outputdir .. "/%{prj.name}")
--    targetdir ("Intermediate/" .. outputdir .. "/%{prj.name}")

--    files 
--    { 
--       "%{prj.name}/**.h", 
--       "%{prj.name}/**.c" 
--    }

--    includedirs
--    {
--       "SkyEngine"
--    }

--    links
--    {
--       "SkyEngine"
--    }

--    filter "system:windows"
--       cppdialect "C++20"
--       staticruntime "On"
--       systemversion "latest"

--       defines
--       {
--          "SE_PLATFORM_WINDOWS"
--          -- _DEBUG;
--          -- CPPDYNAMICLIBRARYTEMPLATE_EXPORTS;
--          -- _WINDOWS;
--          -- _USRDLL;
--       }

--    filter "configurations:Debug"
--       defines "SKYENGINE_DEBUG"
--       symbols "On"

--    filter "configurations:Release"
--       defines "SKYENGINE_RELEASE"
--       optimize "On"

--    filter "configurations:Dist"
--    defines "SKYENGINE_DIST"
--       optimize "On"