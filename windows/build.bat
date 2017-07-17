@echo off

set commonCompilerFlags=-O2 -MT -nologo -GR- -Gm- -EHsc- -Oi -FC -Zi -Iinclude -Isrc -arch:AVX
set commonLinkerFlags=-opt:ref -incremental:no user32.lib gdi32.lib winmm.lib opengl32.lib glew32.lib

del bin\*.pdb >nul 2>nul
cl %commonCompilerFlags% freetype28.lib src/RacingToHell.cpp /Febin\RacingToHell.dll -LD /link -incremental:no /PDB:bin\RacingToHell%random%.pdb /EXPORT:updateAndRender /EXPORT:getSoundSamples
cl %commonCompilerFlags% windows/win32_RacingToHell.cpp /Febin\RacingToHell.exe -Fmwin32_handmade.map /link %commonLinkerFlags%
