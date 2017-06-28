![Companion](https://libcompanion.github.io/libCompanion/images/logo.png "Companion")

[Companion](https://libcompanion.github.io/libCompanion/) is a computer vision library written with OpenCV 3.

# Supported Plattforms

* Windows
* UWP
* Linux

# Dependencies

Following dependencies are needed to build libCompanion.

* OpenCV Version 3.X
* OpenMP
* Cuda Support*

\* Optional

# Build LibCompanion

Simply use cmake or cmake-gui to build Companion for your platform.
Enable the Cuda flag to build LibCompanion with implemented Cuda features.

```
cmake -DCompanion_USE_CUDA=ON/OFF
make
make install
```

# Build Companion Samples

[Samples](https://github.com/LibCompanion/libCompanionSamples) are included as submodule or can be set in an cmake path. 
To build samples you must enable Companion_BUILD_SAMPLES flag.

## Build Samples from Submodule

```
git submodule update --init --recursive
cmake -DCompanion_BUILD_SAMPLES=ON
make
```

## Build Samples from Project

```
git clone https://github.com/LibCompanion/libCompanionSamples.git
cmake -DCompanion_BUILD_SAMPLES=ON -DSAMPLE_MODULE=/path/to/libCompanionSamples/
make
```

## UWP Support

If you desire to build Companion for *Universal Windows Platform* you can simply use the provided toolchain file to do so.
```
cmake -DCMAKE_TOOLCHAIN_FILE=UWPToolchain.cmake
```
If you prefer the cmake-gui instead, make sure to add the parameter ```CMAKE_TOOLCHAIN_FILE``` to the configuration before pressing *Configure*. Select FILEPATH as the parameter type and choose the file ```UWPToolchain.cmake``` as the parameter value.

# Getting Started

An usage example for an 2D Object detection setup is implemented under Main folder. 
For more Information go to this [Getting Started](https://nepitwin.github.io/libCompanion/gettingstarted/) Guide.

## License

This program is an image recognition library written with OpenCV.
Copyright (C) 2016-2017 Andreas Sekulski

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
