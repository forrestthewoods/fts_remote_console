-- LICENSE
--
--   This software is dual-licensed to the public domain and under the following 
--   license: you are granted a perpetual,   irrevocable license to copy, modify, 
--   publish, and distribute this file as you see fit.



ROOT_DIR = path.getabsolute("../..")
SLN_DIR = path.join(ROOT_DIR, "projects", _ACTION)

PLATFORMS = { "x32", "x64" }
BUILDS = { "debug", "release" }

if (_ACTION and not string.startswith(_ACTION, "vs")) then
    SLN_DIR = SLN_DIR.."_"..os.get()
end


-----------------------------------
function fts_project(project_name)

        configuration {}

        objdir (path.join(SLN_DIR, "build"))

        configuration "debug"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "release"
            defines { "NDEBUG" }
            flags { "Optimize" }

        configuration "windows"
            defines { 
                "FTS_WINDOWS", 
                "_WIN32_WINNT=0x0601",  -- Windows 7 minimum
            }

        configuration "linux"
            defines { "FTS_LINUX" }
            buildoptions_cpp { "-std=c++11", }

        configuration "osx"
            defines { "FTS_OSX" }
            buildoptions_cpp { "-std=c++11", }

            for i, platform in ipairs(PLATFORMS) do
                for j, build in ipairs(BUILDS) do
                    configuration { platform, build }
                    targetdir (path.join(SLN_DIR, "bin", platform .. "_" .. build, project_name))
                end
            end

        configuration {}
end


