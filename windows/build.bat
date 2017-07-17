@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

set commonCompilerFlags=-O2 -MT -nologo -GR- -Gm- -EHsc- -Oi -FC -Zi -Iinclude -Isrc -arch:AVX
set commonLinkerFlags=-opt:ref -incremental:no user32.lib gdi32.lib winmm.lib opengl32.lib

mkdir bin
del bin\*.pdb >nul 2>nul
cl %commonCompilerFlags% windows/win32_RacingToHell.cpp /Febin\RacingToHell.exe -Fmwin32_handmade.map /link %commonLinkerFlags%
cl %commonCompilerFlags% opengl32.lib glew32.lib freetype28.lib src/RacingToHell.cpp /Febin\RacingToHell.dll -LD /link -incremental:no /PDB:bin\RacingToHell%random%.pdb /EXPORT:updateAndRender /EXPORT:getSoundSamples
