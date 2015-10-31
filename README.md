Welcome to Project Alpha
========================

Alpha is a multi-platform game engine which currently supports Windows and Linux systems.  The purpose of Alpha is to learn, plain and simple.  I wanted to take what i've already learned, iterate over it, and apply it to a new project.

Alpha Requirements
------------------

- All
 * CMake 2.6+
 * Git
 * Mercurial

- Windows
 * DirectX 11.1
 * DXSDK June 2010 (Required for SDL2 Audio)

- Linux
 * OpenGL 3.3+
 * X11
 * GLEW
 * GLFW
 * GLUT
 * GCC 4.5+ / Clang 3.1+

Building Alpha
--------------

Alpha uses CMake as its build system, which allows for easy cross-platform build support without having to manage the various solutions, projects, or make files on a per system basis.  The build system has been tested with modern Visual Studio releases (11+) and with Unix Makefiles in linux.  While others may work, they have not been tested.

### Pre-build

Before building, all dependencies and initialization must be complete.  Once
all requirements listed above are meet on your system, run the setup script; on
linux run setup.sh, and on windows run setup.bat.

The setup script will initialize the submodules for the project and update
them.  Then it will attempt to use mercurial to pull the SDL2 sources into the
3rdParty directory.  Obviously, mercurial must be install on your system at
this point for it to be successful.

1. ```sh setup.sh``` -or- ```setup.bat```

### CMake

1. Set Source Directory to the root alpha directory.
2. Set Build Directory to the target output, usually ```/build``` inside alpha root.
3. Configure, select target build system (Visual Studio, Unix Makefiles, etc.)
4. Generate.

### Build

Build projects/solutions should now have been created in the build folder specified.

License
-------

> Copyright 2014-2015 Jason R. Wendlandt
> 
> Licensed under the Apache License, Version 2.0 (the "License");
> you may not use this file except in compliance with the License.
> You may obtain a copy of the License at
> 
>     http://www.apache.org/licenses/LICENSE-2.0
>     
> Unless required by applicable law or agreed to in writing, software
> distributed under the License is distributed on an "AS IS" BASIS,
> WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
> See the License for the specific language governing permissions and
> limitations under the License.