-----------------------------------
solution "fts_console"
    configurations (BUILDS)
    platforms (PLATFORMS)
    location (SLN_DIR)


   -----------------------------------
    project "fts_remote_console"

        kind "WindowedApp"
        language "C++"
        location (path.join(SLN_DIR, "projects"))
        flags { "ExtraWarnings", "NoEditAndContinue" }

        files {
            path.join(ROOT_DIR, "code/remote_console/*.h"),
            path.join(ROOT_DIR, "code/remote_console/*.cpp"),
            path.join(ROOT_DIR, "code/lib_fts/*.h"),
            path.join(ROOT_DIR, "code/lib_fts/*.cpp"),
        }

        includedirs {
            path.join(ROOT_DIR, "code/thirdparty"),
            path.join(ROOT_DIR, "code/thirdparty/asio"),
            path.join(ROOT_DIR, "code/thirdparty/imgui"),
            path.join(ROOT_DIR, "code/thirdparty/glfw/include"),
            path.join(ROOT_DIR, "code") 
        }

        links {
            -- internal
            "imgui",
            "glfw",
            "flatbuffers",
            "net",
        }

        defines {
            "ASIO_STANDALONE"
        }

        configuration "windows"
            links {
                "ws2_32",
                "shcore",
                "opengl32"
            }

        configuration "not windows"
            links {
            }

        fts_project("fts_remote_console")

    -----------------------------------
    project "example_game"

        kind "ConsoleApp"
        language "C++"
        location (path.join(SLN_DIR, "projects"))
        flags { "ExtraWarnings", "NoEditAndContinue" }

        files {
            path.join(ROOT_DIR, "code/example_game/*.h"),
            path.join(ROOT_DIR, "code/example_game/*.cpp"),
        }

        includedirs {
            path.join(ROOT_DIR, "code/thirdparty"),
            path.join(ROOT_DIR, "code/thirdparty/asio"),
            path.join(ROOT_DIR, "code") 
        }

        links {
            "flatbuffers",
            "net",
        }

        defines {
            "ASIO_STANDALONE"
        }

        -- external
        configuration "windows"
            links {
                "ws2_32",
            }

        configuration "not windows"
            links {
                "pthread"
            }


        fts_project("example_game")


    -----------------------------------
    project "protocol"

        kind "StaticLib"
        language "C++"
        location (path.join(SLN_DIR, "projects"))
        flags { "NoEditAndContinue" }

        -- $FTS Want fbs files marked as "Custom Build Tool" in VS
        files { 
            path.join(ROOT_DIR, "code/protocol/**.fbs"),
            path.join(ROOT_DIR, "code/protocol/**.h"),
            path.join(ROOT_DIR, "code/protocol/**.cpp"),
        }

        -- $$$FTS want to generate these automatically from **.fbs
        files {
            path.join(ROOT_DIR, "code/protocol/protocol_generated.h"),
        }

        includedirs { 
            path.join(ROOT_DIR, "code/thirdparty"),
        }

        links {
            -- built from solution
            "flatbuffers",
        }
        
        -- Execute flatc (compiled by flatbuffers project) to read .fbs files and generate C++ code
        for i, platform in ipairs(PLATFORMS) do
            for j, build in ipairs(BUILDS) do
                configuration { platform, build, "vs*" }
                    prebuildcommands { "$(SolutionDir)bin/"..platform.."_"..build.."/flatbuffers/flatc ".."--cpp --scoped-enums -o ../../../code/protocol/ ../../../code/protocol/protocol.fbs" }
                configuration { platform, build, "not vs*" }
                   prebuildcommands { "../bin/"..platform.."_"..build.."/flatbuffers/flatc ".."--cpp --scoped-enums -o ../../../code/protocol/ ../../../code/protocol/protocol.fbs" }
            end
        end

        fts_project("protocol")


    -----------------------------------
    project "net"

        kind "StaticLib"
        language "C++"
        location (path.join(SLN_DIR, "projects"))
        flags { "NoEditAndContinue" }

        files { 
            path.join(ROOT_DIR, "code/net/**.h"),
            path.join(ROOT_DIR, "code/net/**.cpp"),
            path.join(ROOT_DIR, "code/thirdparty/asio/**.hpp"),
            path.join(ROOT_DIR, "code/thirdparty/asio/**.ipp"),
        }

        includedirs { 
            path.join(ROOT_DIR, "code/thirdparty"),
            path.join(ROOT_DIR, "code/thirdparty/asio"),
            path.join(ROOT_DIR, "code"),
        }

        links {
            -- built from solution
            "protocol",
        }

        defines {
            "ASIO_STANDALONE"
        }
        
        fts_project("net")


        
   -----------------------------------
    group "thirdparty"
      
       -----------------------------------
        project "flatbuffers"

            kind "ConsoleApp"
            language "C++"
            targetname "flatc"
            location (path.join(SLN_DIR, "projects"))
            flags { "NoEditAndContinue" }

            files { 
                path.join(ROOT_DIR, "code/thirdparty/flatbuffers/**.h"),
                path.join(ROOT_DIR, "code/thirdparty/flatbuffers/**.cpp"),
            }

            includedirs { 
                path.join(ROOT_DIR, "code/thirdparty"), 
            }

            defines {
                "FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE"
            }

            fts_project("flatbuffers")


        -----------------------------------
        project "glfw"

            kind "StaticLib"
            language "C++"
            location (path.join(SLN_DIR, "projects"))
            flags { "NoEditAndContinue" }

            glfw_src = path.join(ROOT_DIR, "code/thirdparty/glfw/src")

            files { 
                path.join(ROOT_DIR, "code/thirdparty/glfw/include/**.h"),

                path.join(glfw_src, "egl_context.h"),
                path.join(glfw_src, "glfw_config.h"),
                path.join(glfw_src, "internal.h"),

                path.join(glfw_src, "context.c"),
                path.join(glfw_src, "egl_context.c"),
                path.join(glfw_src, "init.c"),
                path.join(glfw_src, "input.c"),
                path.join(glfw_src, "monitor.c"),
                path.join(glfw_src, "vulkan.c"),
                path.join(glfw_src, "window.c"),
            }

            includedirs { 
                path.join(ROOT_DIR, "code/thirdparty"), 
            }

            configuration "windows"
                files { 
                    path.join(glfw_src, "wgl_context.h"),
                    path.join(glfw_src, "wgl_context.c"),
                    path.join(glfw_src, "win32**.h"),
                    path.join(glfw_src, "win32**.c"),
                }

                defines {
                    "_GLFW_WIN32",
                    "_CRT_SECURE_NO_WARNINGS"
                }

            configuration "linux"
                files { 
                    path.join(glfw_src, "linux**.h"),
                    path.join(glfw_src, "linux**.c"),
                    path.join(glfw_src, "posix**.h"),
                    path.join(glfw_src, "posix**.c"),
                    path.join(glfw_src, "x11**.h"),
                    path.join(glfw_src, "x11**.c"),
                    path.join(glfw_src, "glx_context.h"),
                    path.join(glfw_src, "glx_context.c"),
                }

                defines {
                    "_GLFW_X11"
                }

            configuration "osx"
                files {
                    path.join(ROOT_DIR, glfw_src, "posix**.h"),
                    path.join(ROOT_DIR, glfw_src, "posix**.c"),
                    path.join(ROOT_DIR, glfw_src, "cocoa**.h"),
                    path.join(ROOT_DIR, glfw_src, "cocoa**.c"),
                }
                defines {
                    "_GLFW_COCOA"
                }

            fts_project("imgui")


        -----------------------------------
        project "imgui"

            kind "StaticLib"
            language "C++"
            location (path.join(SLN_DIR, "projects"))
            flags { "NoEditAndContinue" }

            files { 
                path.join(ROOT_DIR, "code/thirdparty/imgui/**.h"),
                path.join(ROOT_DIR, "code/thirdparty/imgui/**.cpp"),
            }

            includedirs { 
                path.join(ROOT_DIR, "code/thirdparty") 
            }

            fts_project("imgui")
