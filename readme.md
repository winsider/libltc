# CMake C++ boot project
Simple C++ boot project for getting started with a CMake C++ project quickly.

     $ git clone https://github.com/winsider/ cppboot.git yourprojectname
     $ cd yourprojectname
     $ ./init.sh yourprojectname
    
## Build
Create a directory called build and change to this folder.

To regenerate make files:
     
     cmake -DCMAKE_BUILD_TYPE=Release ..

or for debug:

     cmake -DCMAKE_BUILD_TYPE=Debug ..


## IMGUI
To add imgui to the project:

    $ git submodule add https://github.com/ocornut/imgui.git imgui


