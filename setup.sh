#!/bin/sh

SDL_DIRECTORY=./3rdParty/SDL2

# initialize and update submodules
echo "Init and Update GIT submodules ..."
git submodule init
git submodule update

# remove sdl repo directory if it exists
if [ -d "$SDL_DIRECTORY/repo" ]; then
    echo "Removing existing SDL2 repository ..."
    rm -rf $SDL_DIRECTORY/repo
fi

# Clone sdl2 repository into 3rdParty directory
if [ ! -d "$SDL_DIRECTORY/repo" ]; then
    echo "Cloning SDL2 ..."
    hg clone --updaterev release-2.0.3 http://hg.libsdl.org/SDL $SDL_DIRECTORY/repo
fi

# copy source and include files into base sdl2 directory
if [ -d "$SDL_DIRECTORY/repo" ]; then
    echo "Copying header files ..."
    cp -rv $SDL_DIRECTORY/repo/include $SDL_DIRECTORY/include

    echo "Copying source files ..."
    cp -rv $SDL_DIRECTORY/repo/src $SDL_DIRECTORY/src

    echo "Copying cmake files ..."
    cp -rv $SDL_DIRECTORY/repo/cmake $SDL_DIRECTORY/cmake

    echo "Copying cmake files ..."
    cp -v $SDL_DIRECTORY/repo/sdl2-config.in $SDL_DIRECTORY
    cp -v $SDL_DIRECTORY/repo/sdl2.pc.in $SDL_DIRECTORY
    cp -v $SDL_DIRECTORY/repo/SDL2.spec.in $SDL_DIRECTORY
    cp -v $SDL_DIRECTORY/repo/sdl2.m4 $SDL_DIRECTORY
fi
