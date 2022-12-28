# UsdAssimp

UsdAssimp is a USD file format plugin for USD leveraging the [Assimp](https://github.com/assimp/assimp/) library to convert 3d file formats to USD. 
This is an early prototype and although it should ultimately be able to convert most of the [assimp supported formats](https://github.com/assimp/assimp/blob/master/doc/Fileformats.md), I am focusing on getting the PLY format working well first. I you are interested by this project, don't hesitate to reach out. Contributions are very welcome !


## Requirements

This project needs:

    - [CMake](https://cmake.org/) > 3.14
    - C++14 compatible compiler, clang, g++, MSVC++ (only tested with clang yet)
    - An installed version of [USD](https://github.com/PixarAnimationStudios/USD/releases/tag/v22.11), version 22.08 works, the other versions and prepackage usd libs have not been tested.
    - An internet connection: Assimp will be downloaded and compiled during the build process, no need for installing it beforehand, but a internet connection will be necessary for the download.


## Building

CMake is used to create IDE projects or Makefiles. CMake needs to know where the USD libraries have been installed and where to install usdassimp by passing the following command line arguments:

- __pxr_DIR__ pointing to the USD installation directory containing the file pxrConfig.cmake

- __CMAKE_INSTALL_PREFIX__ should point to where you want to install usdassimp


Setting up the project on linux or mac could look like:

    git clone https://github.com/cpichard/usdassimp
    cd usdassimp
    mkdir build
    cd build
    cmake -Dpxr_DIR=/path/to/usd-22.08 -DCMAKE_INSTALL_PREFIX=/path/to/usdassimp-install ..

NOTE: be sure to replace "/path/to/xxxx" with your paths.

Once the project is setup you can build with the make command (or your IDE)


## Installing and testing

Install with 

    make install

To make the plugin available to USD, you need to tell USD where it lives. You can do that by adding the usdassimp installation path to the environment variable __PXR_PLUGIN_PATH_NAME__. If you installed usdassimp in /path/to/usdassimp-install, then the path you need to add is /path/to/usdassimp-install/plugin/usd. For instance using a linux bash terminal:

    export PXR_PLUGINPATH_NAME=/path/to/usdassimp-install/plugin/usd:$PXR_PLUGINPATH_NAME

The format ply should now be recognized by usd, you can test using usdview or usdcat and the examples in the tests folder.

    usdcat tests/example1.ply -o example1.usda


## Licence



