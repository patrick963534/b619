if not _PREMAKE_VERSION then
   os.execute('premake4 --file=premake4_makefile.lua clean')
   os.execute('premake4 --file=premake4_makefile.lua gmake')
   return
end


solution "libmz"
   configurations { "Debug", "Release" }
 
   -- A project defines one build target
   project "libmz"
      kind "ConsoleApp"
      language "C"
      location ( "build" )
      targetdir ( "bin" )
      libdirs { "/usr/lib" }
      links { "SDL", "SDL_image", "GLEW", "GL", "xml2" }
      includedirs { "/usr/include", "include", "/usr/include/libxml2" }
      files { "include/*.h", "src/**.h", "src/**.c" }
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }     
