#!/bin/sh

# initialize and update submodules
echo "Init and Update GIT submodules ..."
git submodule init
git submodule update

SDL_DIRECTORY=./3rdParty/SDL2

# Clone sdl2 repository into 3rdParty directory
if [ ! -d "$SDL_DIRECTORY" ]; then
    echo "Cloning SDL2 ..."
    hg clone --updaterev release-2.0.3 http://hg.libsdl.org/SDL $SDL_DIRECTORY
fi
