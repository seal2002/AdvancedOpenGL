-- The name of your workspace will be used, for example, to name the Visual Studio .sln file generated by Premake.
workspace "AdvanceOpenGL"
    -- We set the location of the files Premake will generate
    location "Generated"

    -- We indicate that all the projects are C++ only
    language "C++"
    -- We will compile for x86_32. You can change this to x86 for 32 bit builds.
    architecture "x86"
    -- Use system lastest version for target to latest SDK for build Win32 version
    filter "system:Windows"
        systemversion "10.0.17134.0"
    -- Configurations are often used to store some compiler / linker settings together.
    -- The Debug configuration will be used by us while debugging.
    -- The optimized Release configuration will be used when shipping the app.
    configurations { "Debug", "Release" }

    -- We use filters to set options, a new feature of Premake5.

    -- We now only set settings for the Debug configuration
    filter { "configurations:Debug" }
        -- We want debug symbols in our debug config
        symbols "On"

    -- We now only set settings for Release
    filter { "configurations:Release" }
        -- Release should be optimized
        optimize "On"

    -- Reset the filter for other settings
    filter { }

    -- Here we use some "tokens" (the things between %{ ... }). They will be replaced by Premake
    -- automatically when configuring the projects.
    -- * %{prj.name} will be replaced by "ExampleLib" / "App" / "UnitTests"
    --  * %{cfg.longname} will be replaced by "Debug" or "Release" depending on the configuration
    -- The path is relative to *this* folder
    targetdir ("Build/Bin/%{prj.name}/%{cfg.longname}")
    objdir ("Build/Obj/%{prj.name}/%{cfg.longname}")

-- This function includes GLFW's header files
function includeGLFW()
    includedirs "Libraries/GLFW/include"
end

-- This function links statically against GLFW
function linkGLFW()
    libdirs "Libraries/GLFW/lib"

    -- Our static lib should not link against GLFW
    filter "kind:not StaticLib"
        links "glfw3"
    filter {}
end

-- This function links statically against assimp
function linkAssimp()
    libdirs "Libraries/assimp/lib"

    -- Our static lib should not link against assimp
    filter "kind:not StaticLib"
        links "assimp-vc140-mt"
    filter {}    
end

-- This function includes Freetype's header files
function includeFreetype()
    includedirs "Libraries/freetype/include"
end

-- This function links statically against Freetype
function linkFreetype()
    libdirs "Libraries/freetype/lib"

    -- Our static lib should not link against Freetype
    filter "kind:not StaticLib"
        links "freetype281d"
    filter {}
end

-- Our first project, the static library
project "GLAD"
    -- kind is used to indicate the type of this project.
    kind "StaticLib"
    -- We specify where the source files are.
    -- It would be better to separate header files in a folder and sources
    -- in another, but for our simple project we will put everything in the same place.
    -- Note: ** means recurse in subdirectories, so it will get all the files in ExampleLib/
    files "Projects/GLAD/**"

    -- We need GL, so we include it
    includedirs "Libraries"

-- Our second project, the static library
project "OpenGLWindow"
    -- kind is used to indicate the type of this project.
    kind "StaticLib"
    -- We specify where the source files are.
    -- It would be better to separate header files in a folder and sources
    -- in another, but for our simple project we will put everything in the same place.
    -- Note: ** means recurse in subdirectories, so it will get all the files in ExampleLib/
    files "Projects/MainWindow/**"

    -- We need GLFW, so we include it
    includeGLFW()
    includedirs "Libraries"

-- Our third project, the static library
project "STB_IMAGE"
    kind "StaticLib"
    files "Projects/STB_IMAGE/**"

    -- We need GL, so we include it
    includedirs "Libraries"

function useGLADLib()
    -- We link against a library that's in the same workspace, so we can just
    -- use the project name - premake is really smart and will handle everything for us.
    links "GLAD"
end

function useOpenGLWindowLib()
    -- The library's public headers
    includedirs "Projects/MainWindow"

    -- We link against a library that's in the same workspace, so we can just
    -- use the project name - premake is really smart and will handle everything for us.
    links "OpenGLWindow"

    -- Users of MainWindowLib need to link GLFW
    linkGLFW()
    useGLADLib()
end

-- Function Generate Project
function GenerateProject(s)
print(s)
project (s)
    kind "ConsoleApp"

    -- We also need the headers
    filter { "system:Windows" }
    files "Libraries/common/*.h"

    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"
    includeFreetype()

    useOpenGLWindowLib()
    linkFreetype()
    links { "STB_IMAGE" }
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

    -- filter { "system:windows" }
    -- vpaths {
    -- ["Headers"] = "Libraries/common/**.h",
    -- ["Sources/*"] = "./Projects/" .. s .. "/**.cpp",
    -- ["Shader"] = {"**.fs", "**.vs"}
    -- }

    filter { "system:Windows" }
    files { './Projects/' .. s .. '/**', }
end

-- Function Generate Project with Assimp
function GenerateProjectwithAssimp(s)
print(s)
project (s)
    kind "ConsoleApp"

    -- We also need the headers
    filter { "system:Windows" }
    files "Libraries/common/*.h"

    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"
    includeFreetype()
    includedirs "Libraries/assimp"

    useOpenGLWindowLib()
    linkFreetype()
    linkAssimp()
    links { "STB_IMAGE" }
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

    filter { "system:windows" }
        -- copy a file from the objects directory to the target directory
        postbuildcommands {
            "{COPY} %{wks.location}..\\dlls\\assimp-vc140-mt.dll %{cfg.targetdir}"
        }
    -- filter { "system:windows" }
    -- vpaths {
    -- ["Headers"] = "Libraries/common/**.h",
    -- ["Sources/*"] = "./Projects/" .. s .. "/**.cpp",
    -- ["Shader"] = {"**.fs", "**.vs"}
    -- }

    filter { "system:Windows" }
    files { './Projects/' .. s .. '/**', }
end

-- List of Project
Projects = { "1.DepthTest", "2.StencilTest", "3.1.BlendingDiscard",
             "3.2.BlendingSort", "4.FaceCulling", "5.1.FrameBuffers",
             "6.1.CubeMaps", "6.2.SkyBox", "6.3.EnviromentMapping",
             "8.1.AdvanceGLSL", "8.2.UniformBufferExample", "9.1.GeometryShaderBasic", "9.2.GeometryShaderHouses",
             "10.1.InstancesQuads", "10.2.InstancesArrays", "11.1.AntiAliasingMultiSampling", "11.2.OffScreenMSAA",
             "TextRendering"}

for key, value in ipairs(Projects) do
    print(key, value)
    GenerateProject(value)
end

ProjectsAssimp = { "9.3.ExplodingObjects", "9.4.VisualizingNormalVector", "10.3.AsteroidField", "10.4.AsteroidInstanced"}

for key, value in ipairs(ProjectsAssimp) do
    print(key, value)
    GenerateProjectwithAssimp(value)
end
        
if _ACTION == "clean" then
   os.rmdir("./Generated")
   os.rmdir("./Build")
end