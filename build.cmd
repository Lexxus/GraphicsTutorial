@echo off

set CodeDir=..\src
set OutputDir=.\build

set CompilerFlags=-Od -FeGraphicsTutorial.exe -nologo -Zi
set LinkerFlags=-opt:ref -incremental:no user32.lib gdi32.lib
set SourceFiles=%CodeDir%\*.cpp

IF NOT EXIST %OutputDir% mkdir $OutputDir%

pushd %OutputDir%

del *.pdb > NUL 2> NUL

call cl.exe %CompilerFlags% %SourceFiles% -Fmmain.map /link %LinkerFlags%

popd
