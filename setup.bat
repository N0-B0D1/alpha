@echo off

rem Pull down SDL2 from mercurial repository
IF EXIST ./3rdParty/SDL2/repo/.hg GOTO :SDLCLONED
echo Cloning down SDL2 release-2.0.3
cd ./3rdParty/SDL2
cd
rem hg init
rem hg pull -f http://hg.libsdl.org/SDL
rem hg update release-2.0.3
hg clone --updaterev release-2.0.3 http://hg.libsdl.org/SDL repo
cd ../../
cd
:SDLCLONED

rem Pause if an error occured
:error
pause