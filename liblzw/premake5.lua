---- liblzw
name = "lzw"
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

------ Library
project (name)
    targetdir "lib"
    files {"src/"..name.."/**.cc"}
    includedirs {"include"}
    filter {"platforms:Static"}
        defines {"STATIC"}
        kind "StaticLib"
    filter {"platforms:Shared"}
        defines {"SHARED"}
        kind "SharedLib"

---- Compression
project ("lzwc")
    targetdir "bin"
    kind "ConsoleApp"
    files {"src/lzwc/**.cc"}
    includedirs {"include"}
    links {name} --- liblzw

-- Decompression
project ("lzwd")
    targetdir "bin"
    kind "ConsoleApp"
    files {"src/lzwd/**.cc"}
    includedirs {"include"}
    links {name} --- liblzw

---------------- Testing
project (name.."-tests")
    targetdir "bin"
    kind "ConsoleApp"
    files {"tests/**.cc"}
    includedirs {"include"}
    links {name} --- liblzw
