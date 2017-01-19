-- premake5.lua
name = "deflate"
workspace (name)
    language "C++"
    flags {"C++14"}
    targetdir "bin"
    location "build"
    warnings "Extra"
    configurations {"Debug", "Release"}
    filter {"configurations:Debug"}
        defines {"DEBUG"}
        optimize "Off"
        symbols "On"
    filter {"configurations:Release"}
        defines {"RELEASE"}
        optimize "Speed"

-- The Program
project (name)
    kind "ConsoleApp"
    files {"src/**.cc"}
    includedirs {"include"}
    -- links {"library"}

-- Program Testing Suite
project (name.."-tests")
    kind "ConsoleApp"
    files {"src/**.cc"}
    files {"tests/**.cc"}
    removefiles {"src/main.cc"}
    includedirs {"include"}
    -- links {"library"}
