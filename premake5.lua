-- The name of your workspace will be used, for example, to name the Visual Studio .sln file generated by Premake.
workspace "AdvanceOpenGL"
    -- We set the location of the files Premake will generate
    location "Generated"

    -- We indicate that all the projects are C++ only
    language "C++"

    -- We will compile for x86_32. You can change this to x86 for 32 bit builds.
    architecture "x86"

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
    -- kind is used to indicate the type of this project.
    kind "StaticLib"
    -- We specify where the source files are.
    -- It would be better to separate header files in a folder and sources
    -- in another, but for our simple project we will put everything in the same place.
    -- Note: ** means recurse in subdirectories, so it will get all the files in ExampleLib/
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

-- The windowed app
project "1.DepthTest"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/1.DepthTest/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "2.StencilTest"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/2.StencilTest/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "3.1.BlendingDiscard"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/3.1.BlendingDiscard/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "3.2.BlendingSort"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/3.2.BlendingSort/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "4.FaceCulling"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/4.FaceCulling/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "5.1.FrameBuffers"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/5.1.FrameBuffers/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "6.1.CubeMaps"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/6.1.CubeMaps/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "6.2.SkyBox"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/6.2.SkyBox/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "6.3.EnviromentMapping"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/6.3.EnviromentMapping/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "8.1.AdvanceGLSL"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/8.1.AdvanceGLSL/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "8.2.UniformBufferExample"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/8.2.UniformBufferExample/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }

-- The windowed app
project "9.1.GeometryShaderBasic"
    kind "ConsoleApp"

    filter { "system:Windows" }
    files "Libraries/common/*.h"

    filter { "system:Windows" }
    files "Projects/9.1.GeometryShaderBasic/**"

    -- We also need the headers
    includedirs "Projects/MainWindowLib"
    includedirs "Libraries"

    useOpenGLWindowLib()
    links "STB_IMAGE"
    -- Now we need to add the OpenGL system libraries

    filter { "system:windows" }
        links { "OpenGL32" }

    filter { "system:not windows" }
        links { "GL" }