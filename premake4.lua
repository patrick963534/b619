if not _PREMAKE_VERSION then
   os.execute('premake4 clean')
   os.execute('premake4 gmake')
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
      links { "SDL", "SDL_image", "GLEW", "GL" }
      includedirs { "/usr/include", "include" }
      files { "include/*.h", "src/**.h", "src/**.c" }
 
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }     
