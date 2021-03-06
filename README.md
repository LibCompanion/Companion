![Companion](https://libcompanion.github.io/CompanionDoc/images/logo.png "Companion")

[![Build Status](https://travis-ci.org/LibCompanion/Companion.svg?branch=master)](https://travis-ci.org/LibCompanion/Companion)

[Companion](https://libcompanion.github.io/CompanionDoc/) is a object recognition framework written with OpenCV 3.

# Documentation

Documentation page can be reached at https://libcompanion.github.io/CompanionDoc/.

# Supported Platforms

* Windows
* Universal Windows Platform (UWP)
* Linux

# Dependencies

Following dependencies are needed to build Companion.

* OpenCV Version 3.X
* Cuda 8 (Cuda 9 is currently not supported) to use Cuda features.

# Build Companion

Simply use CMake or CMake GUI to build Companion for your platform.
Enable the Cuda flag to build Companion with implemented Cuda features.

```
cmake -DCompanion_USE_CUDA=ON/OFF
make
make install
```

# Build Companion Samples

[Samples](https://github.com/LibCompanion/CompanionSamples) are included as a submodule or can be referenced via the CMake variable `Companion_SAMPLE_MODULE`. To build the samples you have to enable the `Companion_BUILD_SAMPLES` flag.

## Build Samples from Submodule

```
git submodule update --init --recursive
cmake -DCompanion_BUILD_SAMPLES=ON
make
```

## Build Samples from Project

```
git clone https://github.com/LibCompanion/CompanionSamples.git
cmake -DCompanion_BUILD_SAMPLES=ON -DCompanion_SAMPLE_MODULE=/path/to/CompanionSamples/
make
```

## UWP Support

If you desire to build Companion for *Universal Windows Platform* you can simply use the provided toolchain file to do so.
```
cmake -DCMAKE_TOOLCHAIN_FILE=UWPToolchain.cmake
```
If you prefer the CMake GUI instead, make sure to add the variable `CMAKE_TOOLCHAIN_FILE` to the configuration before pressing *Configure*. Select FILEPATH as the variable type and choose the file `UWPToolchain.cmake` as the value.

## License

```
This program is an object recognition framework written with OpenCV.
Copyright (C) 2016-2018 Andreas Sekulski, Dimitri Kotlovsky

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
```
