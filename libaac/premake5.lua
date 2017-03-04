----- libaac
name = "aac"
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

-------- Encoder
project ("aacz")
    targetdir "bin"
    kind "ConsoleApp"
    files {"src/aacz.cc"}
    includedirs {"include"}
    links {name} --- libaac

-------- Decoder
project ("aacx")
    targetdir "bin"
    kind "ConsoleApp"
    files {"src/aacx.cc"}
    includedirs {"include"}
    links {name} --- libaac

---------------- Testing
project (name.."-tests")
    targetdir "bin"
    kind "ConsoleApp"
    files {"tests/main.cc"}
    files {"tests/"..name.."/**.cc"}
    includedirs {"include"}
    links {name} -- libaac
