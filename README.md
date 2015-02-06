Welcome to Project Alpha
========================

Alpha is a multi-platform game engine which currently supports Windows and Linux systems.  The purpose of Alpha is to learn, plain and simple.  I wanted to take what i've already learned, iterate over it, and apply it to a new project.

Alpha Requirements
------------------

- All
 * CMake 2.6+

- Windows
 * DirectX 11 (June 2010 SDK)
 * Visual Studio 11+

- Linux
 * OpenGL 3/4
 * X11
 * GCC 4.5+ / Clang 3.1+

Building Alpha
--------------

Alpha uses CMake as its build system, which allows for easy cross-platform build support without having to manage the various solutions, projects, or make files on a per system basis.  The build system has been tested with modern Visual Studio releases (11+) and with Unix Makefiles in linux.  While others may work, they have not been tested.

1. Set Source Directory to the root alpha directory.
2. Set Build Directory to the target output, usually ```/build``` inside alpha root.
3. Configure, select target build system (Visual Studio, Unix Makefiles, etc.)
4. Generate.
5. In build output directory, the generated project solution should have a ALL_BUILD target.  Or simply us make to build everything.
6. Building the INSTALL target will output include, libs, and any demo projects to a cmake specified intall directory.

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
