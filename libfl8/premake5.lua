---libfl8---
name = "fl8"
workspace (name)
    language "C++"
    flags {"C++14"}
    location "build"
    warnings "Extra"
    platforms {"Static", "Shared"}
    configurations {"Debug", "Release"}
    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "Off"
        symbols "On"
    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "Speed"
        symbols "Off"

-- The Library
project (name)
    targetdir "lib"
    files {"src/fl8/**.cc"}
    includedirs {"include"}
    filter {"platforms:Static"}
        defines {"STATIC"}
        kind "StaticLib"
    filter {"platforms:Shared"}
        defines {"SHARED"}
        kind "SharedLib"

-- Library Testing Suite
project (name.."-tests")
    targetdir "bin"
    kind "ConsoleApp"
    files {"tests/**.cc"}
    includedirs {"include"}
    links {name} -- libfl8.
