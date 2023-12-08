@echo off

set CodeDir=..\src
set OutputDir=.\build

set CompilerFlags=-Od -nologo -Zi
set LinkerFlags=-opt:ref -incremental:no user32.lib gdi32.lib

IF NOT EXIST %OutputDir% mkdir $OutputDir%

pushd %OutputDir%

del *.pdb > NUL 2> NUL

call cl.exe %CompilerFlags% %CodeDir%\main.cpp %CodeDir%\FrameBuffer.cpp -Fmmain.map /link %LinkerFlags%

popd
