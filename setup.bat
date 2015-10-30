@echo off

echo Init and Update GIT submodules ...
git submodule init
git submodule update

rem Pull down SDL2 from mercurial repository
IF EXIST ./3rdParty/SDL2/repo GOTO :SDL_CLONED
	echo Cloning down SDL2 release-2.0.3
	cd ./3rdParty/SDL2
	cd
	
	hg clone --updaterev release-2.0.3 http://hg.libsdl.org/SDL repo
	cd ../../
	cd
:SDL_CLONED

IF NOT EXIST ./3rdParty/SDL2/repo GOTO :SDL_CLONE_FAILED
	IF EXIST ./3rdParty/SDL2/include GOTO :SDL_INCLUDE_COPIED
		echo Copying header files ...
		xcopy "./3rdParty/SDL2/repo/include" "./3rdParty/SDL2/include" /E /H /C /R /Q /Y /I /S /L
	:SDL_INCLUDE_COPIED
	
	IF EXIST ./3rdParty/SDL2/src GOTO :SDL_SRC_COPIED
		echo Copying source files ...
		xcopy "./3rdParty/SDL2/repo/src" "./3rdParty/SDL2/src" /E /H /C /R /Q /Y /I /S /L
	:SDL_SRC_COPIED
	
	IF EXIST ./3rdParty/SDL2/cmake GOTO :SDL_CMAKE_COPIED
		echo Copying cmake files ...
		xcopy "./3rdParty/SDL2/repo/cmake" "./3rdParty/SDL2/cmake" /E /H /C /R /Q /Y /I /S /L
	:SDL_CMAKE_COPIED
	
	echo Copying config files ...
	xcopy "./3rdParty/SDL2/repo/sdl2-config.in" "./3rdParty/SDL2/sdl2-config.in" /C /R /Y /F /I
	xcopy "./3rdParty/SDL2/repo/sdl2.pc.in" "./3rdParty/SDL2/sdl2.pc.in" /C /R /Y /F /I
	xcopy "./3rdParty/SDL2/repo/SDL2.spec.in" "./3rdParty/SDL2/SDL2.spec.in" /C /R /Y /F /I
	xcopy "./3rdParty/SDL2/repo/sdl2.m4" "./3rdParty/SDL2/sdl2.m4" /C /R /Y /F /I
:SDL_CLONE_FAILED

rem Pause if an error occured
:error
pause