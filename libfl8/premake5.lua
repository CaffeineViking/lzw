---libfl8---
name = "fl8"
workspace (name)
    language "C++"
    flags {"C++14"}
    targetdir "bin"
    location "build"
    warnings "Extra"
    configurations {"Debug", "Release",
                    "Static", "Shared"}
    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "Off"
        symbols "On"
    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "Speed"

-- The Library
project (name)
    targetdir "lib"
    filter {"configurations:Static"}
        kind "StaticLib"
    filter {"configurations:Shared"}
        kind "SharedLib"
    files {"src/fl8/**.cc"}
    includedirs {"include"}

-- Library Testing Suite
project (name.."-tests")
    kind "ConsoleApp"
    files {"tests/**.cc"}
    includedirs {"include"}
    links {name} -- libfl8.
