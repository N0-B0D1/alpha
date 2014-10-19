Welcome to Project Alpha
========================

The purpose of Alpha is to learn, plain and simple.  I wanted to take what i've already learned, iterate over it, and apply it to a new project.

Building Alpha
--------------

Alpha uses CMake as its build system, which allows for easy cross-platform build support without having to manage the various solutions, projects, or make files independently. 

1. CMake GUI
------------

Using CMake GUI point the Source directory to the base directory in which Alpha resides.  Set the build directory to a folder inside of the Alpha directory (usually ```/build```)

Click configure, then Generate.  All project files for building alpha will now exist in the ```/build``` directory;

Build the ALL_BUILD project, then build the INSTALL project.

Assuming success, the built binaries, libraries, and include files will be placed into a ```/sdk``` folder under the Alpha directory.

License
-------

Copyright 2014 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0
    
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
